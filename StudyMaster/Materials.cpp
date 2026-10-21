#include "Materials.h"

#include <curl/curl.h>

#include <cmath>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr double AtomicMassEnergyPerU = 931494.10242;

size_t WriteCallback(void* contents, size_t size, size_t numberOfMembers, void* userData)
{
    const size_t totalSize = size * numberOfMembers;
    auto* output = static_cast<std::string*>(userData);
    output->append(static_cast<const char*>(contents), totalSize);
    return totalSize;
}

std::string Trim(std::string value)
{
    const auto first = value.find_first_not_of(" \t\r");
    if (first == std::string::npos) {
        return {};
    }

    const auto last = value.find_last_not_of(" \t\r");
    return value.substr(first, last - first + 1);
}

std::vector<std::string> ParseCsvLine(const std::string& line)
{
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;

    for (size_t i = 0; i < line.size(); ++i) {
        const char character = line[i];

        if (character == '"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            }
            else {
                quoted = !quoted;
            }
        }
        else if (character == ',' && !quoted) {
            fields.push_back(Trim(field));
            field.clear();
        }
        else {
            field += character;
        }
    }

    fields.push_back(Trim(field));
    return fields;
}

bool TryParseInt(const std::string& text, int& value)
{
    try {
        size_t position = 0;
        const int parsed = std::stoi(text, &position);
        if (position != text.size()) {
            return false;
        }

        value = parsed;
        return true;
    }
    catch (...) {
        return false;
    }
}

bool TryParseDouble(const std::string& text, double& value)
{
    try {
        size_t position = 0;
        const double parsed = std::stod(text, &position);
        if (position != text.size() || !std::isfinite(parsed)) {
            return false;
        }

        value = parsed;
        return true;
    }
    catch (...) {
        return false;
    }
}

std::string DownloadGroundStates()
{
    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
        throw std::runtime_error("Failed to initialize cURL.");
    }

    std::string csvData;
    curl_easy_setopt(curl, CURLOPT_URL, "https://nds.iaea.org/relnsd/v1/data?fields=ground_states&nuclides=all");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "StudyMaster/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &csvData);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    const CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK) {
        throw std::runtime_error("IAEA request failed: " + std::string(curl_easy_strerror(result)));
    }

    return csvData;
}

}

std::vector<element> LoadIsotopes()
{
    const std::string csvData = DownloadGroundStates();
    std::istringstream input(csvData);
    std::string line;

    if (!std::getline(input, line)) {
        throw std::runtime_error("The IAEA response did not contain a header.");
    }

    const std::vector<std::string> headers = ParseCsvLine(line);
    std::unordered_map<std::string, size_t> column;
    for (size_t i = 0; i < headers.size(); ++i) {
        column[headers[i]] = i;
    }

    const auto findColumn = [&column](const std::string& name) -> size_t {
        const auto found = column.find(name);
        if (found == column.end()) {
            throw std::runtime_error("Missing IAEA CSV column: " + name);
        }
        return found->second;
    };

    const size_t zColumn = findColumn("z");
    const size_t nColumn = findColumn("n");
    const size_t symbolColumn = findColumn("symbol");
    const size_t halfLifeColumn = findColumn("half_life_sec");
    const size_t halfLifeTextColumn = findColumn("half_life");
    const size_t spinParityColumn = findColumn("jp");
    const size_t atomicMassColumn = findColumn("atomic_mass");

    std::vector<element> isotopes;
    while (std::getline(input, line)) {
        const std::vector<std::string> fields = ParseCsvLine(line);
        if (fields.size() <= atomicMassColumn || fields.size() <= spinParityColumn) {
            continue;
        }

        int z = 0;
        int n = 0;
        double atomicMassEnergy = 0.0;
        if (!TryParseInt(fields[zColumn], z) ||
            !TryParseInt(fields[nColumn], n) ||
            !TryParseDouble(fields[atomicMassColumn], atomicMassEnergy) ||
            fields[symbolColumn].empty()) {
            continue;
        }

        double halfLifeSeconds = 0.0;
        if (!fields[halfLifeColumn].empty()) {
            TryParseDouble(fields[halfLifeColumn], halfLifeSeconds);
        }

        const std::string halfLifeText = fields[halfLifeTextColumn];
        const bool isRadioactive = !halfLifeText.empty() && halfLifeText != "STABLE";

        isotopes.push_back({
            fields[symbolColumn],
            z,
            z + n,
            atomicMassEnergy / AtomicMassEnergyPerU,
            halfLifeSeconds,
            isRadioactive,
            fields[spinParityColumn]
        });
    }

    if (isotopes.empty()) {
        throw std::runtime_error("The IAEA response contained no usable isotopes.");
    }

    return isotopes;
}

element Materials()
{
    static const std::vector<element> isotopes = LoadIsotopes();
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> distribution(0, isotopes.size() - 1);
    return isotopes[distribution(generator)];
}
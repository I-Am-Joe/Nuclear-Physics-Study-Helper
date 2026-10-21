#pragma once

#include <string>
#include <vector>

struct element {
    std::string symbol;
    int Z;
    int A;
    double atomicMassU;
    double halfLifeSeconds;
    bool isRadioactive;
    std::string groundState;
};

std::vector<element> LoadIsotopes();
element Materials();

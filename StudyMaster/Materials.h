#pragma once

#include <string>

struct element {
	std::string symbol;
	int Z;
	int A;
	int MolarMass;
};

element Materials();

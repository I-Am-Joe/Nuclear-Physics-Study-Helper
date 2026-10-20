#include <iostream>
#include <iomanip>
#include <random>
#include "Materials.h"

element Materials() {
	element Materials[15] = { {"He", 2, 4}, {"H", 1, 1}, {"U", 92, 238}, {"Li", 3, 7}, {"K", 19, 39}, {"Be", 4, 9}, {"B", 5, 11}, {"C", 6, 12 },
	{"O", 8, 16},
	{"Fe", 26, 56},
	{"Ni", 28, 58},
	{"Pb", 82, 208},
	{"Th", 90, 232},
	{"I", 53, 127},
	{"Cs", 55, 133} };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, 14);
	std::uniform_int_distribution<int> u_dist(214, 242);
	std::uniform_int_distribution<int> h_dist(1, 3);
	std::uniform_int_distribution<int> li_dist(6, 8);
	std::uniform_int_distribution<int> k_dist(38, 40);
	std::uniform_int_distribution<int> be_dist(8, 10);
	std::uniform_int_distribution<int> b_dist(10, 12);
	std::uniform_int_distribution<int> c_dist(12, 14);
	std::uniform_int_distribution<int> o_dist(16, 18);
	std::uniform_int_distribution<int> fe_dist(54, 58);
	std::uniform_int_distribution<int> ni_dist(58, 64);
	std::uniform_int_distribution<int> pb_dist(204, 208);
	std::uniform_int_distribution<int> th_dist(232, 232);
	std::uniform_int_distribution<int> cs_dist(112, 151);
	std::uniform_int_distribution<int> I_dist(108, 144);

	for (int i = 0; i < 15; i++) {
		if (Materials[i].symbol == "U") {
			Materials[i].A = u_dist(gen);
		}
		else if (Materials[i].symbol == "H") {
			Materials[i].A = h_dist(gen);
		}
		else if (Materials[i].symbol == "Li") {
			Materials[i].A = li_dist(gen);
		}
		else if (Materials[i].symbol == "K") {
			Materials[i].A = k_dist(gen);
		}
		else if (Materials[i].symbol == "Be") {
			Materials[i].A = be_dist(gen);
		}
		else if (Materials[i].symbol == "B") {
			Materials[i].A = b_dist(gen);
		}
		else if (Materials[i].symbol == "C") {
			Materials[i].A = c_dist(gen);
		}
		else if (Materials[i].symbol == "O") {
			Materials[i].A = o_dist(gen);
		}
		else if (Materials[i].symbol == "Fe") {
			Materials[i].A = fe_dist(gen);
		}
		else if (Materials[i].symbol == "Ni") {
			Materials[i].A = ni_dist(gen);
		}
		else if (Materials[i].symbol == "Pb") {
			Materials[i].A = pb_dist(gen);
		}
		else if (Materials[i].symbol == "Th") {
			Materials[i].A = th_dist(gen);
		}
		else if (Materials[i].symbol == "Cs") {
			Materials[i].A = cs_dist(gen);
		}
		else if (Materials[i].symbol == "I") {
			Materials[i].A = I_dist(gen);
		}
		Materials[i].MolarMass = Materials[i].A;
	}
	return Materials[distrib(gen)];
}
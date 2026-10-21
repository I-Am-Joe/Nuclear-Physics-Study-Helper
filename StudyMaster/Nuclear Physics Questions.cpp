#include <iostream>
#include <iomanip>
#include <random>
#include "Materials.h"
#include <iostream>
#include <iomanip>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <sstream>
std::random_device rd;
std::mt19937 gen(rd());

std::string generateQuestion(int questionType, element MaterialChose, matlab::engine::MATLABEngine* matlab) {
	std::string question;
	std::uniform_int_distribution<int> halflife(1, 20);
	std::uniform_real_distribution<double> NumberOfAtoms(2.0e23, 3.45e25);
	std::uniform_int_distribution<int> Time(1, 100);
	std::ostringstream formatted;
	double QuestionAnswer;
	formatted << std::scientific << std::setprecision(3) << NumberOfAtoms(gen);
    double halfLifeValue = static_cast<double>(halflife(gen));

    matlab::data::ArrayFactory factory;
    matlab->setVariable(
        u"halfLife",
        factory.createScalar<double>(halfLifeValue)
    );
	
    matlab->eval(u"decay_constant = log(2) / (halfLife*3600);");
    matlab::data::TypedArray<double> decayConstant = matlab->getVariable(u"decay_constant");

	matlab->setVariable(
		u"initialAtoms",
		factory.createScalar<double>(NumberOfAtoms(gen))
	);
	matlab->setVariable(
		u"time",
		factory.createScalar<double>(Time(gen))
	);
	matlab->eval(u"remaining_atoms = initialAtoms * exp(-decay_constant * time * 3600);");
	matlab::data::TypedArray<double> remainingAtoms = matlab->getVariable(u"remaining_atoms");
	matlab->eval(u"activity = decay_constant * remaining_atoms;");
	matlab::data::TypedArray<double> activity = matlab->getVariable(u"activity");
	std::uniform_int_distribution<int> Questionaire(1, 3);
std::uniform_int_distribution<int>Questionaire2(1, 2);
	double as = 18.3;
	double av = 15.8;
	double ac = 0.714;
	double aa = 23.2;

	double mn = 1.00866491606;
	double mp = 1.007276466621;
	double c = 931.5;


	switch (questionType) {
    case 1:
    {
		question = "A parent Isotope  " + MaterialChose.symbol + "-" +std::to_string(MaterialChose.A) + " has a half-life of " +std::to_string(halflife(gen)) +" hours. The atomic mass of this isotope is " +std::to_string(MaterialChose.atomicMassU) +" u. It has an initial value of atoms at: " +formatted.str() +" atoms. Find the: ";
        switch (Questionaire(gen)) {
		case 1:
		{
			question += "decay constant.";

			std::cout << question << '\n';
			std::cout << "Your answer: ";
			std::cin >> QuestionAnswer;
			double error = ((std::abs(QuestionAnswer - decayConstant[0]))/ decayConstant[0]) * 100;
			if (error < 5) {
				std::cout << "Correct answer! The decay constant is: "
					<< decayConstant[0] << " s^-1.\n";
			}
			else {
				std::cout << "Incorrect answer.\n";
			}
			break;
		}
		case 2:
		{
			question += "number of atoms after " +
				std::to_string(Time(gen)) + " hours.";
			std::cout << question << '\n';
			std::cout << "Your answer: ";
			std::cin >> QuestionAnswer;
			double error = ((std::abs(QuestionAnswer - remainingAtoms[0]))/ remainingAtoms[0]) * 100;
			if (error < 5) {
				std::cout << "Correct answer! The number of atoms is: "
					<< remainingAtoms[0] << " atoms.\n";
			}
			else {
				std::cout << "Incorrect answer.\n";
			}
			break;
		}
        case 3:
            question += "activity after " +
                std::to_string(Time(gen)) + " hours.";
			std::cout << question << '\n';
			std::cout << "Your answer: ";
            std::cin >> QuestionAnswer;
			double error = ((std::abs(QuestionAnswer - activity[0])))/activity[0] * 100;
			if (error < 5) {
				std::cout << "Correct answer! The activity is: "
					<< activity[0] << " Bq.\n";
			}
			else {
				std::cout << "Incorrect answer.\n";
			}
            break;
        }

        break;
    }
	case 2:
	{

		question = "Given : a_s = 18.3 MeV, a_v = 15.8 MeV, a_c = 0.714 MeV, a_a = 23.2 MeV, and a_p = 12.0 MeV, as well as " + std::to_string(MaterialChose.Z) + " and " + std::to_string(MaterialChose.A) + ", determine the: ";
		int N = MaterialChose.A - MaterialChose.Z;
		switch (Questionaire2(gen)) {
		case 1:
			question += "nuclear total binding energy of the nucleus.";
			std::cout << question << '\n';
			std::cout << "Your answer: ";
			std::cin >> QuestionAnswer;

			if (N % 2 == 0 && MaterialChose.Z % 2 == 0) {
				std::cout << "The nucleus is even-even, so the binding energy is higher than average.\n";
				double bindingEnergy = (av * MaterialChose.A) - (as * pow(MaterialChose.A, 2.0 / 3.0)) - (ac * ((MaterialChose.Z * (MaterialChose.Z - 1)) / pow(MaterialChose.A, 1.0 / 3.0))) - (aa * (pow((MaterialChose.A - 2 * MaterialChose.Z), 2.0) / MaterialChose.A)) + (12 * pow(MaterialChose.A, -1.0 / 2.0));
				double error = ((std::abs(QuestionAnswer - bindingEnergy) / bindingEnergy) * 100);
				if (error < 5) {
					std::cout << "Correct answer! The binding energy is: "
						<< bindingEnergy << " MeV.\n";
				}
				else {
					std::cout << "Incorrect answer.\n" << bindingEnergy << " MeV.\n";
				}
			}
			else if (N % 2 != 0 && MaterialChose.Z % 2 != 0) {
				std::cout << "The nucleus is odd-odd, so the binding energy is lower than average.\n";
				double bindingEnergy = (av * MaterialChose.A) - (as * pow(MaterialChose.A, 2.0 / 3.0)) - (ac * ((MaterialChose.Z * (MaterialChose.Z - 1)) / pow(MaterialChose.A, 1.0 / 3.0))) - (aa * (pow((MaterialChose.A - 2 * MaterialChose.Z), 2.0) / MaterialChose.A)) - (12 * pow(MaterialChose.A, -1.0 / 2.0));
				double error = ((std::abs(QuestionAnswer - bindingEnergy) / bindingEnergy) * 100);
				if (error < 5) {
					std::cout << "Correct answer! The binding energy is: "
						<< bindingEnergy << " MeV.\n";
				}
				else {
					std::cout << "Incorrect answer.\n" << bindingEnergy << " MeV.\n";
				}
			}
			else {
				std::cout << "The nucleus is odd-even or even-odd, so the binding energy is average.\n";
				double bindingEnergy = (av * MaterialChose.A) - (as * pow(MaterialChose.A, 2.0 / 3.0)) - (ac * ((MaterialChose.Z * (MaterialChose.Z - 1)) / pow(MaterialChose.A, 1.0 / 3.0))) - (aa * (pow((MaterialChose.A - 2 * MaterialChose.Z), 2.0) / MaterialChose.A));
				double error = ((std::abs(QuestionAnswer - bindingEnergy) / bindingEnergy) * 100);
				if (error < 5) {
					std::cout << "Correct answer! The binding energy is: "
						<< bindingEnergy << " MeV.\n";
				}
				else {
					std::cout << "Incorrect answer.\n" << bindingEnergy << " MeV.\n";
				}
			}
			break;
		case 2: {
			question += "the specific binding energy per nucleon for the specified nucleus.";
			std::cout << question << '\n';
			std::cout << "Your answer: ";
			std::cin >> QuestionAnswer;


			if (N % 2 == 0 && MaterialChose.Z % 2 == 0) {
				std::cout << "The nucleus is even-even, so the binding energy is higher than average.\n";
				double bindingEnergy = (av * MaterialChose.A) - (as * pow(MaterialChose.A, 2.0 / 3.0)) - (ac * ((MaterialChose.Z * (MaterialChose.Z - 1)) / pow(MaterialChose.A, 1.0 / 3.0))) - (aa * (pow((MaterialChose.A - 2 * MaterialChose.Z), 2.0) / MaterialChose.A)) + (12 * pow(MaterialChose.A, -1.0 / 2.0));
				double specificBindingEnergy = bindingEnergy / MaterialChose.A;
				double error = ((std::abs(QuestionAnswer - specificBindingEnergy) / specificBindingEnergy) * 100);
				if (error < 5) {
					std::cout << "Correct answer! The specific binding energy is: "
						<< specificBindingEnergy << " MeV.\n";
				}
				else {
					std::cout << "Incorrect answer.\n";
				}
			}
			else if (N % 2 != 0 && MaterialChose.Z % 2 != 0) {
				std::cout << "The nucleus is odd-odd, so the binding energy is lower than average.\n";
				double bindingEnergy = (av * MaterialChose.A) - (as * pow(MaterialChose.A, 2.0 / 3.0)) - (ac * ((MaterialChose.Z * (MaterialChose.Z - 1)) / pow(MaterialChose.A, 1.0 / 3.0))) - (aa * (pow((MaterialChose.A - 2 * MaterialChose.Z), 2.0) / MaterialChose.A)) - (12 * pow(MaterialChose.A, -1.0 / 2.0));
				double specificBindingEnergy = bindingEnergy / MaterialChose.A;
				double error = ((std::abs(QuestionAnswer - specificBindingEnergy) / specificBindingEnergy) * 100);
				if (error < 5) {
					std::cout << "Correct answer! The specific binding energy is: "
						<< specificBindingEnergy << " MeV.\n";
				}
				else {
					std::cout << "Incorrect answer.\n";
				}
			}
			else {
				std::cout << "The nucleus is odd-even or even-odd, so the binding energy is average.\n";
				double bindingEnergy = (av * MaterialChose.A) - (as * pow(MaterialChose.A, 2.0 / 3.0)) - (ac * ((MaterialChose.Z * (MaterialChose.Z - 1)) / pow(MaterialChose.A, 1.0 / 3.0))) - (aa * (pow((MaterialChose.A - 2 * MaterialChose.Z), 2.0) / MaterialChose.A));
				double specificBindingEnergy = bindingEnergy / MaterialChose.A;
				double error = ((std::abs(QuestionAnswer - specificBindingEnergy) / specificBindingEnergy) * 100);
				if (error < 5) {
					std::cout << "Correct answer! The specific binding energy is: "
						<< specificBindingEnergy << " MeV.\n";
				}
				else {
					std::cout << "Incorrect answer.\n";
				}
			}
			break;
			break;
		}
			  break;
		}
		break;
	}
	case 3: {


		question = "Calculate the binding energy of the nucleus " + MaterialChose.symbol + "-" + std::to_string(MaterialChose.A) + " with a Z value of: " + std::to_string(MaterialChose.Z);
		+" using the Mass defect method.";
		std::cout << question << '\n';
		std::cout << "Your answer: ";
		std::cin >> QuestionAnswer;
		int Z = MaterialChose.Z;
		int A = MaterialChose.A;
		int N = A - Z;
		double massDefect = ((Z * mp)+ (N * mn)) - MaterialChose.atomicMassU;
		double bindingEnergys = massDefect * c;
		double error = ((std::abs(QuestionAnswer - bindingEnergys) / bindingEnergys) * 100);
		if (error < 5) {
			std::cout << "Correct answer! The binding energy is: "
				<< bindingEnergys << " MeV.\n";
		}
		else {
			std::cout << "Incorrect answer.\n" << bindingEnergys << " MeV.\n";
		}
		break;
	}


	case 4:
		question = "Discuss the nuclear models and parity for the given nucleus.";
		break;
	default:
		question = "Invalid question type selected.";
		break;
	}
	return question;
}


int main() {
    int Answer;
	int again = 1;
	auto matlab = matlab::engine::startMATLAB();
	while (again == 1) {
	std::cout << "Enter the type of question you want to generate: \n" << "1. Half-life\n"
		<< "2. Weizaicker formula\n 3. binding energy\n 4.Nuclear models and paraity\n 5.Exit\n";

    std::cin >> Answer;
	if (Answer == 5) { 
		break;
		again = 5;
	}
	element material = Materials();
	std::cout << "Selected isotope: " << material.symbol << "-" << material.A << std::endl;
	std::cout << "Atomic mass: " << material.atomicMassU << " u" << std::endl;
	std::cout << "Atomic number: " << material.Z << std::endl;

    switch (Answer) {

	case 1: {
        std::cout << "You selected Half-life question." << std::endl;
		generateQuestion(1, material, matlab.get());
        break;
	}

	case 2:
		std::cout << "You selected Weizaicker formula question." << std::endl;
		generateQuestion(2, material, matlab.get());
		break;
    case 3:
		std::cout << "You selected Nuclear binding energy question." << std::endl;
		generateQuestion(3, material, matlab.get());
		break;
    case 4:
		std::cout << "You selected Nuclear models and parity question." << std::endl;
		generateQuestion(4, material, matlab.get());
		break;

	default:
		std::cout << "Invalid Lil boi :skull: :skull:" << std::endl;
		return 0;
    }
	std::cout << "\nEnter 1 to generate another question " << "or 2 to exit: ";
	std::cin >> again;
	}
    std::cin.get();
}


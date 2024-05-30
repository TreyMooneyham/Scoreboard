#include "settings.h"
//#include "charAttributes.h"
#include <fstream>
#include <iostream>

namespace Settings {
	void newCharacter() {

	}

	void saveCharacter(playerCharacter pc) {
		nlohmann::json jsonObj = pc.toJson();
		std::string filename = pc.getName(1) + ".json";
		std::replace(filename.begin(), filename.end(), ' ', '_');
		std::ofstream file (filename);
		if (file.is_open()) {
			file << jsonObj.dump(4);  // Pretty print with 4 spaces indentation
			file.close();
			std::cout << "Object saved to " << filename << std::endl;
		}
		else {
			std::cerr << "Could not open file for writing: " << filename << std::endl;
		}
	}

	void saveAsCharacter(playerCharacter pc) {
		nlohmann::json jsonObj = pc.toJson();
		std::string filename;
		std::cout << "Enter a name for this file: ";
		std::getline(std::cin, filename);
		filename += ".json";
		std::ofstream file(filename);
		if (file.is_open()) {
			file << jsonObj.dump(4);  // Pretty print with 4 spaces indentation
			file.close();
			std::cout << "Object saved to " << filename << std::endl;
		}
		else {
			std::cerr << "Could not open file for writing: " << filename << std::endl;
		}
	}

	void loadCharacter() {

	}
}
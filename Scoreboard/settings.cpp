#include "settings.h"
//#include "charAttributes.h"
#include <fstream>
#include <iostream>
namespace Settings {
	void newCharacter() {

	}

	void saveCharacter(playerCharacter pc, std::string filename) {
		nlohmann::json jsonObj = pc.toJson();

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

	void saveAsCharacter() {

	}

	void loadCharacter() {

	}
}
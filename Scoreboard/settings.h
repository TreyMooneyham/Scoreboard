// Includes
#pragma once
#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

#include "charAttributes.h"

namespace Settings {
	void newCharacter(playerCharacter& pc);
	void saveCharacter(playerCharacter pc);
	void saveAsCharacter(playerCharacter pc, std::string fn);
	void loadCharacter(playerCharacter& pc, std::string fp);
}
//#include "nlohmann/json.hpp"
#include "hpWindow.h"

namespace Settings {
	void newCharacter();
	void saveCharacter(playerCharacter pc, std::string filename);
	void saveAsCharacter();
	void loadCharacter();
}
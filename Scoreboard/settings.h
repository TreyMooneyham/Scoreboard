//#include "nlohmann/json.hpp"
#include "hpWindow.h"

namespace Settings {
	void newCharacter();
	void saveCharacter(playerCharacter pc);
	void saveAsCharacter(playerCharacter pc);
	void loadCharacter(playerCharacter& pc);
}
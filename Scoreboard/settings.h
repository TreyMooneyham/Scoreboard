//#include "nlohmann/json.hpp"
#include "charAttributes.h"
namespace Settings {
	void newCharacter();
	void saveCharacter(playerCharacter pc, std::string filename);
	void saveAsCharacter();
	void loadCharacter();
}
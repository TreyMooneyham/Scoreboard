//#include "settings.h"
#include "nlohmann/json.hpp"
// Levels
enum class levels {
	character,		// Character level
	warrior,		// Warrior, spellcaster, and rogue "level" is just the amount of features from their respective section
	spellcaster,
	rogue
};

// Ability Scores
enum class abilityScores {
	strength, dexterity, constitution,	// Physical Abilities
	intelligence, wisdom, charisma		// Mental Abilities
};

// Proficiencies
enum class proficiencies {
	noProficiency,	// No bonus
	proficiency,	// Bonus is 2 + level
	expertise,		// Bonus is 4 + level
	mastery,		// Bonus is 6 + level
	legendary		// Bonus is 8 + level
};

struct playerCharacter {
	std::string charName;
	std::string playerName;
	std::map<levels, int> levelInfo;
	std::map<abilityScores, int> abilities;

	void setPName(std::string name);
	std::string getPName();
	void setCName(std::string name);
	std::string getCName();
	void initScores();
	void setScore(abilityScores ability, int score);
	int getScore(abilityScores ability);


	nlohmann::json toJson() const {
		nlohmann::json jsonObj;
		jsonObj["charName"] = charName;
		jsonObj["playerName"] = playerName;

		nlohmann::json levelInfoJson;
		levelInfoJson["character"] = levelInfo.at(levels::character);
		jsonObj["levelInfo"] = levelInfoJson;

		nlohmann::json abilitiesJson;
		abilitiesJson["strength"] = abilities.at(abilityScores::strength);
		abilitiesJson["dexterity"] = abilities.at(abilityScores::dexterity);
		abilitiesJson["constitution"] = abilities.at(abilityScores::constitution);
		abilitiesJson["intelligence"] = abilities.at(abilityScores::intelligence);
		abilitiesJson["wisdom"] = abilities.at(abilityScores::wisdom);
		abilitiesJson["charisma"] = abilities.at(abilityScores::charisma);
		jsonObj["abilities"] = abilitiesJson;

		return jsonObj;
	}

};

// Functions

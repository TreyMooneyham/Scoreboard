#include "settings.h"

struct playerCharacter {
	std::string charName;
	std::string playerName;
	std::map<levels, int> levelInfo;
	std::map<abilityScores, int> abilityScoreInfo;
};

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
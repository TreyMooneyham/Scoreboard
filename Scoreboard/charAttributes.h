#include "settings.h"

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
enum class proficiencyLevels {
	noProficiency,	// No bonus
	proficiency,	// Bonus is 2 + level
	expertise,		// Bonus is 4 + level
	mastery,		// Bonus is 6 + level
	legendary		// Bonus is 8 + level
};

// Skills
enum class skills : int {
	acrobatics, animalHandling, arcana, athletics,
	crafting, deception, history, insight,
	intimidation, investigation, medicine, nature,
	perception, performance, persuasion, religion,
	sleightOfHand, stealth, survival
};

struct skill {
public:
	proficiencyLevels	profLevel;
	abilityScores		mainAbility;

	std::string getSkillName(skills skill);
};

// playerCharacter holds all the information for the currently loaded character
struct playerCharacter {
	std::string charName;
	std::string playerName;
	std::map<levels, int> levelInfo;
	std::map<abilityScores, int> abilities;
	std::map<skills, skill> skillInfo;

	void setPName(std::string name);
	std::string getPName();
	void setCName(std::string name);
	std::string getCName();
	void initScores();
	void setScore(abilityScores ability, int score);
	int getScore(abilityScores ability);
	void setSkillProficiency(skills skill, proficiencyLevels level);
	proficiencyLevels getSkillProficiency(skills skill);
	void setSkillAbility(skills skill, abilityScores ability);
	abilityScores getSkillAbility(skills skill);
};
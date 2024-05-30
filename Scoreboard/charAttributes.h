//#include "settings.h"
#include "nlohmann/json.hpp"

// Names
struct names {
public:
	std::string charName;
	std::string playerName;
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
enum class proficiencyLevels {
	noProficiency,	// No bonus
	proficiency,	// Bonus is 2 + level
	expertise,		// Bonus is 4 + level
	mastery,		// Bonus is 6 + level
	legendary		// Bonus is 8 + level
};

// Skills
enum class skills {
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

// Hit points
struct hitPoints {
public:
	int rolledHP;
	int tempHP;
	int currentHP;
};

// playerCharacter holds all the information for the currently loaded character
struct playerCharacter {
	// Data Types
	names							nameInfo;
	std::map<levels, int>			levelInfo;
	std::map<abilityScores, int>	abilities;
	std::map<skills, skill>			skillInfo;
	hitPoints						hpInfo;

	// Setters and getters
	void				setName(std::string name, int type);
	std::string			getName(int type);

	void				setLevel(levels section, int level);
	int					getLevel(levels section);

	void				initScores();
	void				setScore(abilityScores ability, int score);
	int					getScore(abilityScores ability);
	int					getMod(abilityScores ability);

	void				initSkills();
	void				setSkillProficiency(skills skill, proficiencyLevels level);
	proficiencyLevels	getSkillProficiency(skills skill);
	void				setSkillAbility(skills skill, abilityScores ability);
	abilityScores		getSkillAbility(skills skill);
	int					calcSkillProfBonus(skills s);

	void				setHP(int hp, int type);
	int					getHP(int type);
	void				initHitPoints();

	nlohmann::json		toJson() const;
};
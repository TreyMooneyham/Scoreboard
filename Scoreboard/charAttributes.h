//#include "settings.h"
#include "nlohmann/json.hpp"

// Names
struct names {
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
	proficiencyLevels	profLevel;
	abilityScores		mainAbility;

	std::string getSkillName(skills skill);
};

// Hit points
struct hitPoints {
	int rolledHP;
	int tempHP;
	int currentHP;
};

/*
All feats:
Name, minimum level, hit die, description
Most feats:
Minimum in a score, special actions/benefits
Some feats:
Prerequisite feat, Restricted feat (This feat cannot be taken if that feat is taken and vice versa)
You can comfortably ignore special actions/benefits for now
*/
struct feat {
	// Reqired
	std::string				name;
	int						id;
	int						minLevel;
	int						hitDie;
	std::string				description;

	// Most feats
	int						minScore;
	//Special shit

	// Some feats
	std::map<feat, bool>	prerequisiteFeats;
	std::map<feat, bool>	RestrictedFeats;

	// Functions
	void setName(std::string name);
	void setID(int id);
	void setMinLevel(int lvl);
	void setHitDie(int hd);
	void setDesc(std::string desc);
	void setMinScore(abilityScores ability, int score);
	void featPrereq(feat reqFeat[]);
	void featRestrict(feat resFeat[]);
};

// playerCharacter holds all the information for the currently loaded character
struct playerCharacter {
	// Data Types
	names							nameInfo;
	std::map<levels, int>			levelInfo;
	std::map<abilityScores, int>	abilities;
	std::map<skills, skill>			skillInfo;
	hitPoints						hpInfo;
	std::map<feat, bool>			feats;

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
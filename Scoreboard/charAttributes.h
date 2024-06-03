// Includes
#pragma once
#include <string>
#include <stdio.h>
#include <map>

#include "nlohmann/json.hpp"
#include <vector>

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


struct feat {
	// Reqired
	std::string						name;
	int								id;
	int								minLevel;
	int								hitDie;
	std::string						description;

	// Most feats
	int								minScore;
	std::vector<abilityScores>		minScoreAbility;

	// Some feats
	std::vector<int>				prerequisiteFeats;
	std::vector<int>				restrictedFeats;

	bool							findFeat(int id, std::vector<int> list);
};

struct item {
	std::string						name;
	int								id;
	double							cost; // This will be in gp
	double							weight;
	std::string						description;

	int								minScore;
	std::vector<abilityScores>		minScoreAbility;

	int								scoreMod;
	std::vector<abilityScores>		modifiedScores;
};

// Resistances
enum class resistanceTypes {
	mechanical,
	bludgeoning, force, piercing, slashing,
	nonMechanical,
	acid, bio, cold, electricity, energy, heat, thunder, psychic
};

// Resistance struct
struct resistance {
	// dt is damage threshold, and dr is damage resistance
	resistanceTypes type;
	int dt;
	float dr;
};

// playerCharacter holds all the information for the currently loaded character
struct playerCharacter {
	// Data Types
	names							nameInfo;
	std::map<levels, int>			levelInfo;
	std::map<abilityScores, int>	abilities;
	std::map<skills, skill>			skillInfo;
	hitPoints						hpInfo;
	std::vector<int>				feats;
	std::map<resistanceTypes, resistance>	resistInfo;
	std::vector<int>				inventory;


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

	void				setDT(resistanceTypes type, int t);
	void				setDR(resistanceTypes type, float r);
	void				setDTR(resistanceTypes type, int t, float r);
	int					getDT(resistanceTypes type);
	float				getDR(resistanceTypes type);
	void				initResist();
	bool				isMech(resistanceTypes type);

	nlohmann::json		toJson() const;
};

std::vector<feat> initFeats();
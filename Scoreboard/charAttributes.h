// Includes
#pragma once
#include <string>
#include <stdio.h>
#include <map>

#include "nlohmann/json.hpp"
#include <vector>

// Names & info
struct names {
	// Names
	std::string charName;
	std::string playerName;

	// Info
	std::string ancestry;
	std::string nationality;
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

// Item struct
struct item {
	std::string						name;
	int								id;
	double							cost; // This will be in gp
	double							weight;
	std::string						description;
	int								count;

	int								minScore;
	std::vector<abilityScores>		minScoreAbility;

	int								scoreMod;
	std::vector<abilityScores>		modifiedScores;
};

// Conditions Enum
// There are currently 30 conditions as of 6/19/24
enum class conditions {
	blinded, 
	clumsy, confused, controlled,
	dazed, deafened, doomed, drained, dying,
	encumbered, enfeebled,
	fatigued, fleeing, frightened,
	grappled,
	hidden,
	immobilized, invisible,
	paralyzed, petrified, prone,
	quickened,
	restrained,
	sickened, slowed, stabilized, stunned, stupefied,
	unconscious, undetected
};

//Movement enum
enum class movements {
	walking,
	swimming,
	climbing,
	flying,
	burrowing
};

// Saving throw enum
enum class savingThrows {
	fortitude, reflex, will
};

// Saving throw struct
struct savingThrow {
	//abilityScores		mainAbility; // One day, sweet prince...
	proficiencyLevels	profLevel;
};

enum class armorTypes {
	unarmored,
	light,
	medium,
	heavy
};

struct armorClass {	
	int					baseAC;
	abilityScores		mainAbility;
	proficiencyLevels	profLevel;
};

// playerCharacter holds all the information for the currently loaded character
struct playerCharacter {
	// Data Types
	names									nameInfo;
	std::map<levels, int>					levelInfo;
	std::map<abilityScores, int>			abilities;
	std::map<skills, skill>					skillInfo;
	std::map<savingThrows, savingThrow>		savesInfo;
	hitPoints								hpInfo;
	std::map<resistanceTypes, resistance>	resistInfo;
	std::vector<int>						feats;
	std::vector<item>						inventory;
	std::map<movements, int>				movementInfo;
	std::map<armorTypes, armorClass>		armorInfo;
	std::map<abilityScores, int>			abilityAdj;
	std::map<skills, int>					skillAdj;
	std::map<movements, int>				movementAdj;
	std::map<savingThrows, int>				saveAdj;
	std::map<conditions, int>				conditionInfo;
	

	// Setters and getters
	void				setName(std::string name, int type);
	std::string			getName(int type);
	void				initNames();

	void				setLevel(levels section, int level);
	int					getLevel(levels section);
	void				initLevels();

	void				initScores();
	void				setScore(abilityScores ability, int score);
	int					getScore(abilityScores ability);
	int					getMod(abilityScores ability);

	int					getProfBonus(proficiencyLevels level);

	void				initSkills();
	void				setSkillProficiency(skills skill, proficiencyLevels level);
	proficiencyLevels	getSkillProficiency(skills skill);
	void				setSkillAbility(skills skill, abilityScores ability);
	abilityScores		getSkillAbility(skills skill);

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

	void				setAdj(abilityScores score, int val);
	void				setAdj(skills skill, int val);
	void				setAdj(movements type, int val);
	void				setAdj(savingThrows save, int val);
	void				setAdj(int armor);
	void				addAdj(abilityScores score, int val);
	void				addAdj(skills skill, int val);
	void				addAdj(movements type, int val);
	void				addAdj(savingThrows save, int val);
	void				addAdj(int armor);
	int					getAdj(abilityScores score);
	int					getAdj(skills skill);
	int					getAdj(movements type);
	int 				getAdj(savingThrows save);
	int					getArmorAdj();
	void				initAdj();

	void				setCondition(conditions cond, int val);
	int					getCondition(conditions cond);
	void				initConditions();

	void				setSpeed(movements type, int speed);
	int					getSpeed(movements type);
	void				initSpeeds();

	void				setSaveProficiency(savingThrows save, proficiencyLevels prof);
	proficiencyLevels	getSaveProficiency(savingThrows save);
	int					getSave(savingThrows save);
	void				initSavingThrows();

	nlohmann::json		toJson() const;
};

std::vector<feat> initFeats();
std::vector<item> initItems();
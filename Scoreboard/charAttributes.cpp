#include <string>
#include "charAttributes.h"
#include <stdio.h>

// Sets the player name for a given character
// -1 for both, 0 for player name, and 1 for character name
void playerCharacter::setName(std::string name, int type) {
	switch (type) {
	case -1:
		this->nameInfo.charName = name;
		this->nameInfo.playerName = name;
		break;
	case 0:
		this->nameInfo.playerName = name;
		break;
	case 1:
		this->nameInfo.charName = name;
		break;
	default:
		this->nameInfo.charName = "Invalid";
		this->nameInfo.playerName = "Invalid";
		break;
	}
}

// Gets the player name for a given character
// 0 for player name, and 1 for character name
std::string playerCharacter::getName(int type) {
	switch (type) {
	case 0:
		return this->nameInfo.playerName;
	case 1:
		return this->nameInfo.charName;
	default:
		return "Invalid";
	}
}

void playerCharacter::setLevel(levels section, int level) {
	this->levelInfo[section] = level;
}

int playerCharacter::getLevel(levels section) {
	return this->levelInfo[section];
}

// Initiatilizes every score to 10
void playerCharacter::initScores() {
	this->abilities[abilityScores::strength]		= 10;
	this->abilities[abilityScores::dexterity]		= 10;
	this->abilities[abilityScores::constitution]	= 10;
	this->abilities[abilityScores::intelligence]	= 10;
	this->abilities[abilityScores::wisdom]			= 10;
	this->abilities[abilityScores::charisma]		= 10;
}

// Sets a specific score to a specific value
// Values are clamped with a range of 0 to 30, and prevents it from changing if the input is outside of that range
void playerCharacter::setScore(abilityScores ability, int score) {
	if (score < 0 || score > 30)
		score = this->abilities[ability];

	this->abilities[ability] = score;
}

// Gets a specified ability score
int playerCharacter::getScore(abilityScores ability) {
	return this->abilities[ability];
}

// Gets a specified ability score's modifier
int playerCharacter::getMod(abilityScores ability) {
	return (this->getScore(ability) - 10) / 2;
}

// This initializes every single skill with their respective ability score and no proficiency.
// It's fucked.
void playerCharacter::initSkills() {
	printf("initSkills() called");
	this->skillInfo[skills::athletics].mainAbility =		abilityScores::strength;
	this->skillInfo[skills::intimidation].mainAbility =		abilityScores::strength;

	this->skillInfo[skills::acrobatics].mainAbility =		abilityScores::dexterity;
	this->skillInfo[skills::sleightOfHand].mainAbility =	abilityScores::dexterity;
	this->skillInfo[skills::stealth].mainAbility =			abilityScores::dexterity;

	this->skillInfo[skills::arcana].mainAbility =			abilityScores::intelligence;
	this->skillInfo[skills::crafting].mainAbility =			abilityScores::intelligence;
	this->skillInfo[skills::history].mainAbility =			abilityScores::intelligence;
	this->skillInfo[skills::investigation].mainAbility =	abilityScores::intelligence;
	this->skillInfo[skills::medicine].mainAbility =			abilityScores::intelligence;
	this->skillInfo[skills::religion].mainAbility =			abilityScores::intelligence;

	this->skillInfo[skills::animalHandling].mainAbility =	abilityScores::wisdom;
	this->skillInfo[skills::insight].mainAbility =			abilityScores::wisdom;
	this->skillInfo[skills::nature].mainAbility =			abilityScores::wisdom;
	this->skillInfo[skills::perception].mainAbility =		abilityScores::wisdom;
	this->skillInfo[skills::survival].mainAbility =			abilityScores::wisdom;

	this->skillInfo[skills::deception].mainAbility =		abilityScores::charisma;
	this->skillInfo[skills::performance].mainAbility =		abilityScores::charisma;
	this->skillInfo[skills::persuasion].mainAbility =		abilityScores::charisma;

	for (auto& it : skillInfo) {
		this->skillInfo[it.first].profLevel = proficiencyLevels::noProficiency;
	}
	printf("initSkills() finished");
}

// Sets a skill's proficiency level
void playerCharacter::setSkillProficiency(skills skill, proficiencyLevels level) {
	this->skillInfo[skill].profLevel = level;
}

// Gets a skill's proficiency level
proficiencyLevels playerCharacter::getSkillProficiency(skills skill) {
	return this->skillInfo[skill].profLevel;
}

// Sets a skill's ability
void playerCharacter::setSkillAbility(skills skill, abilityScores ability) {
	this->skillInfo[skill].mainAbility = ability;
}

// Gets a skill's ability
abilityScores playerCharacter::getSkillAbility(skills skill) {
	return this->skillInfo[skill].mainAbility;
}

// Function to get a skill name based on it's enumerated value.
std::string skill::getSkillName(skills skill) {
	switch (skill) {
	case skills::acrobatics:
		return "Acrobatics";
	case skills::animalHandling:
		return "Animal Handling";
	case skills::arcana:
		return "Arcana";
	case skills::athletics:
		return "Athletics";
	case skills::crafting:
		return "Crafting";
	case skills::deception:
		return "Deception";
	case skills::history:
		return "History";
	case skills::insight:
		return "Inisght";
	case skills::intimidation:
		return "Intimidation";
	case skills::investigation:
		return "Investigation";
	case skills::medicine:
		return "Medicine";
	case skills::nature:
		return "Nature";
	case skills::perception:
		return "Perception";
	case skills::performance:
		return "Performance";
	case skills::persuasion:
		return "Persuasion";
	case skills::religion:
		return "Religion";
	case skills:: sleightOfHand:
		return "Sleight of Hand";
	case skills::stealth:
		return "Stealth";
	case skills::survival:
		return "Survival";
	default:
		return "Invalid";
	}
}

int playerCharacter::calcSkillProfBonus(skills s) {
	switch (this->getSkillProficiency(s)) {
	case proficiencyLevels::noProficiency:
		return 0;
	case proficiencyLevels::proficiency:
		return 2 + this->getLevel(levels::character);
	case proficiencyLevels::expertise:
		return 4 + this->getLevel(levels::character);
	case proficiencyLevels::mastery:
		return 6 + this->getLevel(levels::character);
	case proficiencyLevels::legendary:
		return 8 + this->getLevel(levels::character);
	default:
		return 0;
	}
}
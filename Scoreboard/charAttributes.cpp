#include <string>
#include "charAttributes.h"

// Sets the player name for a given character
void playerCharacter::setPName(std::string name) {
	this->playerName = name;
}

// Gets the player name for a given character
std::string playerCharacter::getPName() {
	return this->playerName;
}

// Sets the character name for a given character
void playerCharacter::setCName(std::string name) {
	this->charName = name;
}

// Gets the character name for a given character
std::string playerCharacter::getCName() {
	return this->charName;
}

// Initiatilizes every score to 7, as if creating a new character
void playerCharacter::initScores() {
	this->abilities[abilityScores::strength]		= 7;
	this->abilities[abilityScores::dexterity]		= 7;
	this->abilities[abilityScores::constitution]	= 7;
	this->abilities[abilityScores::intelligence]	= 7;
	this->abilities[abilityScores::wisdom]			= 7;
	this->abilities[abilityScores::charisma]		= 7;
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

int playerCharacter::calcProfBonus(skills s) {
	switch (this->getSkillProficiency(s)) {
	case proficiencyLevels::noProficiency:
		return 0;
	case proficiencyLevels::proficiency:
		return 2;
	case proficiencyLevels::expertise:
		return 4;
	case proficiencyLevels::mastery:
		return 6;
	case proficiencyLevels::legendary:
		return 8;
	default:
		return 0;
	}
}
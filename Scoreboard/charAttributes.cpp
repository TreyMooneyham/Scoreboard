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

// Gets a specific ability score
int playerCharacter::getScore(abilityScores ability) {
	return this->abilities[ability];
}

void playerCharacter::setSkillProficiency(skills skill, proficiencyLevels level) {
	this->skillInfo[skill].profLevel = level;
}

proficiencyLevels playerCharacter::getSkillProficiency(skills skill) {
	return this->skillInfo[skill].profLevel;
}

void playerCharacter::setSkillAbility(skills skill, abilityScores ability) {
	this->skillInfo[skill].mainAbility = ability;
}

abilityScores playerCharacter::getSkillAbility(skills skill) {
	return this->skillInfo[skill].mainAbility;
}

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
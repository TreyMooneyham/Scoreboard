#include <string>
#include <stdio.h>
#include "charAttributes.h"

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

// Sets the level for a particular section
void playerCharacter::setLevel(levels section, int level) {
	this->levelInfo[section] = level;
}

// Gets the level for a specific section
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

// Calculates the proficiency bonus for a skill
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

// Function to set the player's hit points
// -1 for all, 0 for rolled, 1 for temp, and 2 for current
void playerCharacter::setHP(int type, int hp) {
	switch (type) {
	case -1:
		this->hpInfo.rolledHP = hp;
		this->hpInfo.tempHP = hp;
		this->hpInfo.currentHP = hp;
		break;
	case 0:
		this->hpInfo.rolledHP = hp;
		break;
	case 1:
		this->hpInfo.tempHP = hp;
		break;
	case 2:
		this->hpInfo.currentHP = hp;
		break;
	default:
		return;
	}
}

// Function to set the player's hit points
// 0 for rolled, 1 for temp, and 2 for current
int playerCharacter::getHP(int type) {
	switch (type) {
	case 0:
		return this->hpInfo.rolledHP;
	case 1:
		return this->hpInfo.tempHP;
	case 2:
		return this->hpInfo.currentHP;
	default:
		return -1;
	}
}

// Initiatlize the hit points for the character
void playerCharacter::initHitPoints() {
	this->hpInfo.rolledHP = ((this->getLevel(levels::rogue) * 2 + this->getLevel(levels::spellcaster) * 2 + this->getLevel(levels::warrior) * 2) + (this->getLevel(levels::rogue) * 8 + this->getLevel(levels::spellcaster) * 6 + this->getLevel(levels::warrior) * 10)) / 2;
	this->hpInfo.tempHP = 0;
	this->hpInfo.currentHP = this->getHP(0) + (this->getLevel(levels::character) * this->getMod(abilityScores::constitution));
}

nlohmann::json playerCharacter::toJson() const {
	// Create the big json
	nlohmann::json jsonObj;

	// Put the char and player names into it
	jsonObj["charName"] = nameInfo.charName;
	jsonObj["playerName"] = nameInfo.playerName;

	// Put the relevant level information in
	nlohmann::json levelInfoJson;
	levelInfoJson["character"] = levelInfo.at(levels::character);
	jsonObj["levelInfo"] = levelInfoJson;

	// Put the relevant ability information in
	nlohmann::json abilitiesJson;
	abilitiesJson["strength"] = abilities.at(abilityScores::strength);
	abilitiesJson["dexterity"] = abilities.at(abilityScores::dexterity);
	abilitiesJson["constitution"] = abilities.at(abilityScores::constitution);
	abilitiesJson["intelligence"] = abilities.at(abilityScores::intelligence);
	abilitiesJson["wisdom"] = abilities.at(abilityScores::wisdom);
	abilitiesJson["charisma"] = abilities.at(abilityScores::charisma);
	jsonObj["abilities"] = abilitiesJson;

	// Put the relevant skills information in
	nlohmann::json skillsJson;
	skillsJson["acrobatics"]["profLevel"] = skillInfo.at(skills::acrobatics).profLevel;
	skillsJson["acrobatics"]["mainAbility"] = skillInfo.at(skills::acrobatics).mainAbility;

	skillsJson["animalHandling"]["profLevel"] = skillInfo.at(skills::animalHandling).profLevel;
	skillsJson["animalHandling"]["mainAbility"] = skillInfo.at(skills::animalHandling).mainAbility;

	skillsJson["arcana"]["profLevel"] = skillInfo.at(skills::arcana).profLevel;
	skillsJson["arcana"]["mainAbility"] = skillInfo.at(skills::arcana).mainAbility;

	skillsJson["athletics"]["profLevel"] = skillInfo.at(skills::athletics).profLevel;
	skillsJson["athletics"]["mainAbility"] = skillInfo.at(skills::athletics).mainAbility;

	skillsJson["crafting"]["profLevel"] = skillInfo.at(skills::crafting).profLevel;
	skillsJson["crafting"]["mainAbility"] = skillInfo.at(skills::crafting).mainAbility;

	skillsJson["deception"]["profLevel"] = skillInfo.at(skills::deception).profLevel;
	skillsJson["deception"]["mainAbility"] = skillInfo.at(skills::deception).mainAbility;

	skillsJson["history"]["profLevel"] = skillInfo.at(skills::history).profLevel;
	skillsJson["history"]["mainAbility"] = skillInfo.at(skills::history).mainAbility;

	skillsJson["insight"]["profLevel"] = skillInfo.at(skills::insight).profLevel;
	skillsJson["insight"]["mainAbility"] = skillInfo.at(skills::insight).mainAbility;

	skillsJson["intimidation"]["profLevel"] = skillInfo.at(skills::intimidation).profLevel;
	skillsJson["intimidation"]["mainAbility"] = skillInfo.at(skills::intimidation).mainAbility;

	skillsJson["investigation"]["profLevel"] = skillInfo.at(skills::investigation).profLevel;
	skillsJson["investigation"]["mainAbility"] = skillInfo.at(skills::investigation).mainAbility;

	skillsJson["medicine"]["profLevel"] = skillInfo.at(skills::medicine).profLevel;
	skillsJson["medicine"]["mainAbility"] = skillInfo.at(skills::medicine).mainAbility;

	skillsJson["nature"]["profLevel"] = skillInfo.at(skills::nature).profLevel;
	skillsJson["nature"]["mainAbility"] = skillInfo.at(skills::nature).mainAbility;

	skillsJson["perception"]["profLevel"] = skillInfo.at(skills::perception).profLevel;
	skillsJson["perception"]["mainAbility"] = skillInfo.at(skills::perception).mainAbility;

	skillsJson["performance"]["profLevel"] = skillInfo.at(skills::performance).profLevel;
	skillsJson["performance"]["mainAbility"] = skillInfo.at(skills::performance).mainAbility;

	skillsJson["persuasion"]["profLevel"] = skillInfo.at(skills::persuasion).profLevel;
	skillsJson["persuasion"]["mainAbility"] = skillInfo.at(skills::persuasion).mainAbility;

	skillsJson["religion"]["profLevel"] = skillInfo.at(skills::religion).profLevel;
	skillsJson["religion"]["mainAbility"] = skillInfo.at(skills::religion).mainAbility;

	skillsJson["sleightOfHand"]["profLevel"] = skillInfo.at(skills::sleightOfHand).profLevel;
	skillsJson["sleightOfHand"]["mainAbility"] = skillInfo.at(skills::sleightOfHand).mainAbility;

	skillsJson["stealth"]["profLevel"] = skillInfo.at(skills::stealth).profLevel;
	skillsJson["stealth"]["mainAbility"] = skillInfo.at(skills::stealth).mainAbility;

	skillsJson["survival"]["profLevel"] = skillInfo.at(skills::survival).profLevel;
	skillsJson["survival"]["mainAbility"] = skillInfo.at(skills::survival).mainAbility;

	jsonObj["skills"] = skillsJson;

	// Put the relevant hp information in
	nlohmann::json hpJson;
	hpJson["rolledHP"] = hpInfo.rolledHP;
	hpJson["tempHP"] = hpInfo.tempHP;
	hpJson["currentHP"] = hpInfo.currentHP;
	jsonObj["hp"] = hpJson;



	return jsonObj;
}
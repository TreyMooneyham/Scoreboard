// Includes
#include "charAttributes.h"
#include <vector>
#include <fstream>
#include <iostream>

bool feat::findFeat(int id, std::vector<int> list) {
	for (int i = 0; i < list.size(); i++) {
		if (list.at(i) == id) {
			return true;
		}
	}
	return false;
}

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

void playerCharacter::initLevels() {
	this->levelInfo[levels::character] = 0;
	this->levelInfo[levels::rogue] = 0;
	this->levelInfo[levels::spellcaster] = 0;
	this->levelInfo[levels::warrior] = 0;
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

int playerCharacter::getProfBonus(proficiencyLevels level) {
	switch (level) {
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

void playerCharacter::setDT(resistanceTypes type, int t) {
	this->resistInfo[type].dt = t;
}

void playerCharacter::setDR(resistanceTypes type, float r) {
	this->resistInfo[type].dr = r;
}

void playerCharacter::setDTR(resistanceTypes type, int t, float r) {
	this->resistInfo[type].dt = t;
	this->resistInfo[type].dr = r;
}

void playerCharacter::initResist() {
	for (int i = 0; i < 14; i++) {
		this->resistInfo[(resistanceTypes)i].dt = 0;
		this->resistInfo[(resistanceTypes)i].dr = 0.0f;
	}
}

int playerCharacter::getDT(resistanceTypes type) {
	return this->resistInfo[type].dt;
}

float playerCharacter::getDR(resistanceTypes type) {
	return this->resistInfo[type].dr;
}

bool playerCharacter::isMech(resistanceTypes type) {
	for (int i = 0; i < 5; i++) {
		if ((int)type == i)
			return true;
	}
	return false;
}

// Setter for ability adjustments
void playerCharacter::setAdj(abilityScores score, int val) {
	this->abilityAdj[score] = val;
}

// Setter for skill adjustments
void playerCharacter::setAdj(skills skill, int val) {
	this->skillAdj[skill] = val;
}

// Setter for movement adjustment
void playerCharacter::setAdj(movements type, int val) {
	this->movementAdj[type] = val;
}

// Setter for save adjustment
void playerCharacter::setAdj(savingThrows save, int val) {
	this->saveAdj[save] = val;
}

// "Setter" for adding an ability adjustment
// Lets you add penalties and bonuses simultaneously, multiple of either, or both.
void playerCharacter::addAdj(abilityScores score, int val) {
	this->abilityAdj[score] += val;
}

// "Setter" for adding a skill adjustment
// Lets you add penalties and bonuses simultaneously, multiple of either, or both.
void playerCharacter::addAdj(skills skill, int val) {
	this->skillAdj[skill] += val;
}

// "Setter" for adding a movement adjustment
// Lets you add penalties and bonuses simultaneously, multiple of either, or both.
void playerCharacter::addAdj(movements type, int val) {
	this->movementAdj[type] += val;
}

// "Setter" for adding a save adjustment
void playerCharacter::addAdj(savingThrows save, int val) {
	this->saveAdj[save] += val;
}

// Getter for ability adjustments
int playerCharacter::getAdj(abilityScores score) {
	return this->abilityAdj[score];
}

// Getter for skill adjustments
int playerCharacter::getAdj(skills skill) {
	return this->skillAdj[skill];
}

// Getter for movement adjustments
int playerCharacter::getAdj(movements type) {
	return this->movementAdj[type];
}

// Getter for saving throw adjustments
int playerCharacter::getAdj(savingThrows save) {
	return this->saveAdj[save];
}

// Initializer for the adjustments
void playerCharacter::initAdj() {
	// Initializes the six ability scores
	for (int i = 0; i < 6; i++) {
		this->abilityAdj[(abilityScores)i] = 0;
	}

	// Initializes the 19 skills
	for (int i = 0; i < 19; i++) {
		this->skillAdj[(skills)i] = 0;
	}

	// Initializes the five movements
	for (int i = 0; i < 5; i++) {
		this->movementAdj[(movements)i] = 0;
	}
}

// Setter for a condition
// Takes a value, 0 is off, most are 0 or 1
// some go higher.
void playerCharacter::setCondition(conditions cond, int val) {
	this->conditionInfo[cond] = val;
}

// Getter for a condition
// returns a value, 0 is off, most are 0 or 1
// some go higher.
int playerCharacter::getCondition(conditions cond) {
	return this->conditionInfo[cond];
}

// Initializer for all conditions
void playerCharacter::initConditions() {
	for (int i = 0; i < 30; i++) {
		this->conditionInfo[(conditions)i] = 0;
	}
}

// Setter for movement
void playerCharacter::setSpeed(movements type, int speed) {
	this->movementInfo[type] = speed;
}

// Getter for movement
int playerCharacter::getSpeed(movements type) {
	return this->movementInfo[type];
}

// Initializer for movement
void playerCharacter::initSpeeds() {
	this->movementInfo[movements::walking] = 30;
	this->movementInfo[movements::swimming] = 15;
	this->movementInfo[movements::climbing] = 15;
	this->movementInfo[movements::flying] = 0;
	this->movementInfo[movements::burrowing] = 0;
}

// Setter for saving throw's proficiency level
void playerCharacter::setSaveProficiency(savingThrows save, proficiencyLevels prof) {
	this->savesInfo[save].profLevel = prof;
}

// Getter for saving throw's proficiency level
proficiencyLevels playerCharacter::getSaveProficiency(savingThrows save) {
	return this->savesInfo[save].profLevel;
}

// Getter for saving throw's bonus
int playerCharacter::getSave(savingThrows save) {
	switch (save) {
	case savingThrows::fortitude:
		if (this->abilities[abilityScores::constitution] >= this->abilities[abilityScores::strength])
			return this->getMod(abilityScores::constitution);

		return this->getMod(abilityScores::strength);
	case savingThrows::reflex:
		if (this->abilities[abilityScores::dexterity] >= this->abilities[abilityScores::intelligence])
			return this->getMod(abilityScores::dexterity);

		return this->getMod(abilityScores::intelligence);
	case savingThrows::will:
		if (this->abilities[abilityScores::wisdom] >= this->abilities[abilityScores::charisma])
			return this->getMod(abilityScores::wisdom);

		return this->getMod(abilityScores::charisma);
	default:
		return 0;
	}
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
	for (const auto& pissman : this->skillInfo) {
		const skills& skillType = pissman.first;
		const skill& actualSkill = pissman.second;
		skillsJson[std::to_string(static_cast<int>(skillType))]["mainAbility"] = actualSkill.mainAbility;
		skillsJson[std::to_string(static_cast<int>(skillType))]["profLevel"] = actualSkill.profLevel;
	}
	jsonObj["skills"] = skillsJson;

	// Put the relevant hp information in
	nlohmann::json hpJson;
	hpJson["rolledHP"] = hpInfo.rolledHP;
	hpJson["tempHP"] = hpInfo.tempHP;
	hpJson["currentHP"] = hpInfo.currentHP;
	jsonObj["hp"] = hpJson;

	// Put the relevant feats information in
	nlohmann::json featsJson;
	featsJson["count"] = this->feats.size();
	for (int i = 0; i < this->feats.size(); ++i) {
		featsJson[std::to_string(i)] = this->feats.at(i);
	}
	jsonObj["feats"] = featsJson;

	// Put the relevant inventory information in
	nlohmann::json inventoryJson;
	inventoryJson["count"] = this->inventory.size();
	for (int i = 0; i < this->inventory.size(); ++i) {
		inventoryJson[std::to_string(i)]["id"] = this->inventory.at(i).id;
		inventoryJson[std::to_string(i)]["count"] = this->inventory.at(i).count;
	}
	jsonObj["inventory"] = inventoryJson;

	// Put the relevant resistance information in
	// NOTE: key = first, value = second
	nlohmann::json resistanceJson;
	for (const auto& pissboy : this->resistInfo) {
		const resistanceTypes& resType = pissboy.first;
		const resistance& res = pissboy.second;
		resistanceJson[std::to_string(static_cast<int>(resType))]["type"] = res.type;
		resistanceJson[std::to_string(static_cast<int>(resType))]["dt"] = res.dt;
		resistanceJson[std::to_string(static_cast<int>(resType))]["dr"] = res.dr;
	}
	jsonObj["resistances"] = resistanceJson;

	// Put the relevant condition information in
	nlohmann::json conditionsJson;
	for (const auto& condition : this->conditionInfo) {
		const conditions& conditionName = condition.first;
		int afflictedLevel = condition.second;
		conditionsJson[std::to_string(static_cast<int>(conditionName))]["level"] = afflictedLevel;
	}
	jsonObj["conditions"] = conditionsJson;

	// Put the relevant saves information in
	nlohmann::json savesJson;
	for (const auto& save : this->savesInfo) {
		const savingThrows& saveType = save.first;
		const savingThrow& saveStats = save.second;
		savesJson[std::to_string(static_cast<int>(saveType))]["mainAbility"] = saveStats.mainAbility;
		savesJson[std::to_string(static_cast<int>(saveType))]["profLevel"] = saveStats.profLevel;
	}
	jsonObj["saves"] = savesJson;

	// Put the relevant movement information in
	nlohmann::json movementJson;
	for (const auto& movement : this->movementInfo) {
		const movements& movementType = movement.first;
		int speed = movement.second;
		movementJson[std::to_string(static_cast<int>(movementType))]["speed"] = speed;
	}
	jsonObj["movement"] = movementJson;

	// Rapid fire for the adjustments
	nlohmann::json abilityAdjJson;
	for (const auto& adjustment : this->abilityAdj) {
		const abilityScores& adjustmentType = adjustment.first;
		abilityAdjJson[std::to_string(static_cast<int>(adjustmentType))]["adjustment"] = adjustment.second;
	}
	jsonObj["abilityAdjustments"] = abilityAdjJson;

	nlohmann::json skillAdjJson;
	for (const auto& adjustment : this->skillAdj) {
		const skills& adjustmentType = adjustment.first;
		skillAdjJson[std::to_string(static_cast<int>(adjustmentType))]["adjustment"] = adjustment.second;
	}
	jsonObj["skillAdjustments"] = skillAdjJson;

	nlohmann::json movementAdjJson;
	for (const auto& adjustment : this->movementAdj) {
		const movements& adjustmentType = adjustment.first;
		movementAdjJson[std::to_string(static_cast<int>(adjustmentType))]["adjustment"] = adjustment.second;
	}
	jsonObj["movementAdjustments"] = movementAdjJson;

	nlohmann::json saveAdjJson;
	for (const auto& adjustment : this->saveAdj) {
		const savingThrows& adjustmentType = adjustment.first;
		saveAdjJson[std::to_string(static_cast<int>(adjustmentType))]["adjustment"] = adjustment.second;
	}
	jsonObj["saveAdjustments"] = saveAdjJson;


	return jsonObj;
}

std::vector<feat> initFeats() {
	std::vector<feat> v;
	std::string filePath = "featsExample.json";
	try {
		std::ifstream inputFile(filePath);
		// Load all this shit into the json
		nlohmann::json jsonObj;
		inputFile >> jsonObj;
		inputFile.close();

		for (auto& elem : jsonObj.items()) {
			feat currFeat;
			currFeat.name = elem.key();
			currFeat.id = elem.value().at("id").get<int>();
			currFeat.minLevel = elem.value().at("minLevel").get<int>();
			currFeat.hitDie = elem.value().at("hitDie").get<int>();
			currFeat.description = elem.value().at("desciption").get<std::string>();
			currFeat.minScore = elem.value().at("minScore").get<int>();
			auto parseSkill = [&](int num) {
				switch (num)
				{
				case 0:
					return abilityScores::strength;
					break;
				case 1:
					return abilityScores::dexterity;
					break;
				case 2:
					return abilityScores::constitution;
					break;
				case 3:
					return abilityScores::intelligence;
					break;
				case 4:
					return abilityScores::wisdom;
					break;
				case 5:
					return abilityScores::charisma;
					break;
				default:
					break;
				}
				};

			if (elem.value().at("minScoreAbility").is_array()) {
				std::vector<int> minScores = elem.value().at("minScoreAbility").get<std::vector<int>>();
				for (int i = 0; i < minScores.size(); ++i) {
					currFeat.minScoreAbility.push_back(parseSkill(minScores.at(i)));
				}
			}
			else {
				currFeat.minScoreAbility.push_back(parseSkill(elem.value().at("minScoreAbility").get<int>()));
			}

			currFeat.prerequisiteFeats = elem.value().at("prerequisiteFeats").get<std::vector<int>>();
			currFeat.restrictedFeats = elem.value().at("restrictedFeats").get<std::vector<int>>();
			v.push_back(currFeat);
		}
	}
	catch(const std::exception& e){
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception caught" << std::endl;
	}
	return v;
}

std::vector<item> initItems() {
	std::vector<item> v;
	std::string filePath = "inventoryExample.json";
	try {
		std::ifstream inputFile(filePath);

		nlohmann::json jsonObj;
		inputFile >> jsonObj;
		inputFile.close();
		for (auto& elem : jsonObj.items()) {
			item currItem;
			currItem.name = elem.key();
			currItem.id = elem.value().at("id").get<int>();
			currItem.cost = elem.value().at("cost").get<double>();
			currItem.weight = elem.value().at("weight").get<double>();
			currItem.description = elem.value().at("desciption").get<std::string>();
			currItem.count = elem.value().at("count").get<int>();
			v.push_back(currItem);
		}
	}
	catch(const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch(...){
		std::cerr << "Unknown exception caught" << std::endl;
	}
	return v;
}
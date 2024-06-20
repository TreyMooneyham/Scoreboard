// Includes
#include "settings.h"

namespace Settings {
	void newCharacter(playerCharacter& pc) {
        pc.setName("New Character", 1);
        pc.setLevel(levels::character, 1);

        pc.initScores();
        pc.initHitPoints();
        pc.initSkills();
        pc.initResist();
        pc.initAdj();
        pc.initConditions();
        pc.initSpeeds();
	}

	void saveCharacter(playerCharacter pc) {
		nlohmann::json jsonObj = pc.toJson();
		std::string filename = pc.getName(1) + ".json";
        if (filename == ".json") {
            std::cerr << "No filename. Could not be saved." << std::endl;
            return;
        }

		std::replace(filename.begin(), filename.end(), ' ', '_');
		std::ofstream file (filename);
		if (file.is_open()) {
			file << jsonObj.dump(4);  // Pretty print with 4 spaces indentation
			file.close();
			std::cout << "Object saved to " << filename << std::endl;
		}
		else {
			std::cerr << "Could not open file for writing: " << filename << std::endl;
		}
	}

	void saveAsCharacter(playerCharacter pc, std::string filename) {
        if (filename == "")
            std::cerr << "No filename. Could not be saved." << std::endl;

		nlohmann::json jsonObj = pc.toJson();
		filename += ".json";
		std::ofstream file(filename);
		if (file.is_open()) {
			file << jsonObj.dump(4);  // Pretty print with 4 spaces indentation
			file.close();
			std::cout << "Object saved to " << filename << std::endl;
		}
		else {
			std::cerr << "Could not open file for writing: " << filename << std::endl;
		}
	}

	void loadCharacter(playerCharacter& pc, std::string filePath) {
        // Place holder for testing
        // std::string filePath = "fuckshit.json";
        try {
            std::ifstream inputFile(filePath + ".json");

            // Load all this shit into the json
            nlohmann::json jsonObj;
            inputFile >> jsonObj;
            inputFile.close();

            // Parsing name information
            pc.nameInfo.charName = jsonObj.at("charName").get<std::string>();
            pc.nameInfo.playerName = jsonObj.at("playerName").get<std::string>();

            // Parsing level information
            pc.levelInfo[levels::character] = jsonObj.at("levelInfo").at("character").get<int>();

            // Parsing abilities
            pc.abilities[abilityScores::strength] = jsonObj.at("abilities").at("strength").get<int>();
            pc.abilities[abilityScores::dexterity] = jsonObj.at("abilities").at("dexterity").get<int>();
            pc.abilities[abilityScores::constitution] = jsonObj.at("abilities").at("constitution").get<int>();
            pc.abilities[abilityScores::intelligence] = jsonObj.at("abilities").at("intelligence").get<int>();
            pc.abilities[abilityScores::wisdom] = jsonObj.at("abilities").at("wisdom").get<int>();
            pc.abilities[abilityScores::charisma] = jsonObj.at("abilities").at("charisma").get<int>();

            // Parsing skills
            for (auto& pissman : jsonObj["skills"].items()) {
                skills skillsKey = (skills)(std::stoi(pissman.key()));
                skill skillStats;
                skillStats.mainAbility = pissman.value()["mainAbility"];
                skillStats.profLevel = pissman.value()["profLevel"];
                pc.skillInfo[skillsKey] = skillStats;
            }

            // Parsing HP information
            pc.hpInfo.rolledHP = jsonObj.at("hp").at("rolledHP").get<int>();
            pc.hpInfo.tempHP = jsonObj.at("hp").at("tempHP").get<int>();
            pc.hpInfo.currentHP = jsonObj.at("hp").at("currentHP").get<int>();

            // Parsing feat information
            int size = jsonObj.at("feats").at("count").get<int>();
            for (int i = 0; i < size; i++) {
                pc.feats.push_back(jsonObj.at("feats").at(std::to_string(i)).get<int>());
            }

            // Parsing inventory information
            size = jsonObj.at("inventory").at("count").get<int>();
            for (int i = 0; i < size; i++) {
                item theRatsAreBackInMySkinHelpMeGetThemOutIAmBeggingYouPlease;
                theRatsAreBackInMySkinHelpMeGetThemOutIAmBeggingYouPlease.id = jsonObj.at("inventory").at(std::to_string(i)).at("id").get<int>();
                theRatsAreBackInMySkinHelpMeGetThemOutIAmBeggingYouPlease.count = jsonObj.at("inventory").at(std::to_string(i)).at("count").get<int>();
                pc.inventory.push_back(theRatsAreBackInMySkinHelpMeGetThemOutIAmBeggingYouPlease);
            }

            // Parsing resistance information
            for (auto& res : jsonObj["resistances"].items()) {
                resistanceTypes resistanceKey = (resistanceTypes) (std::stoi(res.key()));
                resistance resistanceValue;
                resistanceValue.dr = res.value()["dr"].get<double>();
                resistanceValue.dt = res.value()["dt"].get<double>();
                resistanceValue.type = (resistanceTypes) res.value()["type"].get<int>();
                pc.resistInfo[resistanceKey] = resistanceValue;
            }

            // Parsing condition information
            for (auto& condtion : jsonObj["conditions"].items()) {
                conditions conditionKey = (conditions)(std::stoi(condtion.key()));
                pc.conditionInfo[conditionKey] = condtion.value()["level"];
            }

            // Parsing saves information
            for (auto& save : jsonObj["saves"].items()) {
                savingThrows savesKey = (savingThrows)(std::stoi(save.key()));
                savingThrow saveStats;
                saveStats.mainAbility = save.value()["mainAbility"];
                saveStats.profLevel = save.value()["profLevel"];
                pc.savesInfo[savesKey] = saveStats;
            }

            // Parsing speed information
            for (auto& movement : jsonObj["movement"].items()) {
                movements movementType = (movements)(std::stoi(movement.key()));
                pc.movementInfo[movementType] = movement.value()["speed"];
            }

            // Adjustments below here
            for (auto& adjustment : jsonObj["abilityAdjustments"].items()) {
                abilityScores adjustmentType = (abilityScores)(std::stoi(adjustment.key()));
                pc.abilityAdj[adjustmentType] = adjustment.value()["adjustment"];
            }

            for (auto& adjustment : jsonObj["skillAdjustments"].items()) {
                skills adjustmentType = (skills)(std::stoi(adjustment.key()));
                pc.skillAdj[adjustmentType] = adjustment.value()["adjustment"];
            }

            for (auto& adjustment : jsonObj["movementAdjustments"].items()) {
                movements adjustmentType = (movements)(std::stoi(adjustment.key()));
                pc.movementAdj[adjustmentType] = adjustment.value()["adjustment"];
            }

            for (auto& adjustment : jsonObj["saveAdjustments"].items()) {
                savingThrows adjustmentType = (savingThrows)(std::stoi(adjustment.key()));
                pc.saveAdj[adjustmentType] = adjustment.value()["adjustment"];
            }

        }
        catch (...) {
            std::cerr << "Character not found/unreachable" << std::endl;
        }
	}
}
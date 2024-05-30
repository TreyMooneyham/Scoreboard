// Includes
#include "settings.h"

namespace Settings {
	void newCharacter(playerCharacter& pc) {
        pc.setName("New Character", -1);
        pc.setLevel(levels::character, 1);

        pc.initScores();
        pc.initHitPoints();
        pc.initSkills();
	}

	void saveCharacter(playerCharacter pc) {
		nlohmann::json jsonObj = pc.toJson();
		std::string filename = pc.getName(1) + ".json";
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

	void saveAsCharacter(playerCharacter pc) {
		nlohmann::json jsonObj = pc.toJson();
		std::string filename;
		std::cout << "Enter a name for this file: ";
		std::getline(std::cin, filename);
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

	void loadCharacter(playerCharacter& pc) {
        // Place holder for testing
        std::string filePath = "fuckshit.json";
        std::ifstream inputFile(filePath);
        std::cout << "hello" << std::endl;
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
        auto parseSkill = [&](const std::string& skillName, skills skillEnum) {
            switch (jsonObj.at("skills").at(skillName).at("profLevel").get<int>())
            {
            case 0:
                pc.skillInfo[skillEnum].profLevel = proficiencyLevels::noProficiency;
                break;
            case 1:
                pc.skillInfo[skillEnum].profLevel = proficiencyLevels::proficiency;
                break;
            case 2:
                pc.skillInfo[skillEnum].profLevel = proficiencyLevels::expertise;
                break;
            case 3:
                pc.skillInfo[skillEnum].profLevel = proficiencyLevels::mastery;
                break;
            case 4:
                pc.skillInfo[skillEnum].profLevel = proficiencyLevels::legendary;
                break;
            default:
                break;
            }
            
            switch (jsonObj.at("skills").at(skillName).at("mainAbility").get<int>())
            {
            case 0:
                pc.skillInfo[skillEnum].mainAbility = abilityScores::strength;
                break;
            case 1:
                pc.skillInfo[skillEnum].mainAbility = abilityScores::dexterity;
                break;
            case 2:
                pc.skillInfo[skillEnum].mainAbility = abilityScores::constitution;
                break;
            case 3:
                pc.skillInfo[skillEnum].mainAbility = abilityScores::intelligence;
                break;
            case 4:
                pc.skillInfo[skillEnum].mainAbility = abilityScores::wisdom;
                break;
            case 5:
                pc.skillInfo[skillEnum].mainAbility = abilityScores::charisma;
                break;
            default:
                break;
            }
        };

        parseSkill("acrobatics", skills::acrobatics);
        parseSkill("animalHandling", skills::animalHandling);
        parseSkill("arcana", skills::arcana);
        parseSkill("athletics", skills::athletics);
        parseSkill("crafting", skills::crafting);
        parseSkill("deception", skills::deception);
        parseSkill("history", skills::history);
        parseSkill("insight", skills::insight);
        parseSkill("intimidation", skills::intimidation);
        parseSkill("investigation", skills::investigation);
        parseSkill("medicine", skills::medicine);
        parseSkill("nature", skills::nature);
        parseSkill("perception", skills::perception);
        parseSkill("performance", skills::performance);
        parseSkill("persuasion", skills::persuasion);
        parseSkill("religion", skills::religion);
        parseSkill("sleightOfHand", skills::sleightOfHand);
        parseSkill("stealth", skills::stealth);
        parseSkill("survival", skills::survival);

        // Parsing HP information
        pc.hpInfo.rolledHP = jsonObj.at("hp").at("rolledHP").get<int>();
        pc.hpInfo.tempHP = jsonObj.at("hp").at("tempHP").get<int>();
        pc.hpInfo.currentHP = jsonObj.at("hp").at("currentHP").get<int>();
	}
}
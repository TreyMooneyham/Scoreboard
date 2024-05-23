#include "charSheet.h"
#include <iostream>

bool contains(const std::string& word, const std::string& sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string toLower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), (int (*)(int))std::tolower);

	return str;
}

std::string modFormat(std::string str, int bonus) {
	str += ":";

	int diff = 22 - str.length();
	if (bonus > 9)
		diff--;

	for (int i = 0; i < diff; i++) {
		str += " ";
	}

	(bonus < 0) ? str += std::to_string(bonus) : str += "+" + std::to_string(bonus);

	return str;
}

std::string modFormat(const char* charray, int bonus) {
	std::string str;
	str += charray;

	str = modFormat(str, bonus);

	return str;
}

// Here's the global character variable
extern playerCharacter globalChar = playerCharacter();

static skill idk;
static skills currentSkill = skills::acrobatics;
static proficiencyLevels currentProficiencyLevel = proficiencyLevels::noProficiency;

// Special things for ImGui to use
const char* proficiencyLevelsList[] = { "No Proficiency", "Proficiency", "Expertise", "Mastery", "Legendary" }; // List of proficiency levels
const char* mainAbilityList[] = { "Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma" }; // List of ability scores
const char* savesList[] = { "Fortitude", "Reflex", "Will" }; // List of saving throws
const char* levelsList[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
							 "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", 
							 "Seventeen", "Eighteen", "Nineteen", "Twenty" }; // May God forgive me for my sins...
static char filterSkills[32];

void charSheet(bool* enable) {
	// Common variables for the global character
	int strScore = globalChar.getScore(abilityScores::strength);
	int strMod = globalChar.getMod(abilityScores::strength);
	int conScore = globalChar.getScore(abilityScores::constitution);
	int conMod = globalChar.getMod(abilityScores::constitution);
	int dexScore = globalChar.getScore(abilityScores::dexterity);
	int dexMod = globalChar.getMod(abilityScores::dexterity);
	int intScore = globalChar.getScore(abilityScores::intelligence);
	int intMod = globalChar.getMod(abilityScores::intelligence);
	int wisScore = globalChar.getScore(abilityScores::wisdom);
	int wisMod = globalChar.getMod(abilityScores::wisdom);
	int chaScore = globalChar.getScore(abilityScores::charisma);
	int chaMod = globalChar.getMod(abilityScores::charisma);

	int skillScoreBonus = globalChar.getMod(globalChar.skillInfo[currentSkill].mainAbility);
	int profBonus = globalChar.calcSkillProfBonus(currentSkill);
	int totalBonus = skillScoreBonus + profBonus;

	// Actual menu starts here
	ImGui::SetNextWindowSize(ImVec2(800, 800));
	if (ImGui::Begin("Character Sheet", enable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::BeginChild("CharInfoChild", ImVec2(-1, 56), ImGuiChildFlags_Border)) {
			if (ImGui::BeginTable("CharInfoTable", 4, ImGuiTableFlags_BordersInnerV)) {
				ImGui::TableNextColumn();
				{
					ImGui::Text("Player:"); ImGui::SameLine();
					ImGui::Text(globalChar.getName(0).c_str());

					ImGui::Text("Character:"); ImGui::SameLine();
					ImGui::Text(globalChar.getName(1).c_str());
				}
				ImGui::TableNextColumn();
				{
					ImGui::Text("Heritage:"); ImGui::SameLine();
					ImGui::Text("PLACEHOLDER");

					ImGui::PushItemWidth(-1);
					std::string lvlLabel = "Level: ";
					lvlLabel += std::to_string(globalChar.getLevel(levels::character));
					if (ImGui::BeginCombo("##LevelCombo", lvlLabel.c_str())) {
						for (int i = 0; i < IM_ARRAYSIZE(levelsList); i++) {
							bool selectedLevel = (globalChar.levelInfo[levels::character] == i);

							std::string lvl = levelsList[i];
							if (ImGui::Selectable(lvl.c_str(), selectedLevel)) {
								globalChar.setLevel(levels::character, i + 1);
							}
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
				}
				ImGui::TableNextColumn();
				{
					
				}
				ImGui::TableNextColumn();
				{

				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}

		if (ImGui::BeginChild("##AbilityScores", ImVec2(190, 170), ImGuiChildFlags_Border)) {
				for (int i = 0; i < IM_ARRAYSIZE(mainAbilityList); i++) {
					std::string formattedAbility = mainAbilityList[i];

					ImGui::Text(modFormat(formattedAbility, globalChar.getMod((abilityScores)i)).c_str());
				}

				ImGui::Separator();

				for (int i = 0; i < IM_ARRAYSIZE(savesList); i++) {
					std::string formattedSave = savesList[i];

					switch (i) {
					case 0:
						(strMod >= conMod) ? ImGui::Text(modFormat(formattedSave, strMod).c_str()) : ImGui::Text(modFormat(formattedSave, conMod).c_str());
						break;
					case 1:
						(dexMod >= intMod) ? ImGui::Text(modFormat(formattedSave, dexMod).c_str()) : ImGui::Text(modFormat(formattedSave, intMod).c_str());
						break;
					case 2:
						(wisMod >= chaMod) ? ImGui::Text(modFormat(formattedSave, wisMod).c_str()) : ImGui::Text(modFormat(formattedSave, chaMod).c_str());
						break;
					}
				}
				ImGui::EndChild();
			}

		ImGui::SameLine();

		if (ImGui::BeginChild("TestZone", ImVec2(-1, 170), ImGuiChildFlags_Border)) {

			ImGui::EndChild();
		}

		if (ImGui::BeginChild("##SkillsOutput", ImVec2(190, -1), ImGuiChildFlags_Border)) {
				
			ImGui::Text(modFormat("Ability Bonus", skillScoreBonus).c_str());
			ImGui::Text(modFormat("Proficiency Bonus", profBonus).c_str());
			ImGui::Text(modFormat("Misc. Bonus", 0).c_str()); // TODO: Create the logic for this

			ImGui::Separator();

			ImGui::Text(modFormat("Total Bonus", totalBonus).c_str());
			ImGui::Text("Passive DC:           %i", totalBonus + 10); // Not really worth it to create a function to format this one.

			ImGui::Separator();

			ImGui::PushItemWidth(-1);
			ImGui::InputTextWithHint("##FilterSkills", "Filter Skills...", filterSkills, IM_ARRAYSIZE(filterSkills));

			std::string mainAbilityText = mainAbilityList[(int)globalChar.skillInfo[currentSkill].mainAbility];
			if (ImGui::BeginCombo("##mainAbility", mainAbilityText.c_str())) {
				for (int n = 0; n < IM_ARRAYSIZE(mainAbilityList); n++) {
					const bool selectedAbility = (globalChar.skillInfo[currentSkill].mainAbility == (abilityScores)n);

					std::string mainAbilityName = mainAbilityList[n];
					if (ImGui::Selectable(mainAbilityName.c_str(), selectedAbility)) {
						globalChar.skillInfo[currentSkill].mainAbility = (abilityScores)n;
					}
				}

				ImGui::EndCombo();
			}

			// Add this back in the level up menu, whenever we get around to that.
			std::string profText = proficiencyLevelsList[(int)currentProficiencyLevel];
			if (ImGui::BeginCombo("##ProfLevels", profText.c_str())) {
				for (int n = 0; n < IM_ARRAYSIZE(proficiencyLevelsList); n++) {
					const bool selectedProf = (currentProficiencyLevel == (proficiencyLevels)n);

					std::string profName = proficiencyLevelsList[n];
					if (ImGui::Selectable(profName.c_str(), selectedProf)) {
						currentProficiencyLevel = (proficiencyLevels)n;
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Apply Proficiency", ImVec2(-1, 0))) {
				globalChar.setSkillProficiency(currentSkill, currentProficiencyLevel);
			}

			ImGui::PopItemWidth();

			if (ImGui::BeginListBox("##SkillsList", ImVec2(-1, -1))) {
				for (auto& it : globalChar.skillInfo) {
					if (!contains(toLower(filterSkills), toLower(idk.getSkillName(it.first).c_str())))
						continue;

					const bool selectedSkill = ((int)it.first == (int)currentSkill);
					ImGui::PushID((int)it.first);
					std::string skillName = idk.getSkillName(it.first).c_str();

					if (ImGui::Selectable(skillName.c_str(), selectedSkill))
						currentSkill = it.first;

					ImGui::PopID();
				}
				ImGui::EndListBox();
			}
			ImGui::EndChild();
		}

		ImGui::SameLine();

		if (ImGui::BeginChild("##Col2Child1", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
			ImGui::Text("2nd Column");

			ImGui::EndChild();
		}

		/*ImGui::TableNextColumn();

		if (ImGui::BeginChild("##Col3Child1", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
			ImGui::Text("3rd Column");

			ImGui::EndChild();
		}

		ImGui::TableNextColumn();

		if (ImGui::BeginChild("##Col4Child1", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
			ImGui::Text("4th Column");

			ImGui::EndChild();
		}*/
	}
	ImGui::End();
}
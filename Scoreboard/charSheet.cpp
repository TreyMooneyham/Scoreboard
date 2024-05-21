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

// Here's the global character variable
extern playerCharacter globalChar = playerCharacter();

static skill idk;
static skills currentSkill = skills::acrobatics;
static proficiencyLevels currentProficiencyLevel = proficiencyLevels::noProficiency;

// Special things for ImGui to use
const char* proficiencyLevelsList[] = { "No Proficiency", "Proficiency", "Expertise", "Mastery", "Legendary" }; // List of proficiency levels
const char* mainAbilityList[] = { "Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma" }; // List of ability scores
static char filterSkills[32];

void charSheet(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(800, 800));
	if (ImGui::Begin("Character Sheet", enable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::BeginTable("##CharacterSheetTable", 4)) {
			ImGui::TableNextColumn();

			if (ImGui::BeginChild("CharInfoChild", ImVec2(-1, 60), ImGuiChildFlags_Border)) {

				ImGui::Text("Fuck");

				ImGui::EndChild();
			}

			if (ImGui::BeginChild("##AbilityScores", ImVec2(190, 170), ImGuiChildFlags_Border)) {
				ImGui::Text("Strength:       %i, (%i)", globalChar.getScore(abilityScores::strength), globalChar.getMod(abilityScores::strength));
				ImGui::Text("Dexterity:      %i, (%i)", globalChar.getScore(abilityScores::dexterity), globalChar.getMod(abilityScores::dexterity));
				ImGui::Text("Constitution:   %i, (%i)", globalChar.getScore(abilityScores::constitution), globalChar.getMod(abilityScores::constitution));
				ImGui::Text("Intelligence:   %i, (%i)", globalChar.getScore(abilityScores::intelligence), globalChar.getMod(abilityScores::intelligence));
				ImGui::Text("Wisdom:         %i, (%i)", globalChar.getScore(abilityScores::wisdom), globalChar.getMod(abilityScores::wisdom));
				ImGui::Text("Charisma:       %i, (%i)", globalChar.getScore(abilityScores::charisma), globalChar.getMod(abilityScores::charisma));

				ImGui::Separator();

				if (globalChar.getScore(abilityScores::strength) > globalChar.getScore(abilityScores::constitution))
					ImGui::Text("Fortitude Save:      %i", globalChar.getMod(abilityScores::strength));
				else
					ImGui::Text("Fortitude Save:      %i", globalChar.getMod(abilityScores::constitution));

				if (globalChar.getScore(abilityScores::dexterity) > globalChar.getScore(abilityScores::intelligence))
					ImGui::Text("Reflex Save:         %i", globalChar.getMod(abilityScores::dexterity));
				else
					ImGui::Text("Reflex Save:         %i", globalChar.getMod(abilityScores::intelligence));

				if (globalChar.getScore(abilityScores::wisdom) > globalChar.getScore(abilityScores::charisma))
					ImGui::Text("Will Save:           %i", globalChar.getMod(abilityScores::wisdom));
				else
					ImGui::Text("Will Save:           %i", globalChar.getMod(abilityScores::charisma));

				ImGui::EndChild();
			}

			if (ImGui::BeginChild("##SkillsOutput", ImVec2(190, -1), ImGuiChildFlags_Border)) {
				// Add this back in the level up menu, whenever we get around to that.
				//std::string profText = proficiencyLevelsList[(int)currentProficiencyLevel];
				//if (ImGui::BeginCombo("##ProfLevels", profText.c_str())) {
				//	for (int n = 0; n < IM_ARRAYSIZE(proficiencyLevelsList); n++) {
				//		const bool selectedProf = (currentProficiencyLevel == (proficiencyLevels)n);

				//		std::string profName = proficiencyLevelsList[n];
				//		if (ImGui::Selectable(profName.c_str(), selectedProf)) {
				//			currentProficiencyLevel = (proficiencyLevels)n;
				//		}
				//	}

				//	ImGui::EndCombo();
				//}

				//if (ImGui::Button("Apply Proficiency", ImVec2(-1, 0))) {
				//	globalChar.setSkillProficiency(currentSkill, currentProficiencyLevel);
				//}

				int skillScoreBonus = globalChar.getMod(globalChar.skillInfo[currentSkill].mainAbility);
				int profBonus = globalChar.calcSkillProfBonus(currentSkill);
				int totalBonus = skillScoreBonus + profBonus;
				ImGui::Text("Ability Bonus: %i", skillScoreBonus);
				ImGui::Text("Proficiency Bonus: %i", profBonus);
				ImGui::Text("Misc. Bonus: WIP");	// TODO: Create the math for this...

				ImGui::Separator();

				ImGui::Text("Total Bonus: %i", totalBonus);

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

			ImGui::TableNextColumn();

			if (ImGui::BeginChild("##Col2", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
				ImGui::Text("2nd Column");

				ImGui::EndChild();
			}

			ImGui::TableNextColumn();

			if (ImGui::BeginChild("##Col3", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
				ImGui::Text("3rd Column");

				ImGui::EndChild();
			}

			ImGui::TableNextColumn();

			if (ImGui::BeginChild("##Col4", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
				ImGui::Text("4th Column");

				ImGui::EndChild();
			}

			ImGui::EndTable();
		}
	}
	ImGui::End();
}
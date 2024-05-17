#include "charSheet.h"
#include <iostream>

bool Contains(const std::string& word, const std::string& sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string ToLower(std::string str) {
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
static char filterSkills[32];

void charSheet(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(600, 800));
	if (!ImGui::Begin("Character Sheet", enable), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar) {
		ImGui::BeginChild("##AbilityScores", ImVec2(-1, 60), ImGuiChildFlags_Border); 
		{
			ImGui::Columns(6, "##AbilityScoreColumns", false);
			{
				ImGui::Text("Strength");
				ImGui::Text("%d", globalChar.getScore(abilityScores::strength));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Dexterity");
				ImGui::Text("%d", globalChar.getScore(abilityScores::dexterity));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Constitution");
				ImGui::Text("%d", globalChar.getScore(abilityScores::constitution));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Intelligence");
				ImGui::Text("%d", globalChar.getScore(abilityScores::intelligence));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Wisdom");
				ImGui::Text("%d", globalChar.getScore(abilityScores::wisdom));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Charisma");
				ImGui::Text("%d", globalChar.getScore(abilityScores::charisma));
			}
		}
		ImGui::EndChild();

		ImGui::Columns(3, "##SkillsColumn", false);
		{
			ImGui::SetColumnOffset(1, 200);
			ImGui::BeginChild("##SkillsOutput", ImVec2(-1, 150), ImGuiChildFlags_Border);
			{
				ImGui::PushItemWidth(-1);
				std::string previewText = proficiencyLevelsList[(int)currentProficiencyLevel];
				if (ImGui::BeginCombo("##ProfLevels", previewText.c_str())) {
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

				int skillScoreBonus = globalChar.getMod(globalChar.skillInfo[currentSkill].mainAbility);
				int profBonus = globalChar.calcSkillProfBonus(currentSkill);
				int totalBonus = skillScoreBonus + profBonus;
				ImGui::Text("Ability Bonus: %i", skillScoreBonus);
				ImGui::Text("Proficiency Bonus: %i", profBonus);
				ImGui::Text("Total Bonus: %i", totalBonus);
			}
			ImGui::EndChild();
			ImGui::PushItemWidth(-1);
			ImGui::InputTextWithHint("##FilterSkills", "Filter Skills...", filterSkills, IM_ARRAYSIZE(filterSkills));
			ImGui::PopItemWidth();

			if (ImGui::BeginListBox("##SkillsList", ImVec2(-1, -1))) {
				for (auto& it : globalChar.skillInfo) {
					if (!Contains(ToLower(filterSkills), ToLower(idk.getSkillName(it.first).c_str())))
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
		}
	}
	ImGui::End();
}
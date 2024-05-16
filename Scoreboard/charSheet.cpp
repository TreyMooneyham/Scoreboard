#include "charSheet.h"
#include <iostream>

void charSheet(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(600, 800));
	playerCharacter testChar;
	testChar.initScores();
	testChar.setScore(abilityScores::strength, 14);
	if (!ImGui::Begin("Character Sheet", enable), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar) {
		ImGui::BeginChild("##AbilityScores", ImVec2(-1, 60), ImGuiChildFlags_Border); 
		{
			ImGui::Columns(6, "##AbilityScoreColumns", false);
			{
				ImGui::Text("Strength");
				ImGui::Text("%d", testChar.getScore(abilityScores::strength));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Dexterity");
				ImGui::Text("%d", testChar.getScore(abilityScores::dexterity));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Constitution");
				ImGui::Text("%d", testChar.getScore(abilityScores::constitution));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Intelligence");
				ImGui::Text("%d", testChar.getScore(abilityScores::intelligence));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Wisdom");
				ImGui::Text("%d", testChar.getScore(abilityScores::wisdom));
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Charisma");
				ImGui::Text("%d", testChar.getScore(abilityScores::charisma));
			}
		}
		ImGui::EndChild();

		static char filterSkills[19];
		ImGui::Columns(3, "##SkillsColumn", false);
		{
			ImGui::SetColumnOffset(1, 200);
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##FilterSkills", filterSkills, IM_ARRAYSIZE(filterSkills));
			ImGui::PopItemWidth();

			static skills currentSkill = skills::acrobatics;

			ImGui::BeginListBox("##SkillsList", ImVec2(-1, -1));
			// Possibly expensive. 
			// TODO: Look into the differences between auto &it vs auto it.
			for (auto &it : testChar.skillInfo) {
				const bool selectedSkill = (int)it.first;
				ImGui::PushID((int)it.first);
				skill idk;
				std::string skillName = idk.getSkillName(it.first);

				if (ImGui::Selectable(skillName.c_str(), selectedSkill)) 
					currentSkill = it.first;
				
				ImGui::PopID();
			}
			ImGui::EndListBox();
		}
	}
	ImGui::End();
}
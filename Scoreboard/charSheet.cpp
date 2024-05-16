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

void charSheet(bool* enable) {
	// Important character stuff
	playerCharacter testChar;
	skill			idk;
	static skills	currentSkill = skills::acrobatics;
	// Initiatlizers
	testChar.initScores();
	testChar.setScore(abilityScores::strength, 14);
	testChar.initSkills();

	ImGui::SetNextWindowSize(ImVec2(600, 800));
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

		static char filterSkills[32];
		ImGui::Columns(3, "##SkillsColumn", false);
		{
			ImGui::SetColumnOffset(1, 200);
			ImGui::BeginChild("##SkillsOutput", ImVec2(-1, 100), ImGuiChildFlags_Border);
			{
				int skillScoreBonus = testChar.getMod(testChar.skillInfo[currentSkill].mainAbility);
				int profBonus = testChar.calcProfBonus(currentSkill);
				ImGui::Text("Ability Bonus: %i", skillScoreBonus);
				ImGui::Text("Proficiency Bonus: %i", profBonus);
			}
			ImGui::EndChild();
			ImGui::PushItemWidth(-1);
			ImGui::InputTextWithHint("##FilterSkills", "Filter Skills...", filterSkills, IM_ARRAYSIZE(filterSkills));
			ImGui::PopItemWidth();

			ImGui::BeginListBox("##SkillsList", ImVec2(-1, -2));
			for (auto &it : testChar.skillInfo) {
				if (!Contains(ToLower(filterSkills), ToLower(idk.getSkillName(it.first).c_str())))
					continue;

				const bool selectedSkill = (int)it.first == (int)currentSkill;
				ImGui::PushID((int)it.first);
				std::string skillName = idk.getSkillName(it.first).c_str();

				if (ImGui::Selectable(skillName.c_str(), selectedSkill))
					currentSkill = it.first;

				ImGui::PopID();
			}
			ImGui::EndListBox();
		}
	}
	ImGui::End();
}
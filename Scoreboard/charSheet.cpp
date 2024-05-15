#include "charSheet.h"

void charSheet(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(600, 800));

	if (!ImGui::Begin("Character Sheet", enable), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar) {
		ImGui::BeginChild("##AbilityScores", ImVec2(-1, 60), ImGuiChildFlags_Border); 
		{
			ImGui::Columns(6, "##AbilityScoreColumns", false);
			{
				ImGui::Text("Strength");
				//ImGui::Text("%d", &testChar.abilities[abilityScores::strength]);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Dexterity");
				//ImGui::Text("%d", &testChar.abilities[abilityScores::dexterity]);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Constitution");
				//ImGui::Text("%d", &testChar.abilities[abilityScores::constitution]);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Intelligence");
				//ImGui::Text("%d", &testChar.abilities[abilityScores::intelligence]);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Wisdom");
				//ImGui::Text("%d", &testChar.abilities[abilityScores::wisdom]);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Charisma");
				//ImGui::Text("%d", &testChar.abilities[abilityScores::charisma]);
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
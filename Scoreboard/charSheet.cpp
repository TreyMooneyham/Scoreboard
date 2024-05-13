#include "charSheet.h"

void charSheet(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(600, 800));

	if (!ImGui::Begin("Character Sheet", enable), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar) {
		ImGui::BeginChild("##AbilityScores", ImVec2(-1, 60), ImGuiChildFlags_Border); 
		{
			ImGui::Columns(6, "##AbilityScoreColumns", false);
			{
				ImGui::Text("Strength");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Dexterity");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Constitution");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Intelligence");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Wisdom");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Charisma");
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
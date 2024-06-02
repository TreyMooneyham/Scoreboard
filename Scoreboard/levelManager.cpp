// Includes
#include "levelManager.h"

void renderLevelOptions(int level) {
	ImGui::Text("Level %i", level);
}

void levelManager(bool* enable) {
	// Variables
	const char* levelsList[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
							 "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen",
							 "Seventeen", "Eighteen", "Nineteen", "Twenty" }; // May God forgive me for my sins...
	int currentLevel = globalChar.getLevel(levels::character);

	// Window
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	if (ImGui::Begin("Level Manager", enable, ImGuiWindowFlags_NoResize)) {
		if (ImGui::BeginChild("LevelChild", ImVec2(200, -1), ImGuiChildFlags_Border)) {
			for (int i = 0; i < IM_ARRAYSIZE(levelsList); i++) {
				std::string childName = "levelSubChild" + std::to_string(i);
				ImGui::BeginChild(childName.c_str(), ImVec2(-1, 100), ImGuiChildFlags_Border);
				{
					ImGui::Text("Level %s", levelsList[i]);
					ImGui::Separator();
					renderLevelOptions(i);
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		if (ImGui::BeginChild("LevelSelectChild", ImVec2(-1, -1), ImGuiChildFlags_Border)) {

			ImGui::EndChild();
		}

		ImGui::End();
	}
}
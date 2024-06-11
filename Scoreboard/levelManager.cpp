// Includes
#include "levelManager.h"

int selectedLevel = 1;

void renderLevelOptions(int level) {
	
}

void levelManager(bool* enable) {
	// Variables
	const char* levelsList[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
								 "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen",
								 "Seventeen", "Eighteen", "Nineteen", "Twenty" }; // May God forgive me for my sins...

	// Window
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	if (ImGui::Begin("Level Manager", enable, ImGuiWindowFlags_NoResize)) {
		if (ImGui::BeginChild("LevelChild", ImVec2(160, -1), ImGuiChildFlags_Border)) {
			for (int i = 0; i < IM_ARRAYSIZE(levelsList); i++) {
				std::string childName = "levelSubChild" + std::to_string(i);
				ImGui::BeginChild(childName.c_str(), ImVec2(-1, 40), ImGuiChildFlags_Border);
				{
					std::string formatName = "Level ";
					formatName += levelsList[i];

					if (ImGui::Button(formatName.c_str(), ImVec2(-1, -1))) {
						selectedLevel = i+1;
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		if (ImGui::BeginChild("LevelSelectChild", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
			ImGui::Text("Current Level: %i", globalChar.levelInfo[levels::character]);
			ImGui::Text("Selected Level: %i", selectedLevel);
			
			ImGui::Separator();

			ImGui::EndChild();
		}

		ImGui::End();
	}
}
// Includes
#include "levelManager.h"

// Variables
int selectedLevel = 1;

void renderLevelOptions(int level) {
	
}

void levelManager(bool* enable) {
	// Variables
	const char* levelsList[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
								 "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen",
								 "Seventeen", "Eighteen", "Nineteen", "Twenty" }; // May God forgive me for my sins...

	// The GUI
	ImGui::SetNextWindowSize(ImVec2(500, 395));
	if (ImGui::Begin("Level Manager", enable, ImGuiWindowFlags_NoResize)) {
		if (ImGui::BeginChild("LevelChild", ImVec2(160, -1), ImGuiChildFlags_Border)) {
			if (ImGui::BeginListBox("##LevelsListBox", ImVec2(-1, -1))) {
				for (int i = 0; i < IM_ARRAYSIZE(levelsList); i++) {
					bool bSelectedLevel = (selectedLevel - 1 == i);
					std::string formatName = "Level ";
					formatName += levelsList[i];

					if (ImGui::Selectable(formatName.c_str(), bSelectedLevel))
						selectedLevel = i + 1;
				}
				ImGui::EndListBox();
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		if (ImGui::BeginChild("LevelSelectChild", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
			ImGui::Text("Current Level: %i", globalChar.levelInfo[levels::character]);
			ImGui::Text("Selected Level: %i", selectedLevel);
			
			ImGui::Separator();

			renderLevelOptions(selectedLevel);

			ImGui::EndChild();
		}

		ImGui::End();
	}
}

void attribManager(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(500, 395));
	if (ImGui::Begin("Attribute Manager", enable, ImGuiWindowFlags_NoResize)) {


		ImGui::End();
	}
}
// Includes
#include "damageCalcWindow.h"

void damageCalcWindow(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(500, 500));

	if (ImGui::Begin("Damage Calculator", enable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {

		ImGui::End();
	}
}
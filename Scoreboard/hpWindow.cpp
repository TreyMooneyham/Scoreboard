#include "hpWindow.h"

void hpWindow(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	if (ImGui::Begin("Hit Points Management", enable, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {
		ImGui::Text("Made it!");

		ImGui::End();
	}
}
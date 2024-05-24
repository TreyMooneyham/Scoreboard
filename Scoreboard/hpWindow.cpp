#include "hpWindow.h"

void hpWindow(bool* enable) {
	ImGui::SetNextWindowSize(ImVec2(162, 150));
	if (ImGui::Begin("HP Management", enable, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {
		ImGui::PushItemWidth(80);
		ImGui::InputInt("Rolled HP", &globalChar.hpInfo.rolledHP);
		ImGui::PopItemWidth();

		int hpTotal = globalChar.getHP(0) + (globalChar.getLevel(levels::character) * globalChar.getMod(abilityScores::constitution));
		ImGui::Text("Max Hit Points: %i", hpTotal);

		ImGui::End();
	}
}
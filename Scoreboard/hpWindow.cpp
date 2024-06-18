// Includes
#include "hpWindow.h"

// Switches between the section system (false) or just 12 hp max per level (true).
const bool classicRoll = true;

void hpWindow(bool* enable) {
	int maxHP = globalChar.getHP(0) + (globalChar.getLevel(levels::character) * globalChar.getMod(abilityScores::constitution));
	int minRolledHP = (globalChar.getLevel(levels::rogue) * 2) + (globalChar.getLevel(levels::spellcaster) * 2) + (globalChar.getLevel(levels::warrior) * 2);
	int maxRolledHP = (globalChar.getLevel(levels::rogue)) * 8 + (globalChar.getLevel(levels::spellcaster) * 6) + (globalChar.getLevel(levels::warrior) * 10);

	ImGui::SetNextWindowSize(ImVec2(188, 200));
	if (ImGui::Begin("HP Management", enable, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {
		if (ImGui::BeginChild("RolledHPChild", ImVec2(172, -1), ImGuiChildFlags_Border)) {
			if (classicRoll) {
				ImGui::PushItemWidth(-1);
				ImGui::SliderInt("##RolledHPInput", &globalChar.hpInfo.rolledHP, 0, 12*globalChar.getLevel(levels::character), "Rolled HP: %i", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();

				ImGui::Separator();

				ImGui::Text("Character Level:   %3.i", globalChar.getLevel(levels::character));
				ImGui::Text("Con Modifier:      %3.i", globalChar.getMod(abilityScores::constitution));

				ImGui::Separator();

				ImGui::Text("Max Hit Points:    %3.i", maxHP);
			}
			else {
				ImGui::PushItemWidth(-1);
				ImGui::SliderInt("##RolledHPInput", &globalChar.hpInfo.rolledHP, minRolledHP, maxRolledHP, "Rolled HP: %i", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();

				ImGui::Separator();

				ImGui::Text("Rogue Level:       %3.i", globalChar.getLevel(levels::rogue));
				ImGui::Text("Spellcaster Level: %3.i", globalChar.getLevel(levels::spellcaster));
				ImGui::Text("Warrior Level:     %3.i", globalChar.getLevel(levels::warrior));
				ImGui::Text("Character Level:   %3.i", globalChar.getLevel(levels::character));
				ImGui::Text("Con Modifier:      %3.i", globalChar.getMod(abilityScores::constitution));

				ImGui::Separator();

				ImGui::Text("Max Hit Points:    %3.i", maxHP);
			}
			ImGui::EndChild();
		}

		ImGui::End();
	}
}
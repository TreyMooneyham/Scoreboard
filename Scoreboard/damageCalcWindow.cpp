// Includes
#include "damageCalcWindow.h"

// Important variables for ImGui
const char* damageTypesList[] = { "Mechanical", 
								  "Bludgeoning", "Force", "Piercing", "Slashing", 
	                              "Non-Mechanical",
                                  "Acid", "Bio", "Cold", "Electricity", "Energy", "Heat", "Thunder", "Psychic" };
resistanceTypes currentResistance = resistanceTypes::mechanical;
resistanceTypes currentDamage[] = { resistanceTypes::mechanical, resistanceTypes::mechanical, resistanceTypes::mechanical, resistanceTypes::mechanical, resistanceTypes::mechanical };
int currentDT = 0;
int intDR = 0;
float currentDR = 0.0f;
int damageAmts[] = { 0, 0, 0, 0, 0 };

int damageCalc(resistanceTypes damageType, int damage) {
	int resultDT =			damage - globalChar.resistInfo[damageType].dt;
	int resultDR =			damage - (damage * globalChar.resistInfo[damageType].dr);
	int resultDTMech =		damage - globalChar.resistInfo[resistanceTypes::mechanical].dt;
	int resultDRMech =		damage - (damage * globalChar.resistInfo[resistanceTypes::mechanical].dr);
	int resultDTNonMech =	damage - globalChar.resistInfo[resistanceTypes::nonMechanical].dt;
	int resultDRNonMech =	damage - (damage * globalChar.resistInfo[resistanceTypes::nonMechanical].dr);

	if (globalChar.isMech(damageType)) {
		if (resultDT > resultDTMech)
			resultDT = resultDTMech;
		if (resultDR > resultDRMech)
			resultDR = resultDRMech;
	} else {
		if (resultDT > resultDTNonMech)
			resultDT = resultDTNonMech;
		if (resultDR > resultDRNonMech)
			resultDR = resultDRNonMech;
	}

	if (resultDT < 0)
		resultDT = 0;

	if (resultDT < resultDR)
		return resultDT;

	return resultDR;
}

void damageCalcWindow(bool* enable) {
	// Window starts here
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	if (ImGui::Begin("Damage Calculator", enable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::BeginChild("ResistanceManagerChild", ImVec2(200, -1), ImGuiChildFlags_Border)) {
			ImGui::PushItemWidth(-1);
			std::string resistanceText = damageTypesList[(int)currentResistance];
			if (ImGui::BeginCombo("##ResistanceSelector", resistanceText.c_str())) {
				for (auto& it : globalChar.resistInfo) {
					const bool selectedResistance = ((int)it.first == (int)currentResistance);

					std::string selectedResistanceText = damageTypesList[(int)it.first];
					ImGui::PushID((int)it.first);
					if (ImGui::Selectable(selectedResistanceText.c_str(), selectedResistance))
						currentResistance = it.first;

					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
			ImGui::SliderInt("##DamageThresh", &currentDT, 0, 30, "DT: %i");
			ImGui::SliderInt("##DamageResist", &intDR, 0, 100, "DT: %i%%");
			currentDR = intDR / 100.0f;

			ImGui::PopItemWidth();

			if (ImGui::Button("Apply Resistance", ImVec2(-1, 0))) {
				globalChar.setDTR(currentResistance, currentDT, currentDR);
			}

			ImGui::Separator();

			ImGui::Text("Resistances:");
			if (ImGui::BeginChild("ResistancesList", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
				for (auto& it : globalChar.resistInfo) {
					if (globalChar.resistInfo[it.first].dr == 0 && globalChar.resistInfo[it.first].dt == 0)
						continue;

					ImGui::Text(damageTypesList[(int)it.first]);
					ImGui::Text("  DT: %i", globalChar.resistInfo[it.first].dt);
					ImGui::Text("  DR: %1.0f%%", globalChar.resistInfo[it.first].dr*100);
				}

				ImGui::EndChild();
			}

			ImGui::EndChild();

			ImGui::SameLine();

			if (ImGui::BeginChild("DamageCalculatorChild", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
				for (int i = 0; i < 5; i++) {
					ImGui::Text("Damage:");
					ImGui::PushItemWidth((ImGui::GetWindowWidth() - 24.5) / 2);
					ImGui::InputInt("##DamageAmount", &damageAmts[i], 1, 5);
					if (damageAmts[i] < 0)
						damageAmts[i] = 0;
					ImGui::SameLine();
					std::string damageText = damageTypesList[(int)currentDamage[i]];
					ImGui::PushID(1234 + i);
					if (ImGui::BeginCombo("##DamageType", damageText.c_str())) {
						ImGui::PopID();
						for (auto& it : globalChar.resistInfo) {
							const bool selectedDamage = ((int)it.first == (int)currentDamage);

							std::string selectedDamageText = damageTypesList[(int)it.first];
							ImGui::PushID((int)it.first);
							if (ImGui::Selectable(selectedDamageText.c_str(), selectedDamage))
								currentDamage[i] = it.first;

							ImGui::PopID();
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();

					ImGui::Separator();

					ImGui::Text("Result: %i", damageCalc(currentDamage[i], damageAmts[i]));

					ImGui::Separator();
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}
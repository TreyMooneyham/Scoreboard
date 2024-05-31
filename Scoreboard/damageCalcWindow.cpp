// Includes
#include "damageCalcWindow.h"

// Important variables for ImGui
const char* damageTypesList[] = { "Mechanical", 
								  "Bludgeoning", "Force", "Piercing", "Slashing", 
	                              "Non-Mechanical",
                                  "Acid", "Bio", "Cold", "Electricity", "Energy", "Heat", "Thunder", "Psychic" };
resistanceTypes currentResistance = resistanceTypes::mechanical;
resistanceTypes currentDamage = resistanceTypes::mechanical;
resistanceTypes currentDamage2 = resistanceTypes::mechanical;
int currentDT = 0;
int intDR = 0;
float currentDR = 0.0f;
int damageAmt = 0, damageAmt2 = 0;

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
				ImGui::Text("Damage:");
				ImGui::PushItemWidth((ImGui::GetWindowWidth()-24.5)/2);
				ImGui::InputInt("##DamageAmount", &damageAmt, 1, 5);
				if (damageAmt < 0)
					damageAmt = 0;
				ImGui::SameLine();
				std::string damageText = damageTypesList[(int)currentDamage];
				if (ImGui::BeginCombo("##DamageType", damageText.c_str())) {
					for (auto& it : globalChar.resistInfo) {
						const bool selectedDamage = ((int)it.first == (int)currentDamage);

						std::string selectedDamageText = damageTypesList[(int)it.first];
						ImGui::PushID((int)it.first);
						if (ImGui::Selectable(selectedDamageText.c_str(), selectedDamage))
							currentDamage = it.first;

						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				ImGui::Separator();

				ImGui::Text("Result: %i", damageCalc(currentDamage, damageAmt));

				ImGui::Separator();

				ImGui::Text("Damage:");
				ImGui::PushItemWidth((ImGui::GetWindowWidth() - 24.5) / 2);
				ImGui::InputInt("##DamageAmount2", &damageAmt2, 1, 5);
				if (damageAmt2 < 0)
					damageAmt2 = 0;
				ImGui::SameLine();
				std::string damageText2 = damageTypesList[(int)currentDamage2];
				if (ImGui::BeginCombo("##DamageType2", damageText2.c_str())) {
					for (auto& it : globalChar.resistInfo) {
						const bool selectedDamage = ((int)it.first == (int)currentDamage2);

						std::string selectedDamageText = damageTypesList[(int)it.first];
						ImGui::PushID((int)it.first);
						if (ImGui::Selectable(selectedDamageText.c_str(), selectedDamage))
							currentDamage2 = it.first;

						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				ImGui::Separator();

				ImGui::Text("Result: %i", damageCalc(currentDamage2, damageAmt2));

				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}
// Includes
#include "charSheet.h"

bool contains(const std::string& word, const std::string& sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string toLower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), (int (*)(int))std::tolower);

	return str;
}

std::string modFormat(std::string str, int bonus) {
	str += ":";

	int diff = 22 - str.length();
	if (bonus > 9)
		diff--;

	for (int i = 0; i < diff; i++) {
		str += " ";
	}

	(bonus < 0) ? str += std::to_string(bonus) : str += "+" + std::to_string(bonus);

	return str;
}

std::string modFormat(const char* charray, int bonus) {
	std::string str;
	str += charray;

	str = modFormat(str, bonus);

	return str;
}

std::string acFormat(std::string str, int bonus) {
	str += ":";

	int diff = 21 - str.length();
	if (bonus > 9)
		diff--;

	for (int i = 0; i < diff; i++) {
		str += " ";
	}

	return str + std::to_string(bonus);
}

std::string condFormat(std::string str, int bonus) {
	str += " ";
	return str + std::to_string(bonus);
}

template <typename T>
void setAdjustment(T& obj, int val) {
	globalChar.setAdj(obj, val);
}

// Here's the global character variable
extern playerCharacter globalChar = playerCharacter();

static skill idk;
static skills currentSkill = skills::acrobatics;
static proficiencyLevels currentProficiencyLevel = proficiencyLevels::noProficiency;
static proficiencyLevels currentInitProficiencyLevel = proficiencyLevels::noProficiency;

// Special things for ImGui to use
const char* proficiencyLevelsList[] = { "No Proficiency", "Proficiency", "Expertise", "Mastery", "Legendary" }; // List of proficiency levels
const char* mainAbilityList[] = { "Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma" }; // List of ability scores
const char* savesList[] = { "Fortitude", "Reflex", "Will" }; // List of saving throws
const char* levelsList[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
							 "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", 
							 "Seventeen", "Eighteen", "Nineteen", "Twenty" }; // May God forgive me for my sins...
static char filterSkills[32];
char pNameChar[128] = "", cNameChar[128] = "", ancestryChar[128] = "", nationalityChar[128] = "";
const char* conditionsList[] = { "Blinded", "Clumsy", "Confused", "Controlled", "Dazed", "Deafened", "Doomed", "Drained", 
								 "Dying", "Encumbered", "Enfeebled", "Fatigued", "Fleeing", "Frightened", "Grappled", 
								 "Hidden", "Immobilized", "Invisible", "Paralyzed", "Petrified", "Prone", "Quickened", 
								 "Restrained", "Sickened", "Slowed", "Stabilized", "Stunned", "Stupefied", "Unconscious", 
								 "Undetected" };
const char* conditionDefinitionList[] = { 
	"All terrain is difficult.\n\nPerception checks that rely on sound always fail.\n\nYou are immune to effects that require sight.\n\nBlinded overwrites dazed.\n\nYou have disadvantage on attack rolls.", 
	"Clumsy includes a value.\n\nYou have a penalty to dexterity-based attack rolls, ability checks, DCs, and saving throws equal to your clumsy value.", 
	"You do not treat anyone as your ally.\n\nYou cannot Delay, Ready, or take Reactions.\n\nYou must use your turn to attack random nearby creatures.\n\nWhenever you take damage, you can attempt to save against the effect.", 
	"You no longer have control of your character.", 
	"All attacks you make have a 25%% chance to miss (DC 5 flat check).", 
	"Perception checks that rely on sound automatically fail.\n\nYou are immune to effects that require hearing.", 
	"Doomed includes a value.\n\nYou have a penalty to Death Saving Throws equal to your doomed value.\n\nDoomed decreases by 1 for each long rest you take.", 
	"Drained includes a value.\n\nYou have a penalty to constitution based attack rolls, ability checks, DCs, and saving throws equal to your drained value.\n\nYou lose a number of hit points equal to your character level times the drained value.\n\nYou maximum hit points is also reduced by your character level times the drained value.\n\nDrained decreases by 1 for each long rest you take.",
	"You receive the dying condition if your hit points are less than 0.\n\nYou are unconscious.\n\nAt the end of your turn, you roll a death saving throw. On a roll of 1-10 you fail. After three failures you die.\n\nIf your hit points are reduced to negative half your maximum hit points, you die.\n\nIf you receive any healing that does not take you over 0 hit points, you go to 0 hit points.\n\nIf another creatures makes a medicine check, DC 18, to stabilize you, you stabilize. If that creatures rolls a natural 1, you instead take a failure on your death saves.", 
	"You are clumsy 1.\n\nYou speeds are reduced by 10 feet.\n\nThis cannot reduce your speed to less than 5 feet.", 
	"Enfeebled incudes a value.\n\nYou have a penalty to strength based attack rolls, ability checks, DCs, and saving throws equal to your enfeebled value.", 
	"You have a -1 penalty to your AC and saving throws.\n\nYou cannot travel quickly, or stealthily, and cannot perform general tasks while traveling.\n\nYou recover from fatigued after a long rest.", 
	"You must spend your turn getting away from the source of your condition as fast as possible.\n\nYou cannot Delay or Ready while fleeing.", 
	"Frightened includes a value.\n\nYou have disadvantage on attack rolls and ability checks against the source of frightened.\n\nYou cannot willingly move closer the the source of frightened.\n\nThe frightened value decreases by 1 at the end of your turn.", 
	"You are immobilized.\n\nAttack rolls against you have advantage.", 
	"You have advantage on attack rolls and checks against creatures that you are hidden from.\n\nIf a creature targets you with something, it has a 55%% chance (DC 11 flat check) to lose that action before being able to make the roll.", 
	"Your speed is zero.", 
	"You are undetected by everyone.\n\nA creature can make a perception check to find you, which makes you hidden until you succeed on a stealth check to become undetected again.\n\nIf you become invisible while seen, you are only hidden to that creature until you succeed on a stealth check to become undetected.", 
	"You cannot move.\n\nYour AC is reduced by 2.\n\nYou cannot take actions that require movement of any kind.", 
	"You cannot do anything.\n\nYou have an AC of 9, your hit points stay the same as when you weren't stone, and your weight is quadrupled.\n\nYou do not age or notice time passing.", 
	"You treat all terrain as difficult.\n\nYou must spend half your movement to stand up.\n\nRanged attacks against you are made at disadvantage.\n\nMelee attacks against you are made at advantage.\n\nYour non-special attack rolls are made at disadvantage.", 
	"Your speed increases by 10 feet.\n\nYou gain a +2 bonus to AC.\n\nYou gain an additional action.\n\nThis action can only be used to make one attack, dash, dodge, disengage, hide, or use an object.", 
	"Your AC is reduced by 2.\n\nYou are immobilized.\n\nYou cannot make attack rolls and can only make attempts to escape.", 
	"Sickened includes a value.\n\nYou have a penalty to attack rolls and ability checks equal to the value tied to sickened.\n\nYou cannot willingly ingest anything (Including potions).\n\nYou can spend an action to throw up, immediately making a fortitude saving throw. On a success you reduce your Sickened level by 1.", 
	"You cannot take reactions while slowed.\n\nYou can only take an action or a bonus action on your turn.\n\nYour speed is reduced by 10 feet.", 
	"You do not make death saving throws.\n\nYou heal 1 hit point every hour.", 
	"Stunned overwrites slowed.\n\nYou cannot take reactions while stunned.\n\nYour are immobilized.\n\nYou cannot take actions or bonus actions.", 
	"Stupefied includes a value.\n\nYou have a penalty to intelligence, wisdom, and charisma based attack rolls, ability checks, DCs, and saving throws equal to your stupefied value.\n\nWhenever you cast a spell you must beat a DC 5 + stupefied value flat roll or the spell fizzles.", 
	"You are stunned.\n\nYou are prone.", 
	"You have a +2 bonus to attack rolls against creatures you are undetected by.\n\nCreatures can attempt to target you by attacking a square.\n\nThey must succeed on a DC 11 flat check to make an attack roll against you if you are in that square."
};
conditions currentCondition = conditions::blinded;
const char* armorTypesList[] = { "Unarmored", "Light", "Medium", "Heavy" };
const char* baseACList[] = { "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen" };
armorTypes currentArmorType = armorTypes::unarmored;
movements currentMovementType = movements::walking;
const char* movementTypesList[] = { "Walking", "Climbing", "Swimming", "Flying", "Burrowing" };
int condVal = 1;

bool actionTab = true;
bool inventoryTab = true;

void charSheet(bool* enable) {
	// Common variables for the global character
	// Ability scores
	int strScore = globalChar.getScore(abilityScores::strength);
	int conScore = globalChar.getScore(abilityScores::constitution);
	int dexScore = globalChar.getScore(abilityScores::dexterity);
	int intScore = globalChar.getScore(abilityScores::intelligence);
	int wisScore = globalChar.getScore(abilityScores::wisdom);
	int chaScore = globalChar.getScore(abilityScores::charisma);

	// Ability modifiers
	int strMod = globalChar.getMod(abilityScores::strength);
	int conMod = globalChar.getMod(abilityScores::constitution);
	int dexMod = globalChar.getMod(abilityScores::dexterity);
	int intMod = globalChar.getMod(abilityScores::intelligence);
	int wisMod = globalChar.getMod(abilityScores::wisdom);
	int chaMod = globalChar.getMod(abilityScores::charisma);

	// Ability Adjustments
	int strAdj = globalChar.abilityAdj[abilityScores::strength];
	int dexAdj = globalChar.abilityAdj[abilityScores::dexterity];
	int conAdj = globalChar.abilityAdj[abilityScores::constitution];
	int intAdj = globalChar.abilityAdj[abilityScores::intelligence];
	int wisAdj = globalChar.abilityAdj[abilityScores::wisdom];
	int chaAdj = globalChar.abilityAdj[abilityScores::charisma];

	// Ability Info Arrays
	int abilityScoreArr[] = { strScore, conScore, dexScore, intScore, wisScore, chaScore }; // Couldn't name this abilityScores oopsie
	int abilityScoreMods[] = { strMod, conMod, dexMod, intMod, wisMod, chaMod };
	int abilityAdjustments[] = { strAdj, conAdj, dexAdj, intAdj, wisAdj, chaAdj };

	// Saving throws
	// Notably much longer than skills because only one skill gets shown at a time
	// Sucks...
	int fortScoreBonus = globalChar.getSave(savingThrows::fortitude);
	int refScoreBonus = globalChar.getSave(savingThrows::reflex);
	int willScoreBonus = globalChar.getSave(savingThrows::will);

	int fortProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::fortitude));
	int refProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::reflex));
	int willProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::will));

	int fortTotalBonus = fortScoreBonus + fortProfBonus;
	int refTotalBonus = refScoreBonus + refProfBonus;
	int willTotalBonus = willScoreBonus + willProfBonus;

	int fortAdjustments = globalChar.getAdj(savingThrows::fortitude);
	int refAdjustments = globalChar.getAdj(savingThrows::reflex);
	int willAdjustments = globalChar.getAdj(savingThrows::will);

	// A little bit of turning my saves into an array...
	// This is done for looping and readability. Not because it is optimal.
	// This is quite bad, honestly.
	int saveScoreBonus[] = { fortScoreBonus, refScoreBonus, willScoreBonus };
	int saveProfBonus[] = { fortProfBonus, refProfBonus, willProfBonus };
	int saveTotalBonus[] = { fortTotalBonus, refTotalBonus, willTotalBonus };
	int saveAdjustments[] = { fortAdjustments, refAdjustments, willAdjustments };

	// Skills
	int skillScoreBonus = globalChar.getMod(globalChar.skillInfo[currentSkill].mainAbility);
	int skillProfBonus = globalChar.getProfBonus(globalChar.skillInfo[currentSkill].profLevel);
	int skillTotalBonus = skillScoreBonus + skillProfBonus;
	int skillAdjustments = globalChar.getAdj(currentSkill);

	int initiativeScoreBonus = globalChar.getMod(globalChar.skillInfo[skills::initiative].mainAbility);
	int initiativeProfBonus = globalChar.getProfBonus(globalChar.skillInfo[skills::initiative].profLevel);
	int initiativeTotalBonus = initiativeScoreBonus + initiativeProfBonus;
	int initiativeAdjustments = globalChar.getAdj(skills::initiative);

	// Hit points
	int maxHP = globalChar.getHP(0) + (globalChar.getLevel(levels::character) * globalChar.getMod(abilityScores::constitution));

	// Armor class
	int currentBaseAC = globalChar.getBaseArmorClass(currentArmorType);

	// Movement
	int currentSpeed = globalChar.getSpeed(currentMovementType);

	// Actual menu starts here
	ImGui::SetNextWindowSize(ImVec2(800, 800));
	if (ImGui::Begin("Character Sheet", enable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::BeginChild("CharInfoChild", ImVec2(-1, 58), ImGuiChildFlags_Border)) {
			ImGui::Columns(4, NULL, false);
			{
				ImGui::PushItemWidth(-1);
				strcpy_s(pNameChar, globalChar.getName(0).c_str());
				if (ImGui::InputTextWithHint("##pNameBox", "Player name...", pNameChar, IM_ARRAYSIZE(pNameChar))) {
					std::string pNameStr = pNameChar;
					globalChar.setName(pNameStr, 0);
				}

				strcpy_s(cNameChar, globalChar.getName(1).c_str());
				if (ImGui::InputTextWithHint("##cNameBox", "Character name...", cNameChar, IM_ARRAYSIZE(cNameChar))) {
					std::string cNameStr = cNameChar;
					globalChar.setName(cNameStr, 1);
				}
				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				strcpy_s(ancestryChar, globalChar.getName(2).c_str());
				if (ImGui::InputTextWithHint("##ancestryBox", "Ancestry...", ancestryChar, IM_ARRAYSIZE(ancestryChar))) {
					std::string ancestryStr = ancestryChar;
					globalChar.setName(ancestryStr, 2);
				}

				strcpy_s(nationalityChar, globalChar.getName(3).c_str());
				if (ImGui::InputTextWithHint("##nationBox", "Nationality...", nationalityChar, IM_ARRAYSIZE(nationalityChar))) {
					std::string nationaityStr = nationalityChar;
					globalChar.setName(nationalityChar, 3);
				}

				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);

				ImGui::Text("Haaaiiii :3");

				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);

				std::string formatHPString = std::to_string(globalChar.hpInfo.currentHP) + "/" + std::to_string(maxHP) + " Hit Points";
				ImGui::SliderInt("##CurrentHPCharSheet", &globalChar.hpInfo.currentHP, 0 - maxHP / 2, maxHP, formatHPString.c_str());

				std::string lvlLabel = "Level: ";
				lvlLabel += std::to_string(globalChar.getLevel(levels::character));
				if (ImGui::BeginCombo("##LevelCombo", lvlLabel.c_str())) {
					for (int i = 0; i < IM_ARRAYSIZE(levelsList); i++) {
						bool selectedLevel = (globalChar.levelInfo[levels::character] == i + 1);

						std::string lvl = levelsList[i];
						if (ImGui::Selectable(lvl.c_str(), selectedLevel)) {
							globalChar.setLevel(levels::character, i + 1);
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();
			}
			ImGui::EndChild();
		}

		if (ImGui::BeginChild("##AbilityScores", ImVec2(190, 170), ImGuiChildFlags_Border)) {
				for (int i = 0; i < IM_ARRAYSIZE(mainAbilityList); i++) {
					std::string formattedAbility = mainAbilityList[i];

					if (abilityAdjustments[i] > 0) {
						ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), modFormat(formattedAbility, abilityScoreMods[i] + abilityAdjustments[i]).c_str());
					}
					else if (abilityAdjustments[i] < 0) {
						ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), modFormat(formattedAbility, abilityScoreMods[i] + abilityAdjustments[i]).c_str());
					}
					else {
						ImGui::Text(modFormat(formattedAbility, abilityScoreMods[i]).c_str());
					}
				}

				ImGui::Separator();

				for (int i = 0; i < 3; i++) {
					std::string saveStr = savesList[i];

					if (saveAdjustments[i] > 0) { // When the adjustments are positive
						ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), modFormat(saveStr, saveTotalBonus[i] + saveAdjustments[i]).c_str());
					}
					else if (saveAdjustments[i] < 0) { // When the adjustments are negative
						ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), modFormat(saveStr, saveTotalBonus[i] + saveAdjustments[i]).c_str());
					}
					else { // When the adjustments are zero
						ImGui::Text(modFormat(saveStr, saveTotalBonus[i]).c_str());
					}
				}

				ImGui::EndChild();
			}

		ImGui::SameLine();

		if (ImGui::BeginChild("##ImportantFeatures", ImVec2(-1, 170), ImGuiChildFlags_Border)) {
			ImGui::Columns(3, "FeatureColumns", false);
			{
				ImGui::PushItemWidth(-1);
				ImGui::Text("Current Armor Type");
				if (ImGui::BeginCombo("##ArmorTypeCombo", armorTypesList[(int)currentArmorType])) {
					for (int n = 0; n < IM_ARRAYSIZE(armorTypesList); n++) {
						const bool selectedArmorType = (currentArmorType == (armorTypes)n);

						if (ImGui::Selectable(armorTypesList[n], selectedArmorType))
							currentArmorType = (armorTypes)n;
					}

					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				if (ImGui::BeginChild("ArmorTypeAttribs", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
					ImGui::PushItemWidth(-1);
					std::string baseACStr = armorTypesList[(int)currentArmorType];
					baseACStr += " Base AC";
					ImGui::Text(baseACStr.c_str());
					if (ImGui::BeginCombo("##BaseACCombo", baseACList[currentBaseAC])) {
						for (int n = 0; n < IM_ARRAYSIZE(baseACList); n++) {
							const bool selectedBaseAC = (currentBaseAC == n);

							if (ImGui::Selectable(baseACList[n], selectedBaseAC))
								globalChar.setBaseArmorClass(currentArmorType, n);
						}

						ImGui::EndCombo();
					}

					std::string mainAbilityStr = armorTypesList[(int)currentArmorType];
					mainAbilityStr += " Main Ability";
					ImGui::Text(mainAbilityStr.c_str());
					if (ImGui::BeginCombo("##MainACAbilityCombo", mainAbilityList[(int)globalChar.getArmorMainAbility(currentArmorType)])) {
						for (int n = 0; n < IM_ARRAYSIZE(mainAbilityList); n++) {
							const bool selectedMainAbility = (globalChar.getArmorMainAbility(currentArmorType) == (abilityScores)n);

							if (ImGui::Selectable(mainAbilityList[n], selectedMainAbility))
								globalChar.setArmorMainAbility(currentArmorType, (abilityScores)n);
						}

						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();

					ImGui::Separator();
					std::string finalACStr = armorTypesList[(int)currentArmorType];
					finalACStr += " AC";
					ImGui::Text(acFormat(finalACStr, 10 + currentBaseAC + globalChar.getMod(globalChar.getArmorMainAbility(currentArmorType))).c_str());

					ImGui::EndChild();
				}
				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Text("Initiative Proficiency");
				std::string profText = proficiencyLevelsList[(int)currentInitProficiencyLevel];
				if (ImGui::BeginCombo("##InitiativeProf", profText.c_str())) {
					for (int n = 0; n < IM_ARRAYSIZE(proficiencyLevelsList); n++) {
						const bool selectedProf = (currentInitProficiencyLevel == (proficiencyLevels)n);

						std::string profName = proficiencyLevelsList[n];
						if (ImGui::Selectable(profName.c_str(), selectedProf)) {
							currentInitProficiencyLevel = (proficiencyLevels)n;
							globalChar.setSkillProficiency(skills::initiative, currentInitProficiencyLevel);
						}
					}
					ImGui::EndCombo();
				}
				std::string initiativeStr = "Initiative Bonus";
				ImGui::Text(modFormat(initiativeStr, initiativeTotalBonus).c_str());
				ImGui::PopItemWidth();

				if (ImGui::BeginChild("Bullshitometer", ImVec2(-1, 0))) {
					ImGui::Separator();

					ImGui::PushItemWidth(-1);
					ImGui::Text("Movement Type");
					if (ImGui::BeginCombo("##MovementTypeCombo", movementTypesList[(int)currentMovementType])) {
						for (int n = 0; n < IM_ARRAYSIZE(movementTypesList); n++) {
							const bool selectedMovementType = (currentMovementType == (movements)n);

							if (ImGui::Selectable(movementTypesList[n], selectedMovementType))
								currentMovementType = (movements)n;
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();

					if (ImGui::BeginChild("MovementChild", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
						ImGui::PushItemWidth(-1);
						std::string movementStr = movementTypesList[(int)currentMovementType];
						movementStr += " Speed";
						ImGui::Text(movementStr.c_str());
						ImGui::InputInt("##MovementSpeedInput", &globalChar.movementInfo[currentMovementType], 5, 10);
						ImGui::PopItemWidth();

						ImGui::EndChild();
					}
					ImGui::EndChild();
				}
			}
			ImGui::NextColumn();
			{
				static char filterConditions[32];
				if (ImGui::Button("Manage Conditions...", ImVec2(-1, 0)))
					ImGui::OpenPopup("Condition Manager");

				ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(700, 306));
				if (ImGui::BeginPopupModal("Condition Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
					ImGui::Columns(2, "PopupColumns", false);
					{
						ImGui::SetColumnOffset(1, 180);
						ImGui::PushItemWidth(-1);
						ImGui::InputTextWithHint("##FilterConditions", "Filter Conditions...", filterConditions, IM_ARRAYSIZE(filterConditions));
						ImGui::PopItemWidth();

						if (ImGui::BeginListBox("##ConditionsListBox", ImVec2(-1, -1))) {
							for (int n = 0; n < IM_ARRAYSIZE(conditionsList); n++) {
								if (!contains(toLower(filterConditions), toLower(conditionsList[n])))
									continue;

								const bool selectedCondition = (currentCondition == (conditions)n);

								if (ImGui::Selectable(conditionsList[n], selectedCondition))
									currentCondition = (conditions)n;
							}
							ImGui::EndListBox();
						}
					}
					ImGui::NextColumn();
					{
						if (ImGui::BeginChild("ConditionInfoChild", ImVec2(-1, 248), ImGuiChildFlags_Border)) {
							ImGui::TextWrapped(conditionDefinitionList[(int)currentCondition]);

							ImGui::EndChild();
						}
						if (currentCondition == conditions::clumsy ||
							currentCondition == conditions::doomed ||
							currentCondition == conditions::drained ||
							currentCondition == conditions::enfeebled ||
							currentCondition == conditions::frightened ||
							currentCondition == conditions::sickened ||
							currentCondition == conditions::stupefied) {

							ImGui::PushItemWidth(154);
							ImGui::SliderInt("##ConditionSlider", &condVal, 1, 10, "Condition Value: %i");
							ImGui::SameLine();
							ImGui::PopItemWidth();

							if (ImGui::Button("Update Condition", ImVec2(123, 0))) {
								globalChar.setCondition(currentCondition, condVal);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Remove Condition", ImVec2(125, 0))) {
								globalChar.setCondition(currentCondition, 0);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Cancel", ImVec2(73, 0)))
								ImGui::CloseCurrentPopup();
						}
						else {
							if (ImGui::Button("Add Condition", ImVec2(180, 0))) {
								globalChar.setCondition(currentCondition, 1);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Remove Condition", ImVec2(180, 0))) {
								globalChar.setCondition(currentCondition, 0);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Cancel", ImVec2(123, 0)))
								ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginListBox("##ActiveConditionsListbox", ImVec2(-1, -1))) {
					for (int n = 0; n < IM_ARRAYSIZE(conditionsList); n++) {
						if (globalChar.conditionInfo[(conditions)n] < 1)
							continue;

						if ((conditions)n == conditions::clumsy ||
							(conditions)n == conditions::doomed ||
							(conditions)n == conditions::drained ||
							(conditions)n == conditions::enfeebled ||
							(conditions)n == conditions::frightened ||
							(conditions)n == conditions::sickened ||
							(conditions)n == conditions::stupefied) {
							ImGui::Text(condFormat(conditionsList[n], globalChar.conditionInfo[(conditions)n]).c_str());
							if (ImGui::BeginItemTooltip())
							{
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::TextUnformatted(conditionDefinitionList[n]);
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}
						}
						else {
							ImGui::Text(conditionsList[n]);
							if (ImGui::BeginItemTooltip())
							{
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::TextUnformatted(conditionDefinitionList[n]);
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}
						}
					}
					ImGui::EndListBox();
				}
			}
			ImGui::EndChild();
		}

		if (ImGui::BeginChild("##SkillsOutput", ImVec2(190, -1), ImGuiChildFlags_Border)) {
				
			ImGui::Text(modFormat("Ability Bonus", skillScoreBonus).c_str());
			ImGui::Text(modFormat("Proficiency Bonus", skillProfBonus).c_str());
			ImGui::Text(modFormat("Misc. Bonus", skillAdjustments).c_str());

			ImGui::Separator();

			if (skillAdjustments > 0) { // When the adjustment is positive
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), modFormat("Total Bonus", skillTotalBonus + skillAdjustments).c_str());
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "Passive DC:           %2i", skillTotalBonus + skillAdjustments + 10);
			}
			else if (skillAdjustments < 0) { // When the adjustment is negative
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), modFormat("Total Bonus", skillTotalBonus + skillAdjustments).c_str());
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Passive DC:           %2i", skillTotalBonus + skillAdjustments + 10);
			}
			else { // When the adjustment is zero
				ImGui::Text(modFormat("Total Bonus", skillTotalBonus).c_str());
				ImGui::Text("Passive DC:           %2i", skillTotalBonus + 10);
			}

			ImGui::Separator();

			ImGui::PushItemWidth(-1);
			ImGui::InputTextWithHint("##FilterSkills", "Filter Skills...", filterSkills, IM_ARRAYSIZE(filterSkills));

			std::string mainAbilityText = mainAbilityList[(int)globalChar.skillInfo[currentSkill].mainAbility];
			if (ImGui::BeginCombo("##mainAbility", mainAbilityText.c_str())) {
				for (int n = 0; n < IM_ARRAYSIZE(mainAbilityList); n++) {
					const bool selectedAbility = (globalChar.skillInfo[currentSkill].mainAbility == (abilityScores)n);

					std::string mainAbilityName = mainAbilityList[n];
					if (ImGui::Selectable(mainAbilityName.c_str(), selectedAbility)) {
						globalChar.skillInfo[currentSkill].mainAbility = (abilityScores)n;
					}
				}

				ImGui::EndCombo();
			}

			std::string profText = proficiencyLevelsList[(int)currentProficiencyLevel];
			if (ImGui::BeginCombo("##ProfLevels", profText.c_str())) {
				for (int n = 0; n < IM_ARRAYSIZE(proficiencyLevelsList); n++) {
					const bool selectedProf = (currentProficiencyLevel == (proficiencyLevels)n);

					std::string profName = proficiencyLevelsList[n];
					if (ImGui::Selectable(profName.c_str(), selectedProf)) {
						currentProficiencyLevel = (proficiencyLevels)n;
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Apply Proficiency", ImVec2(-1, 0))) {
				globalChar.setSkillProficiency(currentSkill, currentProficiencyLevel);
			}

			ImGui::PopItemWidth();

			if (ImGui::BeginListBox("##SkillsList", ImVec2(-1, -1))) {
				for (auto& it : globalChar.skillInfo) {
					if (!contains(toLower(filterSkills), toLower(idk.getSkillName(it.first).c_str())))
						continue;

					if (it.first == skills::initiative)
						continue;

					const bool selectedSkill = ((int)it.first == (int)currentSkill);
					ImGui::PushID((int)it.first);
					std::string skillName = idk.getSkillName(it.first).c_str();

					if (ImGui::Selectable(skillName.c_str(), selectedSkill))
						currentSkill = it.first;

					ImGui::PopID();
				}
				ImGui::EndListBox();
			}
			ImGui::EndChild();
		}

		ImGui::SameLine();

		if (ImGui::BeginChild("##Col2Child1", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
			if (ImGui::BeginTabBar("CharSheetTabBar", ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem("Actions")) {
					ImGui::Text("Action Tab");

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Inventory")) {
					ImGui::Text("Inventory Tab");

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Feats")) {
					ImGui::Text("Feats Tab");
					if (ImGui::Button("Add Feature...", ImVec2(0, 0)))
						ImGui::OpenPopup("Feature Manager");

					ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::SetNextWindowSize(ImVec2(700, 306));
					if (ImGui::BeginPopupModal("Feature Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
						if (ImGui::Button("Close", ImVec2(0, 0)))
							ImGui::CloseCurrentPopup();

						ImGui::EndPopup();
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndChild();
		}
	}
	ImGui::End();
}
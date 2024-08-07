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

std::string scoreFormat(std::string str, abilityScores ability) {
	int score = globalChar.getScore(ability), mod = globalChar.getMod(ability) + globalChar.getAdj(ability), diff;
	str += ":";
	(score < 10) ? diff = 19 - str.length() : diff = 18 - str.length();

	for (int i = 0; i < diff; i++) {
		str += " ";
	}

	str += std::to_string(score) + ", ";
	(mod < 0) ? str += std::to_string(mod) : str += "+" + std::to_string(mod);

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

void beginTooltip(const char* str) {
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(str);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
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
	"Clumsy includes a value.\n\nYou have a penalty to dexterity-based attack rolls, ability checks and DCs, and reflex saving throws equal to your clumsy value.", 
	"You do not treat anyone as your ally.\n\nYou cannot Delay, Ready, or take Reactions.\n\nYou must use your turn to attack random nearby creatures.\n\nWhenever you take damage, you can attempt to save against the effect.", 
	"You no longer have control of your character.", 
	"All attacks you make have a 25%% chance to miss (DC 5 flat check).", 
	"Perception checks that rely on sound automatically fail.\n\nYou are immune to effects that require hearing.", 
	"Doomed includes a value.\n\nYou have a penalty to Death Saving Throws equal to your doomed value.\n\nDoomed decreases by 1 for each long rest you take.", 
	"Drained includes a value.\n\nYou have a penalty to constitution based attack rolls, ability checks, and DCs, and fortitude saving throws equal to your drained value.\n\nYou lose a number of hit points equal to your character level times the drained value.\n\nYou maximum hit points is also reduced by your character level times the drained value.\n\nDrained decreases by 1 for each long rest you take.",
	"You receive the dying condition if your hit points are less than 0.\n\nYou are unconscious.\n\nAt the end of your turn, you roll a death saving throw. On a roll of 1-10 you fail. After three failures you die.\n\nIf your hit points are reduced to negative half your maximum hit points, you die.\n\nIf you receive any healing that does not take you over 0 hit points, you go to 0 hit points.\n\nIf another creatures makes a medicine check, DC 18, to stabilize you, you stabilize. If that creatures rolls a natural 1, you instead take a failure on your death saves.", 
	"You are clumsy 1.\n\nYou speeds are reduced by 10 feet.\n\nThis cannot reduce your speed to less than 5 feet.", 
	"Enfeebled incudes a value.\n\nYou have a penalty to strength based attack rolls, ability checks, and DCs equal to your enfeebled value.", 
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
	"Stupefied includes a value.\n\nYou have a penalty to intelligence, wisdom, and charisma based attack rolls, ability checks, and DCs, and will saving throws equal to your stupefied value.\n\nWhenever you cast a spell you must beat a DC 5 + stupefied value flat roll or the spell fizzles.", 
	"You are stunned.\n\nYou are prone.", 
	"You have a +2 bonus to attack rolls against creatures you are undetected by.\n\nCreatures can attempt to target you by attacking a square.\n\nThey must succeed on a DC 11 flat check to make an attack roll against you if you are in that square."
};
conditions currentCondition = conditions::blinded;
const char* armorTypesList[] = { "Unarmored", "Light", "Medium", "Heavy" };
const char* baseACList[] = { "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen" };
armorTypes currentArmorType = armorTypes::unarmored;
proficiencyLevels currentArmorProfLevel = proficiencyLevels::noProficiency;
movements currentMovementType = movements::walking;
const char* movementTypesList[] = { "Walking", "Swimming", "Climbing", "Flying", "Burrowing" };
int condVal = 1;
const char* actionTypesList[] = { "Action", "Bonus Action", "Reaction", "Other Action" };
action currentAction = { actionTypes::action, "", "" };
char actionNameStr[24];
char actionDescStr[1024 * 32];

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
	int strAdj = globalChar.getAdj(abilityScores::strength);
	int dexAdj = globalChar.getAdj(abilityScores::dexterity);
	int conAdj = globalChar.getAdj(abilityScores::constitution);
	int intAdj = globalChar.getAdj(abilityScores::intelligence);
	int wisAdj = globalChar.getAdj(abilityScores::wisdom);
	int chaAdj = globalChar.getAdj(abilityScores::charisma);

	// Ability Info Arrays
	int abilityScoreArr[] = { strScore, dexScore, conScore, intScore, wisScore, chaScore }; // Couldn't name this abilityScores oopsie
	int abilityScoreMods[] = { strMod, dexMod, conMod, intMod, wisMod, chaMod };
	int abilityAdjustments[] = { strAdj, dexAdj, conAdj, intAdj, wisAdj, chaAdj };

	// Saving throws
	// Notably much longer than skills because only one skill gets shown at a time
	// Sucks...
	int fortScoreBonus = globalChar.getSaveBonus(savingThrows::fortitude);
	int refScoreBonus = globalChar.getSaveBonus(savingThrows::reflex);
	int willScoreBonus = globalChar.getSaveBonus(savingThrows::will);

	int fortProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::fortitude));
	int refProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::reflex));
	int willProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::will));

	int fortTotalBonus = fortScoreBonus + fortProfBonus;
	int refTotalBonus = refScoreBonus + refProfBonus;
	int willTotalBonus = willScoreBonus + willProfBonus;

	int fortAdjustments = globalChar.getAdj(savingThrows::fortitude);
	int refAdjustments = globalChar.getAdj(savingThrows::reflex);
	int willAdjustments = globalChar.getAdj(savingThrows::will);
	int deathAdjustment = globalChar.getAdj(savingThrows::death);

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
	int dcAdjustment = skillAdjustments;

	int initiativeScoreBonus = globalChar.getMod(globalChar.skillInfo[skills::initiative].mainAbility);
	int initiativeProfBonus = globalChar.getProfBonus(globalChar.skillInfo[skills::initiative].profLevel);
	int initiativeTotalBonus = initiativeScoreBonus + initiativeProfBonus;
	int initiativeAdjustments = globalChar.getAdj(skills::initiative);

	// Hit points
	int maxHP = globalChar.getHP(0) + (globalChar.getLevel(levels::character) * globalChar.getMod(abilityScores::constitution));

	// Armor class
	int currentBaseAC = globalChar.getBaseArmorClass(currentArmorType);
	int acAdjustment = globalChar.getAdj(currentArmorType);
	int acAbilityBonus = globalChar.getMod(globalChar.getArmorMainAbility(currentArmorType));
	int acProfBonus = globalChar.getProfBonus(globalChar.getArmorProficiency(currentArmorType));
	int currentAC = 10 + currentBaseAC + acAbilityBonus + acProfBonus + acAdjustment;

	// Movement
	int currentSpeed = globalChar.getSpeed(currentMovementType);
	int speedAdj = globalChar.getAdj(currentMovementType);
	int finalSpeed = currentSpeed + speedAdj;

	// Clamping for the movement speeds
	if (globalChar.getCondition(conditions::encumbered) == 1) {
		if (finalSpeed < 5 && currentSpeed > 5)
			finalSpeed = 5;

		if (finalSpeed < 0)
			finalSpeed = 0;
	}

	if (finalSpeed < 0) {
		finalSpeed = 0;
	}

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

				if (globalChar.getHP(2) > 0 || globalChar.getCondition(conditions::stabilized) > 0) {
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
				}
				else {
					if (deathAdjustment > 0) { // Positive adjustment
						ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), modFormat(" Death Save", deathAdjustment).c_str());
					}
					else if (deathAdjustment < 0) { // Negative adjustment
						ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), modFormat(" Death Save", deathAdjustment).c_str());
					}
					else { // No adjustment
						ImGui::Text(modFormat(" Death Save", deathAdjustment).c_str());
					}
				}
				
				ImGui::PopItemWidth();
			}
			ImGui::EndChild();
		}

		if (ImGui::BeginChild("##AbilityScores", ImVec2(190, 170), ImGuiChildFlags_Border)) {
			for (int i = 0; i < IM_ARRAYSIZE(mainAbilityList); i++) {
				std::string formattedAbility = mainAbilityList[i];

				if (abilityAdjustments[i] > 0) { // Positive adjustment
					ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), scoreFormat(formattedAbility, (abilityScores)i).c_str());
				}
				else if (abilityAdjustments[i] < 0) { // Negative adjustment
					ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), scoreFormat(formattedAbility, (abilityScores)i).c_str());
				}
				else { // No adjustment
					ImGui::Text(scoreFormat(formattedAbility, (abilityScores)i).c_str());
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
				ImGui::Text("Armor Type");
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
					if (ImGui::BeginCombo("##ArmorTypeProfCombo", proficiencyLevelsList[(int)currentArmorProfLevel])) {
						for (int n = 0; n < IM_ARRAYSIZE(proficiencyLevelsList); n++) {
							const bool selectedArmorProfLevel = (currentArmorProfLevel == (proficiencyLevels)n);

							if (ImGui::Selectable(proficiencyLevelsList[n], selectedArmorProfLevel)) {
								currentArmorProfLevel = (proficiencyLevels)n;
								globalChar.setArmorProficiency(currentArmorType, currentArmorProfLevel);
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::BeginItemTooltip())
					{
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted("Armor Proficiency Level");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					if (ImGui::BeginCombo("##BaseACCombo", baseACList[currentBaseAC])) {
						for (int n = 0; n < IM_ARRAYSIZE(baseACList); n++) {
							const bool selectedBaseAC = (currentBaseAC == n);

							if (ImGui::Selectable(baseACList[n], selectedBaseAC))
								globalChar.setBaseArmorClass(currentArmorType, n);
						}
						ImGui::EndCombo();
					}
					if (ImGui::BeginItemTooltip())
					{
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted("Armor Base AC");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					if (ImGui::BeginCombo("##MainACAbilityCombo", mainAbilityList[(int)globalChar.getArmorMainAbility(currentArmorType)])) {
						for (int n = 0; n < IM_ARRAYSIZE(mainAbilityList); n++) {
							const bool selectedMainAbility = (globalChar.getArmorMainAbility(currentArmorType) == (abilityScores)n);

							if (ImGui::Selectable(mainAbilityList[n], selectedMainAbility))
								globalChar.setArmorMainAbility(currentArmorType, (abilityScores)n);
						}
						ImGui::EndCombo();
					}
					if (ImGui::BeginItemTooltip())
					{
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted("Armor Main Ability");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					ImGui::PopItemWidth();

					ImGui::Separator();
					std::string finalACStr = armorTypesList[(int)currentArmorType];
					finalACStr += " AC";
					if (acAdjustment > 0) { // When the adjustments are positive
						ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), acFormat(finalACStr, currentAC).c_str());
					}
					else if (acAdjustment < 0) { // When the adjustments are negative
						ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), acFormat(finalACStr, currentAC).c_str());
					}
					else { // When the adjustments are zero
						ImGui::Text(acFormat(finalACStr, currentAC).c_str());
					}

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
					ImGui::Text("Movement");
					std::string formattedMovementStr = movementTypesList[(int)currentMovementType];
					formattedMovementStr += ": " + std::to_string(finalSpeed) + " ft.";
					if (ImGui::BeginCombo("##MovementTypeCombo", formattedMovementStr.c_str())) {
						for (int n = 0; n < IM_ARRAYSIZE(movementTypesList); n++) {
							const bool selectedMovementType = (currentMovementType == (movements)n);

							if (ImGui::Selectable(movementTypesList[n], selectedMovementType))
								currentMovementType = (movements)n;
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
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
								applyConditionEffect(currentCondition, condVal);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Remove Condition", ImVec2(125, 0))) {
								applyConditionEffect(currentCondition, 0);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Cancel", ImVec2(73, 0)))
								ImGui::CloseCurrentPopup();
						}
						else {
							if (ImGui::Button("Add Condition", ImVec2(180, 0))) {
								applyConditionEffect(currentCondition, 1);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

							if (ImGui::Button("Remove Condition", ImVec2(180, 0))) {
								applyConditionEffect(currentCondition, 0);
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
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), modFormat("Total Bonus", skillTotalBonus + skillAdjustments).c_str());\
			}
			else if (skillAdjustments < 0) { // When the adjustment is negative
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), modFormat("Total Bonus", skillTotalBonus + skillAdjustments).c_str());
			}
			else { // When the adjustment is zero
				ImGui::Text(modFormat("Total Bonus", skillTotalBonus).c_str());
			}

			if (globalChar.getCondition(conditions::sickened) != 0) {
				dcAdjustment += globalChar.getCondition(conditions::sickened);
			}

			if (dcAdjustment > 0) { // When the adjustment is positive
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "Passive DC:           %2i", skillTotalBonus + dcAdjustment + 10);
			}
			else if (dcAdjustment < 0) { // When the adjustment is negative
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Passive DC:           %2i", skillTotalBonus + dcAdjustment + 10);
			}
			else { // When the adjustment is zero
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
						globalChar.setSkillAbility(currentSkill, (abilityScores)n);
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
						globalChar.setSkillProficiency(currentSkill, currentProficiencyLevel);
					}
				}
				ImGui::EndCombo();
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

		if (ImGui::BeginChild("##MainArea", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
			if (ImGui::BeginTabBar("CharSheetTabBar")) {
				if (ImGui::BeginTabItem("Actions")) {
					if (ImGui::Button("Manage Custom Actions...", ImVec2(0, 0)))
						ImGui::OpenPopup("Action Manager");

					ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::SetNextWindowSize(ImVec2(480, 300));
					if (ImGui::BeginPopupModal("Action Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
						ImGui::Columns(2, "ActionManagerColumns", false);
						ImGui::SetColumnOffset(1, 160);
						{
							if (ImGui::BeginChild("ActionManagerChildCol1", ImVec2(-1, -1))) {
								ImGui::PushItemWidth(-1);
								static char filterActions[20] = "";
								ImGui::InputTextWithHint("##ActionsListFilter", "Filter Actions...", filterActions, IM_ARRAYSIZE(filterActions));
								ImGui::PopItemWidth();
								if (ImGui::BeginListBox("##ActionsListBox", ImVec2(-1, -1))) {
									for (int i = 0; i < globalChar.actions.size(); i++) {
										if (!contains(toLower(filterActions), toLower(globalChar.actions[i].actionName)))
											continue;

										const bool selectedAction = (globalChar.actions[i].actionName == currentAction.actionName);

										if (i == 0) {
											if (ImGui::Selectable("New Action...", selectedAction)) {
												currentAction = globalChar.actions[0];
												strcpy_s(actionNameStr, currentAction.actionName.c_str());
												strcpy_s(actionDescStr, currentAction.actionDescription.c_str());
											}
										}
										else {
											if (ImGui::Selectable(globalChar.actions[i].actionName.c_str(), selectedAction)) {
												currentAction = globalChar.actions[i];
												strcpy_s(actionNameStr, currentAction.actionName.c_str());
												strcpy_s(actionDescStr, currentAction.actionDescription.c_str());
											}
										}
									}
									ImGui::EndListBox();
								}
								ImGui::EndChild();
							}
						}
						ImGui::NextColumn();
						{
							if (ImGui::BeginChild("ActionManagerChildCol2", ImVec2(-1, -1), ImGuiChildFlags_Border)) {
								ImGui::PushItemWidth(-1);
								ImGui::Text("Action Type");
								if (ImGui::BeginCombo("##ActionTypeCombo", actionTypesList[(int)currentAction.actionType])) {
									for (int i = 0; i < IM_ARRAYSIZE(actionTypesList); i++) {
										const bool selectedActionType = (currentAction.actionType == (actionTypes)i);

										if (ImGui::Selectable(actionTypesList[i], selectedActionType))
											currentAction.actionType = (actionTypes)i;
										
									}
									ImGui::EndCombo();
								}
								ImGui::Separator();
								ImGui::Text("Action Name & Description");
								ImGui::InputTextWithHint("##ActionNameInput", "Action Name...", actionNameStr, IM_ARRAYSIZE(actionNameStr));
								ImGui::InputTextMultiline("##ActionDescInput", actionDescStr, IM_ARRAYSIZE(actionDescStr), ImVec2(-1, 136));
								ImGui::Separator();

								if (currentAction.actionName == "") {
									if (actionNameStr[0] == '\0')
										ImGui::BeginDisabled();

									for (int i = 1; i < globalChar.actions.size(); i++) {
										if (actionNameStr == globalChar.actions[i].actionName)
											ImGui::BeginDisabled();
									}

									if (ImGui::Button("Add Action", ImVec2(218, 0))) {
										currentAction.actionName = actionNameStr;
										currentAction.actionDescription = actionDescStr;
										globalChar.createAction(currentAction.actionType, currentAction.actionName, currentAction.actionDescription);
									}
									ImGui::EndDisabled();
								}
								else {
									if (ImGui::Button("Update Action", ImVec2(105, 0))) {
										for (int i = 0; i < globalChar.actions.size(); i++) {
											if (currentAction.actionName == globalChar.actions[i].actionName) {
												currentAction.actionName = actionNameStr;
												currentAction.actionDescription = actionDescStr;
												globalChar.actions[i] = currentAction;
											}
										}
									}
									ImGui::SameLine();
									if (ImGui::Button("Remove Action", ImVec2(105, 0))) {
										for (int i = 1; i < globalChar.actions.size(); i++) { // Skipping the first element to avoid deleting our only way to add actions
											if (currentAction.actionName == globalChar.actions[i].actionName) {
												globalChar.actions.erase(globalChar.actions.begin() + i);
												currentAction.actionType = actionTypes::action;
												currentAction.actionName = "";
												currentAction.actionDescription = "";
												actionNameStr[0] = '\0';
												actionDescStr[0] = '\0';
											}
										}
									}
								}
								ImGui::SameLine();
								if (ImGui::Button("Close", ImVec2(-1, 0)))
									ImGui::CloseCurrentPopup();

								ImGui::PopItemWidth();
								ImGui::EndChild();
							}
						}
						ImGui::EndPopup();
					}

					ImGui::Text("Actions");
					ImGui::Separator();
					if (ImGui::BeginChild("ActionsChild", ImVec2(-1, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
						
						if (globalChar.getActionCount(actionTypes::action) > 0) {
							ImGui::Text("Custom Actions");
							ImGui::Separator();
							for (int i = 1; i < globalChar.actions.size(); i++) { // Starting the array at one to ignore the dummy element in position 0
								if (globalChar.actions[i].actionType == actionTypes::action) {
									ImGui::Text(globalChar.actions[i].actionName.c_str());
									if (ImGui::BeginItemTooltip())
									{
										ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
										ImGui::TextUnformatted(globalChar.actions[i].actionDescription.c_str());
										ImGui::PopTextWrapPos();
										ImGui::EndTooltip();
									}
								}
							}
							ImGui::Spacing();
						}

						ImGui::Text("Basic Actions");
						ImGui::Separator();
						{
							ImGui::Text("Attack,"); ImGui::SameLine();
							beginTooltip("The most common action to take in combat is the Attack action, whether you are swinging a sword, firing an arrow from a bow, or brawling with your fists.\n\nWhen you take this action you may make a number of melee or ranged attacks against a target. The maximum number of attacks you make is governed by how many attacks per action you can take.");
							ImGui::Text("Dash," ); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Delay,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Disengage,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Dodge,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Grapple,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Help,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Hide,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Improvise,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Ready,");
							beginTooltip("");
							ImGui::Text("Search,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Shove,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("and"); ImGui::SameLine();
							ImGui::Text("Use an Object.");
							beginTooltip("");
						}
						ImGui::EndChild();
					}

					ImGui::Text("Bonus Actions");
					ImGui::Separator();
					if (ImGui::BeginChild("BonusActionsChild", ImVec2(-1, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
						if (globalChar.getActionCount(actionTypes::bonusAction) > 0) {
							ImGui::Text("Custom Bonus Actions");
							ImGui::Separator();
							for (int i = 1; i < globalChar.actions.size(); i++) { // Starting the array at one to ignore the dummy element in position 0
								if (globalChar.actions[i].actionType == actionTypes::bonusAction) {
									ImGui::Text(globalChar.actions[i].actionName.c_str());
									beginTooltip(globalChar.actions[i].actionDescription.c_str());
								}
							}
							ImGui::Spacing();
						}

						ImGui::Text("Basic Bonus Actions");
						ImGui::Separator();
						{
							ImGui::Text("Two-Weapon Fighting.");
							beginTooltip("");
						}
						ImGui::EndChild();
					}

					ImGui::Text("Reactions");
					ImGui::Separator();
					if (ImGui::BeginChild("ReactionsChild", ImVec2(-1, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
						if (globalChar.getActionCount(actionTypes::reaction) > 0) {
							ImGui::Text("Custom Reactions");
							ImGui::Separator();
							for (int i = 1; i < globalChar.actions.size(); i++) { // Starting the array at one to ignore the dummy element in position 0
								if (globalChar.actions[i].actionType == actionTypes::reaction) {
									ImGui::Text(globalChar.actions[i].actionName.c_str());
									beginTooltip(globalChar.actions[i].actionDescription.c_str());
								}
							}
							ImGui::Spacing();
						}

						ImGui::Text("Basic Reactions");
						ImGui::Separator();
						{
							ImGui::Text("Opportunity Attack.");
							beginTooltip("");
						}
						ImGui::EndChild();
					}

					ImGui::Text("Other Actions");
					ImGui::Separator();
					if (ImGui::BeginChild("OtherActionsChild", ImVec2(-1, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
						if (globalChar.getActionCount(actionTypes::otherAction) > 0) {
							ImGui::Text("Custom Other Actions");
							ImGui::Separator();
							for (int i = 1; i < globalChar.actions.size(); i++) { // Starting the array at one to ignore the dummy element in position 0
								if (globalChar.actions[i].actionType == actionTypes::otherAction) {
									ImGui::Text(globalChar.actions[i].actionName.c_str());
									beginTooltip(globalChar.actions[i].actionDescription.c_str());
								}
							}
							ImGui::Spacing();
						}

						ImGui::Text("Basic Other Actions");
						ImGui::Separator();
						{

							ImGui::Text("Cast a Spell,"); ImGui::SameLine();
							beginTooltip("");
							ImGui::Text("Interact with an Object.");
							beginTooltip("");
						}
						ImGui::EndChild();
					}

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
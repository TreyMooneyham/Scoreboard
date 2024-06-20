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

// Here's the global character variable
extern playerCharacter globalChar = playerCharacter();

static skill idk;
static skills currentSkill = skills::acrobatics;
static proficiencyLevels currentProficiencyLevel = proficiencyLevels::noProficiency;

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
	"All terrain is difficult.\nPerception checks that rely on sound always fail.\nYou are immune to effects that require sight.\nBlinded overwrites dazed.\nYou have disadvantage on attack rolls.", 
	"Clumsy includes a value.\nYou have a penalty to dexterity-based attack rolls, ability checks, DCs, and saving throws equal to your clumsy value.", 
	"You do not treat anyone as your ally.\nYou cannot Delay, Ready, or take Reactions.\nYou must use your turn to attack random nearby creatures.\nWhenever you take damage, you can attempt to save against the effect.", 
	"You no longer have control of your character.", 
	"All attacks you make have a 25% chance to miss (DC 5 flat check).", 
	"Perception checks that rely on sound automatically fail.\nYou are immune to effects that require hearing.", 
	"Doomed includes a value.\nYou have a penalty to Death Saving Throws equal to your doomed value.\nDoomed decreases by 1 for each long rest you take.", 
	"Drained includes a value.\nYou have a penalty to constitution based attack rolls, ability checks, DCs, and saving throws equal to your drained value.\nYou lose a number of hit points equal to your character level times the drained value.\nYou maximum hit points is also reduced by your character level times the drained value.\nDrained decreases by 1 for each long rest you take.",
	"You receive the dying condition if your hit points are less than 0.\nYou are unconscious.\nAt the end of your turn, you roll a death saving throw. On a roll of 1-10 you fail. After three failures you die.\nIf your hit points are reduced to negative half your maximum hit points, you die.\nIf you receive any healing that does not take you over 0 hit points, you go to 0 hit points.\nIf another creatures makes a medicine check, DC 18, to stabilize you, you stabilize. If that creatures rolls a natural 1, you instead take a failure on your death saves.", 
	"You are clumsy 1.\nYou speeds are reduced by 10 feet.\nThis cannot reduce your speed to less than 5 feet.", 
	"Enfeebled incudes a value.\nYou have a penalty to strength based attack rolls, ability checks, DCs, and saving throws equal to your enfeebled value.", 
	"You have a -1 penalty to your AC and saving throws.\nYou cannot travel quickly, or stealthily, and cannot perform general tasks while traveling.\nYou recover from fatigued after a long rest.", 
	"You must spend your turn getting away from the source of your condition as fast as possible.\nYou cannot Delay or Ready while fleeing.", 
	"Frightened includes a value.\nYou have disadvantage on attack rolls and ability checks against the source of frightened.\nYou cannot willingly move closer the the source of frightened.\nThe frightened value decreases by 1 at the end of your turn.", 
	"You are immobilized.\nAttack rolls against you have advantage.", 
	"You have advantage on attack rolls and checks against creatures that you are hidden from.\nIf a creature targets you with something, it has a 55% chance (DC 11 flat check) to lose that action before being able to make the roll.", 
	"Your speed is zero.", 
	"You are undetected by everyone.\nA creature can make a perception check to find you, which makes you hidden until you succeed on a stealth check to become undetected again.\nIf you become invisible while seen, you are only hidden to that creature until you succeed on a stealth check to become undetected.", 
	"You cannot move.\nYour AC is reduced by 2.\nYou cannot take actions that require movement of any kind.", 
	"You cannot do anything.\nYou have an AC of 9, your hit points stay the same as when you weren't stone, and your weight is quadrupled.\nYou do not age or notice time passing.", 
	"You treat all terrain as difficult.\nYou must spend half your movement to stand up.\nRanged attacks against you are made at disadvantage.\nMelee attacks against you are made at advantage.\nYour non-special attack rolls are made at disadvantage.", 
	"Your speed increases by 10 feet.\nYou gain a +2 bonus to AC.\nYou gain an additional action.\nThis action can only be used to make one attack, dash, dodge, disengage, hide, or use an object.", 
	"Your AC is reduced by 2.\nYou are immobilized.\nYou cannot make attack rolls and can only make attempts to escape.", 
	"Sickened includes a value.\nYou have a penalty to attack rolls and ability checks equal to the value tied to sickened.\nYou cannot willingly ingest anything (Including potions).\nYou can spend an action to throw up, immediately making a fortitude saving throw. On a success you reduce your Sickened level by 1.", 
	"You cannot take reactions while slowed.\nYou can only take an action or a bonus action on your turn.\nYour speed is reduced by 10 feet.", 
	"You do not make death saving throws.\nYou heal 1 hit point every hour.", 
	"Stunned overwrites slowed.\nYou cannot take reactions while stunned.\nYour are immobilized.\nYou cannot take actions or bonus actions.", 
	"Stupefied includes a value.\nYou have a penalty to intelligence, wisdom, and charisma based attack rolls, ability checks, DCs, and saving throws equal to your stupefied value.\nWhenever you cast a spell you must beat a DC 5 + stupefied value flat roll or the spell fizzles.", 
	"You are stunned.\nYou are prone.", 
	"You have a +2 bonus to attack rolls against creatures you are undetected by.\nCreatures can attempt to target you by attacking a square.\nThey must succeed on a DC 11 flat check to make an attack roll against you if you are in that square."
};

void charSheet(bool* enable) {
	// Common variables for the global character
	// Will need to add support for conditions, bonuses, and penalties.
	int strScore = globalChar.getScore(abilityScores::strength);
	int strMod = globalChar.getMod(abilityScores::strength);
	int conScore = globalChar.getScore(abilityScores::constitution);
	int conMod = globalChar.getMod(abilityScores::constitution);
	int dexScore = globalChar.getScore(abilityScores::dexterity);
	int dexMod = globalChar.getMod(abilityScores::dexterity);
	int intScore = globalChar.getScore(abilityScores::intelligence);
	int intMod = globalChar.getMod(abilityScores::intelligence);
	int wisScore = globalChar.getScore(abilityScores::wisdom);
	int wisMod = globalChar.getMod(abilityScores::wisdom);
	int chaScore = globalChar.getScore(abilityScores::charisma);
	int chaMod = globalChar.getMod(abilityScores::charisma);

	int fortScoreBonus = globalChar.getSave(savingThrows::fortitude);
	int refScoreBonus = globalChar.getSave(savingThrows::reflex);
	int willScoreBonus = globalChar.getSave(savingThrows::will);
	int fortProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::fortitude));
	int refProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::reflex));
	int willProfBonus = globalChar.getProfBonus(globalChar.getSaveProficiency(savingThrows::will));
	int fortTotalBonus = fortScoreBonus + fortProfBonus;
	int refTotalBonus = refScoreBonus + refProfBonus;
	int willTotalBonus = willScoreBonus + willProfBonus;
	//int fortAdjustments = globalChar.getAdj(savingThrows::fortitude);
	//int refAdjustments = globalChar.getAdj(savingThrows::reflex);
	//int willAdjustments = globalChar.getAdj(savingThrows::will);

	int skillScoreBonus = globalChar.getMod(globalChar.skillInfo[currentSkill].mainAbility);
	int skillProfBonus = globalChar.getProfBonus(globalChar.skillInfo[currentSkill].profLevel);
	int skillTotalBonus = skillScoreBonus + skillProfBonus;
	int skillAdjustments = globalChar.getAdj(currentSkill);

	int maxHP = globalChar.getHP(0) + (globalChar.getLevel(levels::character) * globalChar.getMod(abilityScores::constitution));

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
				//strcpy_s(pNameChar, globalChar.getName(0).c_str());
				if (ImGui::InputTextWithHint("##ancestryBox", "Ancestry...", ancestryChar, IM_ARRAYSIZE(ancestryChar))) {
					std::string ancestryStr = ancestryChar;
					//globalChar.setName(pNameStr, 0);
				}

				//strcpy_s(pNameChar, globalChar.getName(0).c_str());
				if (ImGui::InputTextWithHint("##nationBox", "Nationality...", nationalityChar, IM_ARRAYSIZE(nationalityChar))) {
					std::string nationaityStr = nationalityChar;
					//globalChar.setName(pNameStr, 0);
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

					ImGui::Text(modFormat(formattedAbility, globalChar.getMod((abilityScores)i)).c_str());
				}

				ImGui::Separator();

				for (int i = 0; i < IM_ARRAYSIZE(savesList); i++) {
					std::string formattedSave = savesList[i];

					switch (i) {
					case 0:
						(strMod >= conMod) ? ImGui::Text(modFormat(formattedSave, strMod).c_str()) : ImGui::Text(modFormat(formattedSave, conMod).c_str());
						break;
					case 1:
						(dexMod >= intMod) ? ImGui::Text(modFormat(formattedSave, dexMod).c_str()) : ImGui::Text(modFormat(formattedSave, intMod).c_str());
						break;
					case 2:
						(wisMod >= chaMod) ? ImGui::Text(modFormat(formattedSave, wisMod).c_str()) : ImGui::Text(modFormat(formattedSave, chaMod).c_str());
						break;
					}
				}
				ImGui::EndChild();
			}

		ImGui::SameLine();

		if (ImGui::BeginChild("TestZone", ImVec2(-1, 170), ImGuiChildFlags_Border)) {

			ImGui::EndChild();
		}

		if (ImGui::BeginChild("##SkillsOutput", ImVec2(190, -1), ImGuiChildFlags_Border)) {
				
			ImGui::Text(modFormat("Ability Bonus", skillScoreBonus).c_str());
			ImGui::Text(modFormat("Proficiency Bonus", skillProfBonus).c_str());
			ImGui::Text(modFormat("Misc. Bonus", skillAdjustments).c_str());

			ImGui::Separator();

			if (skillTotalBonus == (skillTotalBonus + skillAdjustments)) {
				ImGui::Text(modFormat("Total Bonus", skillTotalBonus).c_str());
				ImGui::Text("Passive DC:           %2i", skillTotalBonus + 10);
			}

			if (skillTotalBonus > (skillTotalBonus + skillAdjustments)) {
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), modFormat("Total Bonus", skillTotalBonus + skillAdjustments).c_str());
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Passive DC:           %2i", skillTotalBonus + skillAdjustments + 10);
			}

			if (skillTotalBonus < (skillTotalBonus + skillAdjustments)) {
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), modFormat("Total Bonus", skillTotalBonus + skillAdjustments).c_str());
				ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "Passive DC:           %2i", skillTotalBonus + skillAdjustments + 10);
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

			// Add this back in the level up menu, whenever we get around to that.
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
			ImGui::Text("2nd Column");

			ImGui::EndChild();
		}

		/*ImGui::TableNextColumn();

		if (ImGui::BeginChild("##Col3Child1", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
			ImGui::Text("3rd Column");

			ImGui::EndChild();
		}

		ImGui::TableNextColumn();

		if (ImGui::BeginChild("##Col4Child1", ImVec2(-1, 190), ImGuiChildFlags_Border)) {
			ImGui::Text("4th Column");

			ImGui::EndChild();
		}*/
	}
	ImGui::End();
}
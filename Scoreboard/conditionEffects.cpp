// Includes
#include "conditionEffects.h"

void checkConditions() {
	for (int i = 0; i < 30; i++) {
		if (globalChar.getCondition((conditions)i) > 0)
			applyConditionEffect((conditions)i, globalChar.getCondition((conditions)i));
	}
}

void applyConditionEffect(conditions cond, int val) {
	int curVal = globalChar.getCondition(cond);
	int valDelta = curVal - val;

	globalChar.setCondition(cond, val); // Sets our condition before going farther.

	switch (cond) {
	case conditions::clumsy:
		// This isn't implemented yet.
		//int curAttackAdj = globalChar.getAdj(/* Logic for dexterity based attack rolls */);
		for (int i = 0; i < 19; i++) { // Searches through the first 19 skills. The ones we care about.
			if (globalChar.getSkillAbility((skills)i) == abilityScores::dexterity) { // Checks if those skills' primary ability is dexterity
				globalChar.addAdj((skills)i, valDelta);
			}
		}
		globalChar.addAdj(savingThrows::reflex, valDelta);
		break;
	case conditions::doomed:
		globalChar.addAdj(savingThrows::death, valDelta);
		break;
	case conditions::drained:
		// This isn't implemented yet.
		//int curAttackAdj = globalChar.getAdj(/* Logic for dexterity based attack rolls */);
		for (int i = 0; i < 19; i++) { // Searches through the first 19 skills. The ones we care about.
			if (globalChar.getSkillAbility((skills)i) == abilityScores::constitution) { // Checks if those skills' primary ability is dexterity
				globalChar.addAdj((skills)i, valDelta);
			}
		}
		globalChar.addAdj(savingThrows::fortitude, valDelta);

		if (valDelta < 0)
			globalChar.hpInfo.currentHP += valDelta;
		globalChar.hpInfo.rolledHP += valDelta;
		break;
	case conditions::encumbered:
		if (globalChar.getCondition(conditions::clumsy) < 1 && valDelta < 0)
			applyConditionEffect(conditions::clumsy, 1);

		for (int i = 0; i < 5; i++) {
			globalChar.addAdj((movements)i, valDelta*10);
		}
		break;
	case conditions::enfeebled:
		// This isn't implemented yet.
		//int curAttackAdj = globalChar.getAdj(/* Logic for dexterity based attack rolls */);
		for (int i = 0; i < 19; i++) { // Searches through the first 19 skills. The ones we care about.
			if (globalChar.getSkillAbility((skills)i) == abilityScores::strength) { // Checks if those skills' primary ability is dexterity
				globalChar.addAdj((skills)i, valDelta);
			}
		}
		break;
	case conditions::fatigued:
		for (int i = 0; i < 4; i++) {
			globalChar.addAdj((savingThrows)i, valDelta);
			globalChar.addAdj((armorTypes)i, valDelta);
		}
		break;
	default:
		return;
	}
}

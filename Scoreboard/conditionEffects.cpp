// Includes
#include "conditionEffects.h"

void applyConditionEffect(conditions cond, int val) {
	int curVal = globalChar.getCondition(cond);
	int valDelta = curVal - val;

	if (valDelta == 0) // Early return to avoid the overhead of the stuff below.
		return;

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
	default:
		return;
	}
}

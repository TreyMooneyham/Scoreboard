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
			if (globalChar.getSkillAbility((skills)i) == abilityScores::constitution) { // Checks if those skills' primary ability is constitution
				globalChar.addAdj((skills)i, valDelta);
			}
		}
		globalChar.addAdj(savingThrows::fortitude, valDelta);

		if (valDelta < 0)
			globalChar.hpInfo.currentHP += valDelta;
		globalChar.hpInfo.rolledHP += valDelta;
		break;
	case conditions::encumbered:
		if (globalChar.getCondition(conditions::clumsy) == 0 && valDelta < 0)
			applyConditionEffect(conditions::clumsy, 1);

		if (globalChar.getCondition(conditions::clumsy) == 1 && valDelta > 0)
			applyConditionEffect(conditions::clumsy, 0);

		for (int i = 0; i < 5; i++) {
			globalChar.addAdj((movements)i, valDelta*10);
		}
		break;
	case conditions::enfeebled:
		// This isn't implemented yet.
		//int curAttackAdj = globalChar.getAdj(/* Logic for dexterity based attack rolls */);
		for (int i = 0; i < 19; i++) { // Searches through the first 19 skills. The ones we care about.
			if (globalChar.getSkillAbility((skills)i) == abilityScores::strength) { // Checks if those skills' primary ability is strength
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
	case conditions::immobilized:
		for (int i = 0; i < 4; i++) {
			int speedPenalty = valDelta * globalChar.getSpeed((movements)i);
			globalChar.addAdj((movements)i, speedPenalty);
		}
		break;
	case conditions::paralyzed:
		for (int i = 0; i < 4; i++) {
			globalChar.addAdj((armorTypes)i, 2 * valDelta);
			int speedPenalty = valDelta * globalChar.getSpeed((movements)i);
			globalChar.addAdj((movements)i, speedPenalty);
		}
		break;
	case conditions::quickened:
		for (int i = 0; i < 4; i++) {
			globalChar.addAdj((movements)i, valDelta * -10); // The system was designed penalties in mind, so I multiply them by the negative of the bonus
			globalChar.addAdj((armorTypes)i, valDelta * -2);
		}
		break;
	case conditions::restrained:
		if (globalChar.getCondition(conditions::immobilized) == 0 && valDelta < 0)
			applyConditionEffect(conditions::immobilized, 1);

		if (globalChar.getCondition(conditions::immobilized) == 1 && valDelta > 0)
			applyConditionEffect(conditions::immobilized, 0);

		for (int i = 0; i < 4; i++) {
			globalChar.addAdj((armorTypes)i, valDelta * 2);
		}
		break;
	case conditions::sickened:
		// Logic for attack rolls
		//asjdbhakdjgnsodga idfk man we haven't made it yet
		for (int i = 0; i < 19; i++) {
			globalChar.addAdj((skills)i, valDelta);
		}
		break;
	case conditions::slowed:
		for (int i = 0; i < 4; i++) {
			globalChar.addAdj((movements)i, valDelta * 10);
		}
		break;
	case conditions::stunned:
		if (globalChar.getCondition(conditions::slowed) == 0 && valDelta < 0)
			applyConditionEffect(conditions::slowed, 1);
		if (globalChar.getCondition(conditions::immobilized) == 0 && valDelta < 0)
			applyConditionEffect(conditions::immobilized, 1);

		if (globalChar.getCondition(conditions::slowed) == 1 && valDelta > 0)
			applyConditionEffect(conditions::slowed, 0);
		if (globalChar.getCondition(conditions::immobilized) == 1 && valDelta > 0)
			applyConditionEffect(conditions::immobilized, 0);
		break;
	case conditions::stupefied:
		// This isn't implemented yet.
		//int curAttackAdj = globalChar.getAdj(/* Logic for dexterity based attack rolls */);
		for (int i = 0; i < 19; i++) { // Searches through the first 19 skills. The ones we care about.
			if (globalChar.getSkillAbility((skills)i) == abilityScores::intelligence || 
				globalChar.getSkillAbility((skills)i) == abilityScores::wisdom ||
				globalChar.getSkillAbility((skills)i) == abilityScores::charisma) { // Checks if those skills' primary ability is int, wis, or cha
				globalChar.addAdj((skills)i, valDelta);
			}
		}
		globalChar.addAdj(savingThrows::will, valDelta);
		break;
	case conditions::unconscious:
		if (globalChar.getCondition(conditions::stunned) == 0 && valDelta < 0)
			applyConditionEffect(conditions::stunned, 1);
		if (globalChar.getCondition(conditions::prone) == 0 && valDelta < 0)
			applyConditionEffect(conditions::prone, 1);

		if (globalChar.getCondition(conditions::stunned) == 1 && valDelta > 0)
			applyConditionEffect(conditions::stunned, 0);
		if (globalChar.getCondition(conditions::prone) == 1 && valDelta > 0)
			applyConditionEffect(conditions::prone, 0);
		break;
	default:
		return;
	}
}

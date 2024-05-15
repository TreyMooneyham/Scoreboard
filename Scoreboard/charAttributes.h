#include "settings.h"

struct playerCharacter {
	
};

// General Character Info
enum class charInfo {
	characterName,
	playerName
};

// Levels
enum class levels {
	character,		// Character level
	warrior,		// Warrior, spellcaster, and rogue "level" is just the amount of features from their respective section
	spellcaster,
	rogue
};

// Ability Scores
enum class abilityScores {
	strength, dexterity, constitution,	// Physical Abilities
	intelligence, wisdom, charisma		// Mental Abilities
};

// Proficiencies
enum class proficiencies {
	noProficiency,	// No bonus
	proficiency,	// Bonus is 2 + level
	expertise,		// Bonus is 4 + level
	mastery,		// Bonus is 6 + level
	legendary		// Bonus is 8 + level
};

// Saving Throws
enum class savingThrows {
	fortitude,	// Strength or Constitution
	reflex,		// Dexterity or Intelligence
	will		// Wisdom or Charisma
};

// Skills
enum class Skills {
	acrobatics, animalHandling, arcana,
	athletics, crafting, deception,
	history, initiative, insight,
	intimidation, investigation,
	medicine, nature, perception,
	performance, persuasion, religion,
	sleightOfHand, stealth, survival
};

// Defensive Attributes
enum class defense {
	armorClass,
	damageThreshold,
	damageResistance
};

// Movement
enum class movementTypes {
	climbing,
	burrowing,
	flying,
	swimming,
	walking
};
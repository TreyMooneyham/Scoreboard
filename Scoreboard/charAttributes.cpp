#include <string>
#include "charAttributes.h"

// Sets the player name for a given character
static void setPName(playerCharacter c, std::string name) {
	c.playerName = name;
}

// Gets the player name for a given character
std::string getPName(playerCharacter c) {
	return c.playerName;
}

// Sets the character name for a given character
void setCName(playerCharacter c, std::string name) {
	c.charName = name;
}

// Gets the character name for a given character
std::string getCName(playerCharacter c) {
	return c.charName;
}

// Initiatilizes every score to 7, as if creating a new character
void initScores(playerCharacter c) {
	c.abilities[abilityScores::strength]		= 7;
	c.abilities[abilityScores::dexterity]		= 7;
	c.abilities[abilityScores::constitution]	= 7;
	c.abilities[abilityScores::intelligence]	= 7;
	c.abilities[abilityScores::wisdom]			= 7;
	c.abilities[abilityScores::charisma]		= 7;
}

// Sets a specific score to a specific value
// Values are clamped with a range of 0 to 30, and prevents it from changing if the input is outside of that range
void setScore(playerCharacter c, abilityScores ability, int score) {
	if (score < 0 || score > 30)
		score = c.abilities[ability];

	c.abilities[ability] = score;
}

// Gets a specific ability score
int getScore(playerCharacter c, abilityScores ability) {
	return c.abilities[ability];
}
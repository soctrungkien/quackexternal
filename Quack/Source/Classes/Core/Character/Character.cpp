#include "Character.hpp"

std::string CharacterClass::RigType() {
	if (FindFirstChild("UpperTorso").IsValid()) {
		return "R15";
	}
	else if (FindFirstChild("Torso").IsValid()) {
		return "R6";
	}
	return "R6";
}
// Function: BIOS_GetCharacter
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetCharacter
// One character input (waiting). Wrapper for CHGET routine.
inline c8 BIOS_GetCharacter() { return CallToA(R_CHGET); }

// Function: BIOS_ClearSprites
// Header: bios.h
// Source: bios.c

// Function: BIOS_ClearSprites
// Initialises all sprites.
inline void BIOS_ClearSprites() { Call(R_CLRSPR); }

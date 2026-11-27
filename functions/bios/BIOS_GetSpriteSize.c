// Function: BIOS_GetSpriteSize
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetSpriteSize
// Returns current sprite size in bytes (8 or 32). Wrapper for GSPSIZ routine.
inline u8 BIOS_GetSpriteSize() { return ((u8(*)(void))R_GSPSIZ)(); }

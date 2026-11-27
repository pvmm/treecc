// Function: BIOS_SetScreen1
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetScreen1
// Switches VDP in SCREEN 1 mode. Wrapper for SETT32 routine.
inline void BIOS_SetScreen1() { Call(R_SETT32); }

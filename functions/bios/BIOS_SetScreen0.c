// Function: BIOS_SetScreen0
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetScreen0
// Switches VDP in SCREEN 0 mode. Wrapper for SETTXT routine.
inline void BIOS_SetScreen0() { Call(R_SETTXT); }

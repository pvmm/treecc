// Function: BIOS_SetScreenMode
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetScreenMode
// Switches to given screen mode. Wrapper for CHGMOD routine.
inline void BIOS_SetScreenMode(u8 screen) { CallA(R_CHGMOD, screen); }

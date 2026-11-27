// Function: BIOS_InitScreen1
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen1
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void BIOS_InitScreen1() { Call(R_INIT32); }

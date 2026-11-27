// Function: BIOS_InitScreen0
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen0
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void BIOS_InitScreen0() { Call(R_INITXT); }

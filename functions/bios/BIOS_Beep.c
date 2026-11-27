// Function: BIOS_Beep
// Header: bios.h
// Source: bios.c

// Function: BIOS_Beep
// Generates beep. Wrapper for BEEP routine.
inline void BIOS_Beep() { Call(R_BEEP); }

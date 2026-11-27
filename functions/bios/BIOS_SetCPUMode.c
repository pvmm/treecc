// Function: BIOS_SetCPUMode
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetCPUMode
// Changes CPU mode. Wrapper for CHGCPU routine.
inline void BIOS_SetCPUMode(u8 mode) { ((void(*)(u8))R_CHGCPU)(mode); }

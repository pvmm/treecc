// Function: BIOS_SetScreen3
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetScreen3
// Switches VDP to SCREEN 3 mode. Wrapper for SETMLT routine.
inline void BIOS_SetScreen3() { Call(R_SETMLT); }

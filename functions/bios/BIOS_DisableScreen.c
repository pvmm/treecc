// Function: BIOS_DisableScreen
// Header: bios.h
// Source: bios.c

// Function: BIOS_DisableScreen
// Inhibits the screen display. Wrapper for DISSCR routine.
inline void BIOS_DisableScreen() { Call(R_DISSCR); }

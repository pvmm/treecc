// Function: BIOS_DisplayScreen
// Header: bios.h
// Source: bios.c

// Function: BIOS_EnableScreen
// Display or disable the screen.
inline void BIOS_DisplayScreen(bool enable) { if (enable) BIOS_EnableScreen(); else BIOS_DisableScreen(); }

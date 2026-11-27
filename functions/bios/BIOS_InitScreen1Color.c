// Function: BIOS_InitScreen1Color
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen1Color
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void BIOS_InitScreen1Color(u8 width, u8 text, u8 back, u8 border) { g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen1(); }

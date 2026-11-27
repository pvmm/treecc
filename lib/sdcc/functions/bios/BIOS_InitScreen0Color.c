// Function: BIOS_InitScreen0Color
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen0Color
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void BIOS_InitScreen0Color(u8 width, u8 text, u8 back, u8 border) { g_LINL40 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen0(); }

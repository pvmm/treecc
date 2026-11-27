// Function: BIOS_InitScreen0Ex
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen0Ex
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void BIOS_InitScreen0Ex(u16 pnt, u16 pgt, u8 width, u8 text, u8 back, u8 border) { g_TXTNAM = pnt; g_TXTCGP = pgt; g_LINL40 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen0(); }

// Function: BIOS_InitScreen1Ex
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen1Ex
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void BIOS_InitScreen1Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 width, u8 text, u8 back, u8 border) { g_T32NAM = pnt; g_T32COL = ct; g_T32CGP = pgt; g_T32ATR = sat; g_T32PAT = sgt; g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen1(); }

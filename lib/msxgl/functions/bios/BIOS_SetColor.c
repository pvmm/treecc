// Function: BIOS_SetColor
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetColor
// Set screen colors.
inline void BIOS_SetColor(u8 text, u8 back, u8 border) { g_FORCLR = text; g_BAKCLR = back; g_BDRCLR = border; }

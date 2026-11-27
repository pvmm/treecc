// Function: BIOS_ApplyColor
// Header: bios.h
// Source: bios.c

// Function: BIOS_ApplyColor
// Changes the screen colors. Wrapper for CHGCLR routine.
inline void BIOS_ApplyColor(u8 text, u8 back, u8 border) { BIOS_SetColor(text, back, border); Call(R_CHGCLR); }

// Function: BIOS_ApplyBorder
// Header: bios.h
// Source: bios.c

// Function: BIOS_ApplyBorder
// Changes the screen colors. Wrapper for CHGCLR routine.
inline void BIOS_ApplyBorder(u8 border) { g_BDRCLR = border; Call(R_CHGCLR); }

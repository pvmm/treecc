// Function: BIOS_TextPrintChar
// Header: bios.h
// Source: bios.c

// Function: BIOS_TextPrintChar
// Displays one character. Wrapper for CHPUT routine.
inline void BIOS_TextPrintChar(c8 chr) { CallA(R_CHPUT, chr); }

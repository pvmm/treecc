// Function: BIOS_TextPrintAt
// Header: bios.h
// Source: bios.c

// Function: BIOS_TextPrintAt
// Print a text at the specified position
inline void BIOS_TextPrintAt(u8 x, u8 y, const c8* str) { BIOS_TextSetCursor(x, y); BIOS_TextPrint(str); }

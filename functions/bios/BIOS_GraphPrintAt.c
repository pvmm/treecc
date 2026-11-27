// Function: BIOS_GraphPrintAt
// Header: bios.h
// Source: bios.c

// Function: BIOS_GraphPrintAt
// Displays a string on the graphic screen. Wrapper for GRPPRT routine.
inline void BIOS_GraphPrintAt(u16 x, u16 y, const c8* str) { BIOS_GraphSetCursor(x, y); BIOS_GraphPrint(str); }

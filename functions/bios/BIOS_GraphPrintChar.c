// Function: BIOS_GraphPrintChar
// Header: bios.h
// Source: bios.c

// Function: BIOS_GraphPrintChar
// Displays a character on the graphic screen. Wrapper for GRPPRT routine.
inline void BIOS_GraphPrintChar(u8 chr) { CallA(R_GRPPRT, chr); }

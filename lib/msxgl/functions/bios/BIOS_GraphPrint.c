// Function: BIOS_GraphPrint
// Header: bios.h
// Source: bios.c

// Function: BIOS_GraphPrint
// Displays a string on the graphic screen. Wrapper for GRPPRT routine.
inline void BIOS_GraphPrint(const c8* str) { while (*str) BIOS_GraphPrintChar(*str++); }

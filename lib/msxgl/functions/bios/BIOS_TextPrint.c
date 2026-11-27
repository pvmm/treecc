// Function: BIOS_TextPrint
// Header: bios.h
// Source: bios.c

// Function: BIOS_TextPrint
// Displays a null-terminated string.
inline void BIOS_TextPrint(const c8* str) { while (*str) BIOS_TextPrintChar(*str++); }

// Function: BIOS_GetFontAddress
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetFontAddress
// Base address of the MSX character set in ROM. Wrapper for CALCGT variable.
inline u16 BIOS_GetFontAddress() { return g_CGTABL; }

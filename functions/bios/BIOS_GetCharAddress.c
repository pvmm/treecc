// Function: BIOS_GetCharAddress
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetCharAddress
// Base address of a given character in ROM. Wrapper for CALCGT variable.
inline u16 BIOS_GetCharAddress(u8 chr) { return g_CGTABL + (chr * 8); }

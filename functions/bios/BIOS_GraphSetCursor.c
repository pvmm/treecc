// Function: BIOS_GraphSetCursor
// Header: bios.h
// Source: bios.c

// Function: BIOS_GraphSetCursor
// Moves graphical cursor to the specified position.
inline void BIOS_GraphSetCursor(u16 x, u16 y) { g_GRPACX = x; g_GRPACY = y; }

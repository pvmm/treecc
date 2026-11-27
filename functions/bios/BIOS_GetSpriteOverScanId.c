// Function: BIOS_GetSpriteOverScanId
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetSpriteOverScanId
// Returns index of the over-scaned sprite (5th/8th sprite on line). Value is in 0-31 range.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL).
inline u8 BIOS_GetSpriteOverScanId() { return S00_GET_SN(g_STATFL); }

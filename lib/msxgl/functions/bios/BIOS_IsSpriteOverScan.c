// Function: BIOS_IsSpriteOverScan
// Header: bios.h
// Source: bios.c

// Function: BIOS_IsSpriteOverScan
// Returns FALSE if no over-scan occured during the previous frame (more than 4/8 sprites on the same line), otherwise returns S00_5S.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL).
inline bool BIOS_IsSpriteOverScan() { return g_STATFL & S00_5S; }

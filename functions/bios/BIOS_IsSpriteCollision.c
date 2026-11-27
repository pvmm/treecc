// Function: BIOS_IsSpriteCollision
// Header: bios.h
// Source: bios.c

// Function: BIOS_IsSpriteCollision
// Returns FALSE if no collision occured during the previous frame, otherwise returns S00_C.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL).
inline bool BIOS_IsSpriteCollision() { return g_STATFL & S00_C; }

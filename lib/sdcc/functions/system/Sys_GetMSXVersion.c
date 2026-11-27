// Function: Sys_GetMSXVersion
// Header: system.h
// Source: system.c

// Function: Sys_GetMSXVersion
// Get the MSX version
//
// Return:
//   The MSX version (0: MSX1, 1: MSX2, 2: MSX2+, 3: MSX turbo R)
inline u8 Sys_GetMSXVersion() { return g_VersionMSX; }

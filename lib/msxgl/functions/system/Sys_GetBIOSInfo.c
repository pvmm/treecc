// Function: Sys_GetBIOSInfo
// Header: system.h
// Source: system.c

// Function: Sys_GetBIOSInfo
// Get BIOS information (frequency, keyboard type, etc.)
//
// Return:
//   The BIOS information
inline u8 Sys_GetBIOSInfo() { return g_VersionROM; }

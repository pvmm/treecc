// Function: Sys_GetHeaderAddr
// Header: system.h
// Source: system.c

// Function: Sys_GetHeaderAddr
// Get address of program header (if any)
//
// Return:
//   The address of the program header
inline u16 Sys_GetHeaderAddr() { return (u16)&g_HeaderAddr; }

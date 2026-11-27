// Function: Sys_GetFirstAddr
// Header: system.h
// Source: system.c

// Function: Sys_GetFirstAddr
// Get first address of program binary
//
// Return:
//   The first address of the program binary
inline u16 Sys_GetFirstAddr() { return (u16)&g_FirstAddr; }

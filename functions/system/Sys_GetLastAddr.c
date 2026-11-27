// Function: Sys_GetLastAddr
// Header: system.h
// Source: system.c

// Function: Sys_GetLastAddr
// Get last address of program binary
//
// Return:
//   The last address of the program binary
inline u16 Sys_GetLastAddr()  { return (u16)&g_LastAddr; }

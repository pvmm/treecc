// Function: Sys_Is50Hz
// Header: system.h
// Source: system.c

// Function: Sys_Is50Hz
// Get the system frequency (from BIOS)
//
// Return:
//   TRUE if the BIOS is 50 Hz
inline bool Sys_Is50Hz() { return Sys_GetFrequency() == SYS_FREQ_50HZ; }

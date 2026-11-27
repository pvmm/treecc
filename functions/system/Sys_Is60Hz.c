// Function: Sys_Is60Hz
// Header: system.h
// Source: system.c

// Function: Sys_Is60Hz
// Get the system frequency (from BIOS)
//
// Return:
//   TRUE if the BIOS is 60 Hz
inline bool Sys_Is60Hz() { return Sys_GetFrequency() == SYS_FREQ_60HZ; }

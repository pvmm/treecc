// Function: Sys_GetFrequency
// Header: system.h
// Source: system.c

// Function: Sys_GetFrequency
// Get the system frequency (from BIOS)
//
// Return:
//   SYS_FERQ_50HZ or SYS_FERQ_60HZ
inline u8 Sys_GetFrequency() { return Sys_GetBIOSInfo() & 0x80; }

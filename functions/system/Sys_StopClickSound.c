// Function: Sys_StopClickSound
// Header: system.h
// Source: system.c

// Function: Sys_StopClickSound
// Stop the click sound
inline void Sys_StopClickSound() { io_PPI_KBAccess &= 0x7F; }

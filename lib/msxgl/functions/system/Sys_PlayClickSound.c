// Function: Sys_PlayClickSound
// Header: system.h
// Source: system.c

// Function: Sys_PlayClickSound
// Play the click sound
inline void Sys_PlayClickSound() { io_PPI_KBAccess |= 0x80; }

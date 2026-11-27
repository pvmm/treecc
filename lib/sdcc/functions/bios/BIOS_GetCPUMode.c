// Function: BIOS_GetCPUMode
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetCPUMode
// Returns current CPU mode. Wrapper for GETCPU routine.
inline u8 BIOS_GetCPUMode() { return ((u8(*)(void))R_GETCPU)(); }

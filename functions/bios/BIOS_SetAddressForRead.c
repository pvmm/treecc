// Function: BIOS_SetAddressForRead
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetAddressForRead
// Enables VDP to read. Wrapper for SETRD routine.
inline void BIOS_SetAddressForRead(u16 addr) { CallHL(R_SETRD, addr); }

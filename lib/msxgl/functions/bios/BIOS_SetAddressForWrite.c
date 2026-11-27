// Function: BIOS_SetAddressForWrite
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetAddressForWrite
// Enables VDP to write. Wrapper for SETWRT routine.
inline void BIOS_SetAddressForWrite(u16 addr) { CallHL(R_SETWRT, addr); }

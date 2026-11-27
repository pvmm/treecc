// Function: BIOS_ReadVRAM
// Header: bios.h
// Source: bios.c

// Function: BIOS_ReadVRAM
// Reads the content of VRAM. Wrapper for RDVRM routine.
inline u8 BIOS_ReadVRAM(u16 addr) { return CallHL(R_RDVRM, addr); }

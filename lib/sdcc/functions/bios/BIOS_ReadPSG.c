// Function: BIOS_ReadPSG
// Header: bios.h
// Source: bios.c

// Function: BIOS_ReadPSG
// Reads value from PSG-register. Wrapper for RDPSG routine.
//> RDPSG
//> Address  : #0096
//> Function : Reads value from PSG register
//> Input    : A  - PSG register read
//> Output   : A  - Value read
//
// Parameters:
//   reg - PSG register number (0-15)
//
// Return:
//   Value read from the PSG register
inline u8 BIOS_ReadPSG(u8 reg) { return ((u8(*)(u8))R_RDPSG)(reg); }

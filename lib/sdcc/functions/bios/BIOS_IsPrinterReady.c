// Function: BIOS_IsPrinterReady
// Header: bios.h
// Source: bios.c

// Function: BIOS_IsPrinterReady
// Tests printer status. Wrapper for LPTSTT routine.
//
// Return:
//   FALSE if printer is not ready
inline bool BIOS_IsPrinterReady() { return CallToA(R_LPTSTT); }

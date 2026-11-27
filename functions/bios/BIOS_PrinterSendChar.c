// Function: BIOS_PrinterSendChar
// Header: bios.h
// Source: bios.c

// Function: BIOS_PrinterSendChar
// Sends one character to printer. Wrapper for LPTOUT routine.
//
// Parameters:
//   chr - ASCII code of character to send
inline void BIOS_PrinterSendChar(u8 chr) { CallA(R_LPTOUT, chr); }

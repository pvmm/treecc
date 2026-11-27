// Function: BIOS_PrinterSendString
// Header: bios.h
// Source: bios.c

// Function: BIOS_PrinterSendString
// Sends a null-terminated string to the printer. Wrapper for LPTOUT routine.
//
// Parameters:
//   str - Pointer to the string to send
inline void BIOS_PrinterSendString(const u8* str) { while (*str) BIOS_PrinterSendChar(*str++); }

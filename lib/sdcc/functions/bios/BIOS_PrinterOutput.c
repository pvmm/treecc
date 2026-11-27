// Function: BIOS_PrinterOutput
// Header: bios.h
// Source: bios.c

// Function: BIOS_PrinterOutput
// Sends one character to printer if printer is ready. Wrapper for OUTDLP routine.
//
// Parameters:
//   chr - ASCII code of character to send
inline void BIOS_PrinterOutput(u8 chr) { CallA(R_OUTDLP, chr); }

// Function: BIOS_PrinterChangePage
// Header: bios.h
// Source: bios.c

// Function: BIOS_PrinterChangePage
// Send a change page code to the printer
inline void BIOS_PrinterChangePage() { BIOS_PrinterSendChar(ASCII_FORM_FEED); }

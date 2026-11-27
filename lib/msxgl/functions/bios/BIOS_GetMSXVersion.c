// Function: BIOS_GetMSXVersion
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetMSXVersion
// Get MSX generation version
//
// Return:
// - MSXVER_1 (0) for MSX1.
// - MSXVER_2 (1) for MSX2.
// - MSXVER_2P (2) for MSX2+.
// - MSXVER_TR (3) for MSX turbo R.
inline u8 BIOS_GetMSXVersion() { return g_MSXVER; }

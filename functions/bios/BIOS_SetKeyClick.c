// Function: BIOS_SetKeyClick
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetKeyClick
// Enable or disable key click
//
// Parameters:
//   enable - TRUE to enable and FALSE to disable
inline void BIOS_SetKeyClick(bool enable) { g_CLIKSW = enable; }

// Function: BIOS_GetVDPReadPort
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetVDPReadPort
// Base port address for VDP data read. Wrapper for R_VDP_DR variable.
inline u8 BIOS_GetVDPReadPort() { return g_VDP_DR; }

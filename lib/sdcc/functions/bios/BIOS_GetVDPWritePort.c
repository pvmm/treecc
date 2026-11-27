// Function: BIOS_GetVDPWritePort
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetVDPWritePort
// Base port address for VDP data write. Wrapper for R_VDP_DW variable.
inline u8 BIOS_GetVDPWritePort() { return g_VDP_DW; }

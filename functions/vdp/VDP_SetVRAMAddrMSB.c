// Function: VDP_SetVRAMAddrMSB
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetVRAMAddrMSB
// Reset the VRAM address most-significant bits (R#14). [MSX2/2+/TR]
inline void VDP_ResetVRAMAddrMSB () { VDP_RegWrite(14, 0); }

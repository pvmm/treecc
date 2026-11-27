// Function: VDP_ExpendCommand
// Header: vdp.h
// Source: vdp.c

// Function: VDP_ExpendCommand
// Enables the VDP commands for screens 0 to 4 (R#25). [MSX2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_ExpendCommand(u8 enable) { VDP_RegWriteBakMask(25, (u8)~R25_CMD, enable ? R25_CMD : 0); }

// Function: VDP_Peek_128K
// Header: vdp.h
// Source: vdp.c

	// Function: VDP_Peek_128K
	// Read a value from VRAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
	//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
	//
	// Return:
	//   Value read in VRAM
	u8 VDP_Peek_128K(u16 srcLow, u8 srcHigh);

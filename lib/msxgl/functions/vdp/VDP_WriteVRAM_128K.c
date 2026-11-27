// Function: VDP_WriteVRAM_128K
// Header: vdp.h
// Source: vdp.c

	// Function: VDP_WriteVRAM_128K
	// Write data from RAM to VRAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   src		- Source data address in RAM
	//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
	//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
	//   count		- Number of byte to copy in VRAM. Note: A count of 0 mean 65536
	void VDP_WriteVRAM_128K(const u8* src, u16 destLow, u8 destHigh, u16 count);

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX2/2+/TR]
//
// Parameters:
//   src		- Source data address in RAM
//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
//   count		- Number of byte to copy in VRAM
void VDP_WriteVRAM_128K(const u8* src, u16 destLow, u8 destHigh, u16 count)
{
	src;      // HL
	destLow;  // DE
	destHigh; // SP+2 => IY+0
	count;    // SP+3 => IY+1

	__asm
		ld		iy, #2
		add		iy, sp
		// Setup address register
		ld		a, 0(iy)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		add		a, c
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14)
		ld		a, e
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT
		// while (count--) DataPort = *src++;
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 1(iy)				// count LSB
		and		a
		jp		z, wrt_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt_loop_init:
		ld		a, 2(iy)				// count MSB
	wrt_loop_start:
		and		a
		jp		z, wrt_loop_end			// finished
		ld		b, #0					// send 256 bytes packages
		otir
		dec		a
		jp		wrt_loop_start

	wrt_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

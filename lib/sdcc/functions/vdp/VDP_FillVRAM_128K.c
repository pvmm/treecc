// Function: VDP_FillVRAM_128K
// Header: vdp.h
// Source: vdp.c

	// Function: VDP_FillVRAM_128K
	// Fill VRAM area with a given value. [MSX2/2+/TR]
	//
	// Parameters:
	//   value		- Byte value to copy in VRAM
	//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
	//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
	//   count		- Number of byte to copy in VRAM. Note: A count of 0 mean 65536
	void VDP_FillVRAM_128K(u8 value, u16 destLow, u8 destHigh, u16 count);

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value [MSX2/2+/TR]
//
// Parameters:
//   value		- Byte value to copy in VRAM
//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
//   count		- Number of byte to copy in VRAM
void VDP_FillVRAM_128K(u8 value, u16 destLow, u8 destHigh, u16 count)
{
	value;		// A => B
	destLow;	// DE
	destHigh;	// SP+2 => IY+0
	count;		// SP+3 => IY+1

	__asm
		ld		b, a					// Backup input value
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
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
		ld		a, e
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
		// while (count--) DataPort = value;
		ld		e, 1(iy)				// count
		ld		d, 2(iy)
		ld		a, b					// value
		// fast 16-bits loop
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
	fll_loop_start:
		out		(P_VDP_DATA), a			// fill			12cc
		djnz	fll_loop_start			//				14cc
		dec		d
		jp		nz, fll_loop_start

	fll_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

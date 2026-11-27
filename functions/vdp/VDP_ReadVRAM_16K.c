// Function: VDP_ReadVRAM_16K
// Header: vdp.h
// Source: vdp.c

// Function: VDP_ReadVRAM_16K
// Read data from VRAM to RAM. [MSX1/2/2+/TR]
//
// Parameters:
//   src	- Source address in VRAM (14 bits address form 16KB VRAM)
//   dest	- Desitation data address in RAM
//   count	- Number of byte to copy from VRAM. Note: A count of 0 mean 65536
void VDP_ReadVRAM_16K(u16 src, u8* dest, u16 count);

//-----------------------------------------------------------------------------
// Read data from VRAM to RAM [MSX1/2/2+/TR]
//
// Parameters:
//   src	- Source address in VRAM (14-bit address form 16KB VRAM)
//   dst	- Desitation data address in RAM
//   count	- Number of byte to copy from VRAM
void VDP_ReadVRAM_16K(u16 src, u8* dest, u16 count)
{
	src;   // HL
	dest;  // DE
	count; // SP+2 => IY+0

	__asm

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		ld		iy, #2
		add		iy, sp
		// Setup address register
		ld		a, l
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ

	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29 cc (use 29 cc loop)

		// Setup fast 16-bits loop
		ex		de, hl
		ld		e, 0(iy)				// count
		ld		d, 1(iy)
		ld		c, #P_VDP_DATA
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
		// Fast loop
	rd16_loop_start:
		ini								// in(c) ; hl++ ; b--					18 cc
		jp		nz, rd16_loop_start		//										11 cc
		dec		d
		jp		nz, rd16_loop_start		// b = 0 = 256

	#else // Worst case 20 cc (use 23 cc loop)

		// while (count--) *src++ = DataPort;
		ex		de, hl
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 0(iy)				// count LSB
		and		a
		jp		z, rd16_loop_init		// skip LSB
		ld		b, a					// retreive (count & 0x00FF) bytes
		inir							// 										23 cc
		// Handle count MSB
	rd16_loop_init:
		ld		a, 1(iy)				// count MSB
	rd16_loop_start:
		and		a
		jp		z, rd16_loop_end		// finished
		ld		b, #0					// retreive 256 bytes packages
		inir							// 										23 cc
		dec		a
		jp		rd16_loop_start

	#endif

	rd16_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

// Function: VDP_WriteVRAM_16K
// Header: vdp.h
// Source: vdp.c

// Function: VDP_WriteVRAM_16K
// Write data from RAM to VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   src   - Source data address in RAM
//   dest  - Destination address in VRAM (14 bits address for 16KB VRAM)
//   count - Number of byte to copy in VRAM. Note: A count of 0 mean 65536
void VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count);

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX1/2/2+/TR]
//
// Parameters:
//   src   - Source data address in RAM
//   dest  - Destination address in VRAM (14-bit address for 16KB VRAM)
//   count - Number of byte to copy in VRAM
void VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count)
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
		ld		a, e
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT

	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29 cc (use 29 cc loop with "outi + jp nz")

		// Setup fast 16-bits loop
		ld		e, 0(iy)				// count
		ld		d, 1(iy)
		ld		c, #P_VDP_DATA
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
		// Fast loop
	wrt16_loop_start:
		outi							// out(c) ; hl++ ; b--
		jp		nz, wrt16_loop_start
		dec		d
		jp		nz, wrt16_loop_start	// b = 0 = 256

	#else // Worst case 20 cc (use 23 cc loop with "otir")

		// while (count--) DataPort = *src++;
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 0(iy)				// count LSB
		and		a
		jp		z, wrt16_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt16_loop_init:
		ld		a, 1(iy)				// count MSB
	wrt16_loop_start:
		and		a
		jp		z, wrt16_loop_end		// finished
		ld		b, #0					// send 256 bytes packages
		otir
		dec		a
		jp		wrt16_loop_start

	#endif

	wrt16_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

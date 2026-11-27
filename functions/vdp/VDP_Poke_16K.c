// Function: VDP_Poke_16K
// Header: vdp.h
// Source: vdp.c

// Function: VDP_Poke_16K
// Write a value to VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   val	- Value to write in VRAM
//   dest	- Desitation address in VRAM (14 bits address form 16KB VRAM)
void VDP_Poke_16K(u8 val, u16 dest) __PRESERVES(c, h, l, iyl, iyh);

//-----------------------------------------------------------------------------
// Write a value to VRAM [MSX1/2/2+/TR]
//
// Parameters:
//   val	- Value to write in VRAM
//   dest	- Desitation address in VRAM (14-bit address form 16KB VRAM)
void VDP_Poke_16K(u8 val, u16 dest) __PRESERVES(c, h, l, iyl, iyh)
{
	val;  // A
	dest; // DE

	__asm
		ld		b, a					// Backup A register

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		// Set destination address bits 0~7 to port #1
		ld		a, e
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)

		// Set destination address bits 8~13 to port #1
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_WRIT

		// Wait for VDP to be ready
	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29cc
		inc 	de						//  7cc		For 29cc wait
	#endif

		// Write data
		ld		a, b					//  5cc
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		out		(P_VDP_DATA), a			// 12cc		DataPort = value

	__endasm;
}

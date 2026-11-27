// Function: VDP_Peek_16K
// Header: vdp.h
// Source: vdp.c

// Function: VDP_Peek_16K
// Read a value from VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   src	- Source address in VRAM (14 bits address form 16KB VRAM)
//
// Return:
//   Value read in VRAM
u8 VDP_Peek_16K(u16 src) __PRESERVES(b, c, d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// Read a value from VRAM [MSX1/2/2+/TR]
//
// Parameters:
//   dest	- Desitation address in VRAM (14-bit address form 16KB VRAM)
//
// Return:
//   Value read in VRAM
u8 VDP_Peek_16K(u16 dest) __NAKED __PRESERVES(b, c, d, e, iyl, iyh)
{
	dest; // HL

	__asm

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		// Set destination address bits 0~7 to port #1
		ld		a, l
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)

		// Set destination address bits 8~13 to port #1
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ

		// Wait for VDP to be ready
	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29cc (interval 29cc)
		add		hl, hl					// 12cc		For 29cc wait
	#elif ((VDP_USE_MODE_T1) || (VDP_USE_MODE_T2)) // Worst case 20cc (interval 22cc)
		nop								//  5cc		For 20cc wait
	#endif

		// Read data
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		in		a, (P_VDP_DATA)			// 12cc		value = DataPort

		ret
	__endasm;
}

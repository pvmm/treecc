// Function: VDP_SetPalette
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetPalette
// Set a new color palette from index 1 to 15. [MSX2/2+/TR]
// If VDP_USE_PALETTE16 is set to TRUE, index 0 to 15 are used.
//
// Parameters:
//   pal - Address of the palette in RAM; table of 15 (or 16) * 2 bytes
//         Format: [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPalette(const u8* pal) __FASTCALL __PRESERVES(d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// Set a new color palette from index 1 to 15 [MSX2/2+/TR]
// @param		pal			Address of the palette in RAM (table of 15 * 2 bytes)
// 						Format : [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPalette(const u8* pal) __FASTCALL __PRESERVES(d, e, iyl, iyh)
{
	pal; // HL

	__asm
	#if (VDP_USE_PALETTE16)
		xor		a // Start from index 0
	#else
		ld		a, #1 // Start from index 1
	#endif
		VDP_DI  //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(16)
		VDP_EI_DEF  //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a

		ld		c, #P_VDP_PAL
	#if (VDP_USE_PALETTE16)
		ld		b, #32 // Copy 16 x 2 bytes
	#else
		ld		b, #30 // Copy 15 x 2 bytes
	#endif
		otir
		VDP_EI_ALL  //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

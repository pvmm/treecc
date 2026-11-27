// Function: VDP_ReadStatus
// Header: vdp.h
// Source: vdp.c

// Function: VDP_ReadStatus
// Read a given status register then reset status register to default (0). [MSX2/2+/TR]
//
// Parameters:
//   stat - Status register number (0-9)
//
// Return:
//   The given status register value
u8 VDP_ReadStatus(u8 stat) __PRESERVES(b, c, d, e, h, iyl, iyh);

//-----------------------------------------------------------------------------
// Read a given status register then reset status register to default (0) [MSX2/2+/TR]
//
// Parameters:
//   stat - Status register number (0-9)
u8 VDP_ReadStatus(u8 stat) __NAKED __PRESERVES(b, c, d, e, h, iyl, iyh)
{
	stat; // A

	__asm
	#if (VDP_USE_RESTORE_S0)
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_STAT)
		ld		l, a					// return value
		// Reset R#15 to default status register S#0
		xor		a
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR),a
		ld		a, l
	#else
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		in		a, (P_VDP_STAT)			// return value
	#endif
		ret
	__endasm;
}

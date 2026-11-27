// Function: VDP_ReadDefaultStatus
// Header: vdp.h
// Source: vdp.c

// Function: VDP_ReadDefaultStatus
// Read default S#0 register. [MSX1/2/2+/TR]
//
// Return:
//   Status R#0 value
u8 VDP_ReadDefaultStatus() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

//-----------------------------------------------------------------------------
// Read default S#0 register [MSX1/2/2+/TR]
u8 VDP_ReadDefaultStatus() __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	__asm
		in		a, (P_VDP_STAT)
		ret
	__endasm;
}

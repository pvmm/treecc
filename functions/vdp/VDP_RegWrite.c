// Function: VDP_RegWrite
// Header: vdp.h
// Source: vdp.c

// Function: VDP_RegWrite
// Set register value. [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWrite(u8 reg, u8 value) __PRESERVES(b, c, d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// Set register value [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWrite(u8 reg, u8 value) __PRESERVES(b, c, d, e, iyl, iyh)
{
	reg;	// A
	value;	// L

	__asm
		ld		h, a					// Register number
		ld		a, l					// Value
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		ld		a, h
		add		#0x80					// @todo Can be optimize by including 80h in the register number ; 33cc -> 25cc (warning for MSX1 VRAM write timing)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
	__endasm;
}

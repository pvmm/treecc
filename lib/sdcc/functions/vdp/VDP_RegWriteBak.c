// Function: VDP_RegWriteBak
// Header: vdp.h
// Source: vdp.c

// Function: VDP_RegWriteBak
// Set register value after backuping previous value. [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWriteBak(u8 reg, u8 value) __PRESERVES(d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// Set register value after backuping previous value [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWriteBak(u8 reg, u8 value) __PRESERVES(d, e, iyl, iyh)
{
	reg;	// A
	value;	// L

	__asm
		ld		c, a					// Register number
		ld		a, l					// Value
		// Backup
		ld		b, #0
		ld		hl, #_g_VDP_REGSAV
		add		hl, bc
		ld		(hl), a
		// Write to register
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, c
		add		#0x80					// @todo Can be optimize by including 80h in the register number ; 33cc -> 25cc (warning for MSX1 VRAM write timing)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
	__endasm;
}

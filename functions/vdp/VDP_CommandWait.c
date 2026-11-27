// Function: VDP_CommandWait
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CommandWait
// Wait for previous VDP command to be finished. [MSX2/2+/TR]
void VDP_CommandWait() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

//-----------------------------------------------------------------------------
// Wait for previous VDP command to be finished
void VDP_CommandWait() __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	__asm
	wait_vdp_ready:
	#if (VDP_USE_RESTORE_S0)
		// Set current status register to S#2
		ld		a, #2
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_REG), a
		in		a, (P_VDP_STAT)		// get S#2 value
		rra							// check CE (bit#0)
		// Reset current status register to S#0 before enabling interruptions
		ld		a, #0				// don't use XOR here to keep C flag from RRA alive
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
	#else
		// Set current status register to S#2
		ld		a, #2
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_REG), a
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		in		a, (P_VDP_STAT)		// get S#2 value
		rra							// check CE (bit#0)
	#endif
		jp		c, wait_vdp_ready	// CE==1 ? command not finished
	__endasm;
}

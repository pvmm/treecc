// Function: VDP_CommandWriteLoop
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CommandWriteLoop
// Write to VRAM command loop. [MSX2/2+/TR]
//
// Parameters:
//   addr - Address of the source buffer
void VDP_CommandWriteLoop(const u8* addr) __FASTCALL __PRESERVES(d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// Write to VRAM command loop
// @param		addr		The address in RAM from where read data to be copied in VRAM
// 						(First byte of data was sent in the command execution)
void VDP_CommandWriteLoop(const u8* addr) __FASTCALL __PRESERVES(d, e, iyl, iyh)
{
	addr; // HL

	__asm
		// Set indirect register write to R#44
		ld  	a, #VDP_REG(44)
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(17)
		out 	(P_VDP_REG), a
		// Set current status register to S#2
		ld  	a, #2
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		out 	(P_VDP_REG), a
		// Setup outi loop (value of register B don't matter)
		inc 	hl
		ld		c, #P_VDP_IREG
	write_loop:
		// Read S#2 to check CE flag (no need to check TR (bit#7) while write loop is longer than worse case VDP write duration (~29cc))
		in		a, (P_VDP_STAT)
		rra							// check CE (bit#0)
		jp		nc, write_finished	// CE==0 ? command finished
		outi						// write a byte from HL to port VDP_IREG
		jp		write_loop

	write_finished:
		// Reset current status register to S#0
	#if (VDP_USE_RESTORE_S0)
		xor 	a
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
	#else
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
	#endif
	__endasm;
}

// Function: BIOS_Exit
// Header: bios.h
// Source: bios.c

// Function: BIOS_Exit
// Handle clean exit form Basic or MSX-DOS environment
//
// Parameters:
//   ret - Return value (for MSX-DOS environment)
void BIOS_Exit(u8 ret);

//-----------------------------------------------------------------------------
// Handle clean transition to Basic or MSX-DOS environment
// For MSX-DOS, return value is in L
void BIOS_Exit(u8 ret)
{
	ret;	// A
#if (TARGET_TYPE == TYPE_DOS)

	__asm

		push	af
		// Set Screen mode to 2...
		ld		a, #2
		ld		ix, #R_CHGMOD
		ld		iy, (M_EXPTBL-1)
		call	R_CALSLT
		// ... to be able to call TOTEXT routine
		ld		ix, #R_TOTEXT
		ld		iy, (M_EXPTBL-1)
		call	R_CALSLT
		pop		af

		// Call DOS exit function
		ld		b, a
		ld		c, #DOS_FUNC_TERM // Try MSX-DOS 2's termination function...
		call	BDOS
		ld		c, #DOS_FUNC_TERM0 // ... if not available, use MSX-DOS 1's one
		jp		BDOS
	__endasm;

#elif (TARGET_TYPE == TYPE_BIN)

	__asm
		// Set Screen mode to 2...
		ld		a, #2
		call	R_CHGMOD
		// ... to be able to call TOTEXT routine
		call	R_TOTEXT

		//
		ld		ix, #0x409B // address of "warm boot" BASIC interpreter
		// this routine is called to reset the stack if basic is externally stopped and then restarted.
		call	R_CALBAS
	__endasm;

#else // if (TARGET_TYPE == TYPE_ROM)

	__asm
		call	0x0000				// Soft reset
	__endasm;

#endif
}

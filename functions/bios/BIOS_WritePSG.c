// Function: BIOS_WritePSG
// Header: bios.h
// Source: bios.c

// Function: BIOS_WritePSG
// Writes data to PSG-register. Wrapper for WRTPSG routine.
//> WRTPSG
//> Address  : #0093
//> Function : Writes data to PSG register
//> Input    : A  - PSG register number
//>            E  - Data write
//> Remarks  : See https://www.msx.org/wiki/SOUND#Registers_Description
//
// Parameters:
//   reg   - PSG register number (0-15)
//   value - Value to write in the PSG register
void BIOS_WritePSG(u8 reg, u8 value);

//-----------------------------------------------------------------------------
// WRTPSG
// Address  : #0093
// Function : Writes data to PSG register
// Input    : A  - PSG register number
//            E  - Data write
// Remarks  : See https://www.msx.org/wiki/SOUND#Registers_Description
void BIOS_WritePSG(u8 reg, u8 value)
{
	reg;   // A
	value; // L

	__asm
		ld		e, l
		call	R_WRTPSG
	__endasm;
}

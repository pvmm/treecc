// Function: Math_Mod10
// Header: math.h
// Source: math.c

// Function: Math_Mod10
// 8-bits fast modulo-10
//
// Parameters:
//   val - Value to module by 10
//
// Return:
//   val % 10
u8 Math_Mod10(u8 val) __PRESERVES(b, c, d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// 18-bits fast modulo-10
// Inputs:		A	8-bit unsigned integer
// Outputs:		A	HL mod 10
//				Z	Flag is set if divisible by 10
// 20 bytes, 83 cycles
u8 Math_Mod10(u8 val) __NAKED __PRESERVES(b, c, d, e, iyl, iyh)
{
	val; // A

__asm
	ld		h, a		// add nibbles
	rrca
	rrca
	rrca
	rrca
	add		a, h
	adc		a, #0		// n mod 15 (+1) in both nibbles
	daa
	ld		l, a
	sub		h			// test if quotient is even or odd
	rra
	sbc		a, a
	and		#5
	add		a, l
	daa
	and		#0x0F
	ret
__endasm;
}

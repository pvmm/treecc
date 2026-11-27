// Function: Math_Mod10_16b
// Header: math.h
// Source: math.c

// Function: Math_Mod10_16b
// 16-bits fast modulo-10
//
// Parameters:
//   val - Value to module by 10
//
// Return:
//   val % 10
u8 Math_Mod10_16b(u16 val) __FASTCALL __PRESERVES(b, c, d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// 16-bits fast modulo-10
// Inputs:		HL	16-bit unsigned integer
// Outputs:		A	HL mod 10
//				Z	Flag is set if divisible by 10
// 24 bytes,  98cc
u8 Math_Mod10_16b(u16 val) __NAKED __FASTCALL __PRESERVES(b, c, d, e, iyl, iyh)
{
	val; // HL

__asm
	ld		a, h		// add bytes
	add		a, l
	adc		a, #0		// n mod 255 (+1)
	ld		h, a		// add nibbles
	rrca
	rrca
	rrca
	rrca
	add		a, h
	adc		a, #0		// n mod 15 (+1) in both nibbles
	daa
	ld		h, a
	sub		l			// test if quotient is even or odd
	rra
	sbc		a, a
	and		#5
	add		a, h
	daa
	and		#0x0F
	ld		l, a
	ret
__endasm;
}

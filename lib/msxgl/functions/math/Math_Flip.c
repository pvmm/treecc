// Function: Math_Flip
// Header: math.h
// Source: math.c

// Function: Math_Flip
// Bits flip routine
//
// Parameters:
//   val - Value to flip
//
// Return:
//   Bits flipped value
u8 Math_Flip(u8 val) __PRESERVES(c, d, e, h, l, iyl, iyh);

//-----------------------------------------------------------------------------
// 8-bits fast bits flip
// Inputs:		A		8-bits value
// Outputs:		A		Bits flipped value
// Author:		John Metcalf (http://www.retroprogramming.com/2014/01/fast-z80-bit-reversal.html)
u8 Math_Flip(u8 val) __NAKED __PRESERVES(c, d, e, h, l, iyl, iyh)
{
	val; // A

__asm
	// reverse bits in A
	// 17 bytes / 66 cycles
	ld		b, a		// a = 76543210
	rlca
	rlca				// a = 54321076
	xor		b
	and		#0xAA
	xor		b			// a = 56341270
	ld		b, a
	rlca
	rlca
	rlca				// a = 41270563
	rrc 	b			// b = 05634127
	xor 	b
	and 	#0x66
	xor 	b			// a = 01234567
	ret
__endasm;
}

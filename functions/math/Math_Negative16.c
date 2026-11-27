// Function: Math_Negative16
// Header: math.h
// Source: math.c

// Function: Math_Negative16
// Get the negative (additive inverse) of a 16-bit register
//
// Parameters:
//   val - Signed 16-bit value to negate
//
// Return:
//   Negative value of the input
i16 Math_Negative16(i16 val) __FASTCALL __PRESERVES(b, c, d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// Get the negative (additive inverse) of a 16-bit register
i16 Math_Negative16(i16 val) __NAKED __FASTCALL __PRESERVES(b, c, d, e, iyl, iyh)
{
	val; // HL

__asm
	xor		a
	sub		l
	ld		l, a
	sbc		a, a
	sub		h
	ld		h, a
	ret
__endasm;
}

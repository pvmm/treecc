// Function: Math_Flip_16b
// Header: math.h
// Source: math.c

// Function: Math_Flip_16b
// Bits flip routine
//
// Parameters:
//   val - Value to flip
//
// Return:
//   Bits flipped value
u16 Math_Flip_16b(u16 val) __PRESERVES(c, iyl, iyh);

//-----------------------------------------------------------------------------
// 16-bits bits flip
u16 Math_Flip_16b(u16 val) __NAKED __PRESERVES(c, iyl, iyh)
{
	val; // HL

__asm
	ld		a, h
	call	_Math_Flip	// flip high bits
	ld		e, a
	ld		a, l
	call	_Math_Flip	// flip low bits
	ld		d, a		// flip high/low
	ret
__endasm;
}

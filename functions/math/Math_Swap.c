// Function: Math_Swap
// Header: math.h
// Source: math.c

// Function: Math_Swap
// Swap MSB and LSB bytes
//
// Parameters:
//   val - 16-bits value to swap
//
// Return:
//   Swapped value
u16 Math_Swap(u16 val) __PRESERVES(a, b, c, iyl, iyh);

//-----------------------------------------------------------------------------
// Swap MSB and LSB bytes
u16 Math_Swap(u16 val) __NAKED __PRESERVES(a, b, c, iyl, iyh)
{
	val; // HL

__asm
	ld		e, h
	ld		d, l
	ret
__endasm;
}

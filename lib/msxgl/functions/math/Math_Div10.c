// Function: Math_Div10
// Header: math.h
// Source: math.c

// Function: Math_Div10
// 8-bits fast 10 times division
//
// Parameters:
//   val - Value to divide by 10
//
// Return:
//   val / 10
i8 Math_Div10(i8 val) __FASTCALL __PRESERVES(a, b, c, iyl, iyh);

//-----------------------------------------------------------------------------
// 8-bits fast 10 times division
i8 Math_Div10(i8 val) __NAKED __FASTCALL __PRESERVES(a, b, c, iyl, iyh)
{
	val; // L

__asm
	ld		d, #0
	ld		h, d
	ld		e, l
	add		hl, hl
	add		hl, de
	add		hl, hl
	add		hl, hl
	add		hl, de
	add		hl, hl
	ld		l, h
	ret
__endasm;
}

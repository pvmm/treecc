// Function: Math_Div10_16b
// Header: math.h
// Source: math.c

// Function: Math_Div10_16b
// 16-bits fast 10 times division
//
// Parameters:
//   val - Value to divide by 10
//
// Return:
//   val / 10
i16 Math_Div10_16b(i16 val) __FASTCALL __PRESERVES(b, d, e, iyl, iyh);

//-----------------------------------------------------------------------------
// 16-bits fast 10 times division
i16 Math_Div10_16b(i16 val) __NAKED __FASTCALL __PRESERVES(b, d, e, iyl, iyh)
{
	val; // HL

__asm
	ld		bc, #0x0D0A
	xor		a
	add		hl, hl
	rla
	add		hl, hl
	rla
	add		hl, hl
	rla
	add		hl, hl
	rla
	cp		c
label_0:
	jr		c, label_0 + #4
	sub		c
	inc		l
label_1:
	djnz	label_1 - #7
	ret
__endasm;
}

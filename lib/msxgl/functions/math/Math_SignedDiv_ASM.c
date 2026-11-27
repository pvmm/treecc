// Function: Math_SignedDiv_ASM
// Source: math.c

//-----------------------------------------------------------------------------
// Divide a signed 8-bits integer by 2, 4, 8, 16, 32 or 64 using right shift
void Math_SignedDiv_ASM() __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
__asm
_Math_SignedDiv64:: // val -> A
	sra		a
_Math_SignedDiv32:: // val -> A
	sra		a
_Math_SignedDiv16:: // val -> A
	sra		a
_Math_SignedDiv8:: // val -> A
	sra		a
_Math_SignedDiv4:: // val -> A
	sra		a
_Math_SignedDiv2:: // val -> A
	sra		a
	ret		p						// Return if positive
	inc		a						// Overwise, add 1 to get proper negative value
	ret
__endasm;
}

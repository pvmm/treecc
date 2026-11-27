// Function: VDP_SetHorizontalOffset
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetHorizontalOffset
// Set the horizontal rendeing offset. [MSX2+/TR]
//
// Parameters:
//   offset - Screen horizontal offset (9-bits value in pixel)
void VDP_SetHorizontalOffset(u16 offset);

//-----------------------------------------------------------------------------
// Set the horizontal rendeing offset (in pixel) [MSX2+/TR]
void VDP_SetHorizontalOffset(u16 offset)
{
	u8 reg = (8 - (offset & 0xFF)) & 0x07;
	VDP_RegWrite(27, reg);
	reg = (offset + 7 ) >> 3;
	VDP_RegWrite(26, reg);
}

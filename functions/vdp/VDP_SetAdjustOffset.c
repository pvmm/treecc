// Function: VDP_SetAdjustOffset
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetAdjustOffset
// Adjustment of the display location on the screen (R#18). [MSX2/2+/TR]
//
// Parameters:
//   offset - Screen display position offset (MSB 4-bits: vertical offset, LSB 4-bits: horizontal offset)
void VDP_SetAdjustOffset(u8 offset);

//-----------------------------------------------------------------------------
// Adjustment of the display location on the screen [MSX2/2+/TR]
void VDP_SetAdjustOffset(u8 offset)
{
	#if (VDP_USE_COMMAND)
	VDP_CommandWait(); // @todo Check if it's really needed. Cf. https://www.msx.org/wiki/VDP_Display_Registers#Control_Register_18
	#endif
	VDP_RegWrite(18, offset);
}

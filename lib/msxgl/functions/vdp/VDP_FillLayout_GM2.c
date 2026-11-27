// Function: VDP_FillLayout_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_FillLayout_GM2
// Fill a rectangle with a given value. [MSX1/2/2+/TR]
//
// Parameters:
//   value - Pattern index to fill
//   dx    - Destination starting X position
//   dy    - Destination starting Y position
//   nx    - Destination width
//   ny    - Destination height
void VDP_FillLayout_GM2(u8 value, u8 dx, u8 dy, u8 nx, u8 ny);

//-----------------------------------------------------------------------------
// Fill a rectangle with a given value
void VDP_FillLayout_GM2(u8 value, u8 dx, u8 dy, u8 nx, u8 ny)
{
	u16 dst = g_ScreenLayoutLow + (dy * 32) + dx;
	for (u8 y = 0; y < ny; ++y)
	{
		VDP_FillVRAM(value, dst, g_ScreenLayoutHigh, nx);
		dst += 32;
	}
}

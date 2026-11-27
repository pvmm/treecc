// Function: VDP_WriteLayout_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_WriteLayout_GM2
// Copy patterns layout to a given rectangle. [MSX1/2/2+/TR]
//
// Parameters:
//   src    - Address of data buffer to copy to VRAM
//   dx     - Destination starting X position
//   dy     - Destination starting Y position
//   nx     - Destination width
//   ny     - Destination height
void VDP_WriteLayout_GM2(const u8* src, u8 dx, u8 dy, u8 nx, u8 ny);

//-----------------------------------------------------------------------------
// Copy patterns to a rectangle
void VDP_WriteLayout_GM2(const u8* src, u8 dx, u8 dy, u8 nx, u8 ny)
{
	u16 dst = g_ScreenLayoutLow + (dy * 32) + dx;
	for (u8 y = 0; y < ny; ++y)
	{
		VDP_WriteVRAM(src, dst, g_ScreenLayoutHigh, nx);
		src += nx;
		dst += 32;
	}
}

// Function: VDP_LoadPattern_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_LoadPattern_GM2
// Load tile patterns in all 3 screen sections. [MSX1/2/2+/TR]
//
// Parameters:
//   src    - Address of data buffer to copy to VRAM
//   count  - Number of pattern to copy. Note: A count of 0 mean 256
//   offset - Pattern index from which to copy data
void VDP_LoadPattern_GM2(const u8* src, u8 count, u8 offset);

//-----------------------------------------------------------------------------
// Load patterns in all 3 screen sections
void VDP_LoadPattern_GM2(const u8* src, u8 count, u8 offset)
{
	u16 cnt = count == 0 ? 256 * 8 : count * 8;
	u16 dst = g_ScreenPatternLow + (offset * 8);
	VDP_WriteVRAM(src, dst, g_ScreenPatternHigh, cnt);
	dst += 0x800;
	VDP_WriteVRAM(src, dst, g_ScreenPatternHigh, cnt);
	dst += 0x800;
	VDP_WriteVRAM(src, dst, g_ScreenPatternHigh, cnt);
}

// Function: VDP_LoadSpritePattern
// Header: vdp.h
// Source: vdp.c

// Function: VDP_LoadSpritePattern
// Load sprite pattern data into VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   addr  - Source address for sprite patterns (in RAM/ROM).
//   index - Index of the first VRAM sprite pattern to copy to (in 16x16 mode, only multiple of 4 can be used).
//   count - Number of patterns to copy (8x8 mode use 1 per sprite shape, while 16x16 mode use 4).
void VDP_LoadSpritePattern(const u8* addr, u8 index, u8 count);

//-----------------------------------------------------------------------------
// Load pattern data into VRAM
void VDP_LoadSpritePattern(const u8* addr, u8 index, u8 count)
{
	u16 low = g_SpritePatternLow;
	low += (index * 8);
	VDP_WriteVRAM(addr, low, g_SpritePatternHigh, count * 8);
}

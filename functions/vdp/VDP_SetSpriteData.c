// Function: VDP_SetSpriteData
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteData
// Set sprite data for Sprite Mode 2. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31]
//   data  - Address to a 3 bytes data buffer including sprite position and shape
void VDP_SetSpriteData(u8 index, const u8* data);

//-----------------------------------------------------------------------------
//
void VDP_SetSpriteData(u8 index, const u8* data)
{
	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM(data, low, g_SpriteAttributeHigh, 3);
}

// Function: VDP_SetSpritePatternTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpritePatternTable
// Set sprite pattern table address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetSpritePatternTable(VADDR addr);

//-----------------------------------------------------------------------------
// Set sprite pattern table address
// @param		addr		VRAM address where to place the table (16 or 17-bits long depending on VDP_VRAM_ADDR definition)
//							Address must be a multiple of 800h.
void VDP_SetSpritePatternTable(VADDR addr)
{
	g_SpritePatternLow  = (u16)addr;
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_SpritePatternHigh = addr >> 16;
	#endif

	u8 reg = (u8)(addr >> 11);
	VDP_RegWrite(6, reg);

}

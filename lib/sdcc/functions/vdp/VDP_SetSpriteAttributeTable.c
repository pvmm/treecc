// Function: VDP_SetSpriteAttributeTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteAttributeTable
// Set sprite attribute table address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetSpriteAttributeTable(VADDR addr);

//-----------------------------------------------------------------------------
// Set sprite attribute table address
// @param		addr		VRAM address where to place the table (16 or 17-bits long depending on VDP_VRAM_ADDR definition)
//							Address must be a multiple of 80h for MSX1 screen modes and  multiple of 200h for MSX2 ones.
void VDP_SetSpriteAttributeTable(VADDR addr)
{
	g_SpriteAttributeLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 7);
	#if (MSX_VERSION >= MSX_2)
		switch (g_VDP_Data.Mode)
		{
		case VDP_MODE_GRAPHIC3:
		case VDP_MODE_GRAPHIC4:
		case VDP_MODE_GRAPHIC5:
		case VDP_MODE_GRAPHIC6:
		case VDP_MODE_GRAPHIC7:
		#if (MSX_VERSION >= MSX_2P)
		case VDP_MODE_SCREEN10:
		case VDP_MODE_SCREEN11:
		case VDP_MODE_SCREEN12:
		#endif
		#if (VDP_USE_UNDOCUMENTED)
		case VDP_MODE_GRAPHIC3_MIRROR_0:
		case VDP_MODE_GRAPHIC3_MIRROR_01:
		case VDP_MODE_GRAPHIC3_MIRROR_02:
		#endif
			reg |= 0b111;
			break;
		};
	#endif
	VDP_RegWrite(5, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		reg = (u8)(addr >> 15);
		VDP_RegWrite(11, reg);

		g_SpriteAttributeHigh = addr >> 16;
	#endif

	addr -= 0x200;
	g_SpriteColorLow = (u16)addr;
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_SpriteColorHigh = addr >> 16;
	#endif
}

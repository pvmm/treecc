// Function: VDP_SetColorTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetColorTable
// Set color table VRAM address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetColorTable(VADDR addr);

//-----------------------------------------------------------------------------
// Set color table VRAM address
void VDP_SetColorTable(VADDR addr)
{
	g_ScreenColorLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 6);
	switch (g_VDP_Data.Mode)
	{
	#if (MSX_VERSION >= MSX_2)
	case VDP_MODE_TEXT2:
		reg |= 0b111;
		break;
	#if (VDP_USE_UNDOCUMENTED)
	case VDP_MODE_GRAPHIC3_MIRROR_0:
		reg |= 0b0011111;
		break;
	case VDP_MODE_GRAPHIC3_MIRROR_01:
		reg |= 0b0111111;
		break;
	case VDP_MODE_GRAPHIC3_MIRROR_02:
		reg |= 0b1011111;
		break;
	#endif
	case VDP_MODE_GRAPHIC3:
	#endif
	case VDP_MODE_GRAPHIC2:
		reg |= 0b1111111;
		break;
	};
	VDP_RegWrite(3, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		reg = (u8)(addr >> 14);
		VDP_RegWrite(10, reg);

		g_ScreenColorHigh = addr >> 16;
	#endif
}

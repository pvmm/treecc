// Function: VDP_SetPatternTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetPatternTable
// Set pattern table VRAM address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetPatternTable(VADDR addr);

//-----------------------------------------------------------------------------
// Set pattern table VRAM address
void VDP_SetPatternTable(VADDR addr)
{
	g_ScreenPatternLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 11);
	switch (g_VDP_Data.Mode)
	{
	#if (MSX_VERSION >= MSX_2)
	#if (VDP_USE_UNDOCUMENTED)
	case VDP_MODE_GRAPHIC3_MIRROR_01:
		reg |= 0b01;
		break;
	case VDP_MODE_GRAPHIC3_MIRROR_02:
		reg |= 0b10;
		break;
	#endif
	case VDP_MODE_GRAPHIC3:
	#endif
	case VDP_MODE_GRAPHIC2:
		reg |= 0b11;
	};
	VDP_RegWrite(4, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenPatternHigh = addr >> 16;
	#endif
}

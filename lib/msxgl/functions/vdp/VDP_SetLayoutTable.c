// Function: VDP_SetLayoutTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetLayoutTable
// Set layout table VRAM address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetLayoutTable(VADDR addr);

//-----------------------------------------------------------------------------
// Set layout table VRAM address
void VDP_SetLayoutTable(VADDR addr)
{
	g_ScreenLayoutLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 10);
	#if (MSX_VERSION >= MSX_2)
		switch (g_VDP_Data.Mode)
		{
		case VDP_MODE_TEXT2:
			reg |= 0b11;
			break;
		case VDP_MODE_GRAPHIC6:
		case VDP_MODE_GRAPHIC7:
		#if (MSX_VERSION >= MSX_2P)
		case VDP_MODE_SCREEN10:
		case VDP_MODE_SCREEN11:
		case VDP_MODE_SCREEN12:
		#endif
			reg >>= 1;
		case VDP_MODE_GRAPHIC4:
		case VDP_MODE_GRAPHIC5:
			reg |= 0b11111;
			break;
		};
	#endif
	VDP_RegWrite(2, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh = addr >> 16;
	#endif
}

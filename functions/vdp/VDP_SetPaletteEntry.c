// Function: VDP_SetPaletteEntry
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetPaletteEntry
// Set a given color entry in the palette. [MSX2/2+/TR]
//
// Parameters:
//   index - Index of the palette entry (0-15)
//   color - 16 bits color value (@see RGB16 macro)
//           Format: [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPaletteEntry(u8 index, u16 color);

//-----------------------------------------------------------------------------
// Set a given color entry in the palette [MSX2/2+/TR]
// @param		index		Index of the palette entry (0-15)
// @param		color		16 bits color value (@see RGB16 macro)
//							Format : [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPaletteEntry(u8 index, u16 color)
{
	index; // A
	color; // DE
	__asm
		VDP_DI
		out	(_io_VDP_Reg), a
		ld	a, #VDP_REG(16)
		out	(_io_VDP_Reg), a
		ld	a, e
		out	(_io_VDP_Pal), a
		ld	a, d
		VDP_EI
		out	(_io_VDP_Pal), a
	__endasm;
}

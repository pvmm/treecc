// Function: VDP_SetMSX1Palette
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetMSX1Palette
// Reset the default MSX1 palette. [MSX2/2+/TR]
void VDP_SetMSX1Palette();

void VDP_SetMSX1Palette()
{
	VDP_SetPalette((const u8*)g_PaletteMSX1);
}

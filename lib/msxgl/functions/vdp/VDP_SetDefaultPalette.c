// Function: VDP_SetDefaultPalette
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetDefaultPalette
// Reset the default MSX2 palette. [MSX2/2+/TR]
void VDP_SetDefaultPalette();

void VDP_SetDefaultPalette()
{
	VDP_SetPalette((const u8*)g_PaletteMSX2);
}

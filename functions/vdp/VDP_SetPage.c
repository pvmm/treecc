// Function: VDP_SetPage
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetPage
// Set current VRAM page. [MSX2/2+/TR]
//
// Parameters:
//   page - Page number to set
void VDP_SetPage(u8 page);

//-----------------------------------------------------------------------------
// Set current VRAM page
void VDP_SetPage(u8 page)
{
	u8 reg = page << 5;
	reg |= 0b11111;
	VDP_RegWriteBak(2, reg);
}

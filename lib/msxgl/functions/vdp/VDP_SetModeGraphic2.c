// Function: VDP_SetModeGraphic2
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 2 screen mode.
void VDP_SetModeGraphic2()
{
	VDP_SetModeFlag(VDP_G2_MODE);
	VDP_SetLayoutTable(VDP_G2_ADDR_NT);
	VDP_SetColorTable(VDP_G2_ADDR_CT);
	VDP_SetPatternTable(VDP_G2_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G2_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G2_ADDR_SPT);
	#endif
}

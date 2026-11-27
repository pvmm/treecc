// Function: VDP_SetModeGraphic1
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 1 screen mode.
void VDP_SetModeGraphic1()
{
	VDP_SetModeFlag(VDP_G1_MODE);
	VDP_SetLayoutTable(VDP_G1_ADDR_NT);
	VDP_SetColorTable(VDP_G1_ADDR_CT);
	VDP_SetPatternTable(VDP_G1_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G1_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G1_ADDR_SPT);
	#endif
}

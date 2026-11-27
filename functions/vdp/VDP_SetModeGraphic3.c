// Function: VDP_SetModeGraphic3
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 3 screen mode.
void VDP_SetModeGraphic3()
{
	VDP_SetModeFlag(VDP_G3_MODE);
	VDP_SetLayoutTable(VDP_G3_ADDR_NT);
	VDP_SetColorTable(VDP_G3_ADDR_CT);
	VDP_SetPatternTable(VDP_G3_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G3_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G3_ADDR_SPT);
	#endif
}

// Function: VDP_SetModeGraphic5
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 5 screen mode.
void VDP_SetModeGraphic5()
{
	VDP_SetModeFlag(VDP_G5_MODE);
	VDP_SetLayoutTable(VDP_G5_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G5_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G5_ADDR_SPT);
	#endif
}

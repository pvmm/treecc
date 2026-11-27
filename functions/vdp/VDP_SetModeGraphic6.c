// Function: VDP_SetModeGraphic6
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 6 screen mode.
void VDP_SetModeGraphic6()
{
	VDP_SetModeFlag(VDP_G6_MODE);
	VDP_SetLayoutTable(VDP_G6_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G6_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G6_ADDR_SPT);
	#endif
}

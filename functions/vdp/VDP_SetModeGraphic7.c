// Function: VDP_SetModeGraphic7
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 7 screen mode.
void VDP_SetModeGraphic7()
{
	VDP_SetModeFlag(VDP_G7_MODE);
	VDP_SetLayoutTable(VDP_G7_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G7_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G7_ADDR_SPT);
	#endif
}

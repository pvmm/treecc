// Function: VDP_SetModeMultiColor
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Multi Color screen mode.
void VDP_SetModeMultiColor()
{
	VDP_SetModeFlag(VDP_MC_MODE);
	VDP_SetLayoutTable(VDP_MC_ADDR_NT);
	VDP_SetPatternTable(VDP_MC_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_MC_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_MC_ADDR_SPT);
	#endif
}

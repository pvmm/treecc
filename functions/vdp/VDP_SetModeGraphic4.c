// Function: VDP_SetModeGraphic4
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Graphic 4 screen mode.
void VDP_SetModeGraphic4()
{
	VDP_SetModeFlag(VDP_G4_MODE);
	VDP_SetLayoutTable(VDP_G4_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G4_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G4_ADDR_SPT);
	#endif
}

// Function: VDP_SetModeText2
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Text 2 screen mode.
void VDP_SetModeText2()
{
	VDP_SetModeFlag(VDP_T2_MODE);
	VDP_SetLayoutTable(VDP_T2_ADDR_NT);
	VDP_SetColorTable(VDP_T2_ADDR_CT);
	VDP_SetPatternTable(VDP_T2_ADDR_PT);
}

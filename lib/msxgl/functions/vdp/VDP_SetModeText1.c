// Function: VDP_SetModeText1
// Source: vdp.c

//-----------------------------------------------------------------------------
// Initialize Text 1 screen mode.
void VDP_SetModeText1()
{
	VDP_SetModeFlag(VDP_T1_MODE);
	VDP_SetLayoutTable(VDP_T1_ADDR_NT);
	VDP_SetPatternTable(VDP_T1_ADDR_PT);
}

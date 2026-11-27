// Function: VDP_CommandSetupR36
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CommandSetupR36
// Send VDP command (form registres 36 to 46). [MSX2/2+/TR]
void VDP_CommandSetupR36();

//-----------------------------------------------------------------------------
// Send VDP command (from register 36 to 46)
void VDP_CommandSetupR36()
{
	VDP_CommandWait();
	ASM_REG_WRITE_INC(g_VDP_Command + 4, 36, 11);
}

// Function: VDP_CommandSetupR32
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CommandSetupR32
// Send VDP command (form registres 32 to 46). [MSX2/2+/TR]
void VDP_CommandSetupR32();

//-----------------------------------------------------------------------------
// Send VDP command (from register 32 to 46)
void VDP_CommandSetupR32()
{
	VDP_CommandWait();
	ASM_REG_WRITE_INC(g_VDP_Command, 32, 15);
}

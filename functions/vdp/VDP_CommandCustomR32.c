// Function: VDP_CommandCustomR32
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CommandCustomR32
// Send VDP custom command through buffer (form registres 32 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR32(const VDP_Command* data);

//-----------------------------------------------------------------------------
// Send VDP custom command through buffer (form registres 32 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR32(const VDP_Command* data)
{
	data; // HL
	VDP_CommandWait(); // don't modify HL
	ASM_REG_WRITE_INC_HL(32, 15);
}

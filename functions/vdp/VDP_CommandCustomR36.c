// Function: VDP_CommandCustomR36
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CommandCustomR36
// Send VDP custom command through buffer (form registres 36 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR36(const VDP_Command36* data);

//-----------------------------------------------------------------------------
// Send VDP custom command through buffer (form registres 36 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR36(const VDP_Command36* data)
{
	data; // HL
	VDP_CommandWait(); // don't modify HL
	ASM_REG_WRITE_INC_HL(36, 11);
}

// Function: VDP_ClearVRAM
// Header: vdp.h
// Source: vdp.c

// Function: VDP_ClearVRAM
// Clear the VRAM content. [MSX1/2/2+/TR]
void VDP_ClearVRAM();

//-----------------------------------------------------------------------------
// Clear the VRAM content
void VDP_ClearVRAM()
{
	#if (MSX_VERSION & MSX_1)
		VDP_FillVRAM_16K(0, 0x0000, 0x4000);  // Clear 16 KB of VRAM
	#else
		u32 addr = 0;
		loopx (8)
		{
			VDP_FillVRAM(0x00, addr & 0xFFFF, addr >> 16, 0x4000); // Clear VRAM by 16 KB step
			addr += 0x4000;
		}
	#endif
}

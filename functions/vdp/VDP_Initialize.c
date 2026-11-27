// Function: VDP_Initialize
// Header: vdp.h
// Source: vdp.c

// Function: VDP_Initialize
// Initialize VDP module. [MSX1/2/2+/TR]
// Retreive BIOS initiale value.
// This function is called automatically when <VDP_SetMode> is called if the macro VDP_AUTO_INIT equals 1.
void VDP_Initialize();

//-----------------------------------------------------------------------------
// Initialize VDP module [MSX1/2/2+/TR]
// Retreive BIOS initiale value.
// This function is called automatically when VDP_SetMode() is called if the macro VDP_AUTO_INIT equals 1.
void VDP_Initialize()
{
__asm
	// Copy 8 bytes from M_RG0SAV to g_VDP_REGSAV+0 (0-7)
	ld		hl, #M_RG0SAV
	ld		de, #_g_VDP_REGSAV+0
	ld		bc, #8
	ldir
__endasm;

#if (MSX_VERSION >= MSX_2)
__asm
	// Copy 16 bytes from M_RG08SAV to g_VDP_REGSAV+8 (8-23)
	ld		hl, #M_RG08SAV
	ld		de, #_g_VDP_REGSAV+8
	ld		bc, #16
	ldir
__endasm;
#endif

#if (MSX_VERSION >= MSX_2P)
__asm
	// Copy 3 bytes from M_RG25SAV to g_VDP_REGSAV+25 (25-27)
	ld		hl, #M_RG25SAV
	ld		de, #_g_VDP_REGSAV+25
	ld		bc, #3
	ldir
__endasm;
#endif

	#if (VDP_AUTO_INIT)
	g_VDPInitilized = TRUE;
	#endif
}

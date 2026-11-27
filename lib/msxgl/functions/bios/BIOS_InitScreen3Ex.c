// Function: BIOS_InitScreen3Ex
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen3Ex
// Switches to SCREEN 3 (multi-color screen 64*48 pixels). Wrapper for INIMLT routine.
void BIOS_InitScreen3Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

//-----------------------------------------------------------------------------
// INIMLT
// Address  : #0075
// Function : Switches to SCREEN 3 (multi-color screen with 64×48 pixels)
// Input    : MLTNAM (#F3D1) = Address of pattern name table
//            MLTCOL (#F3D3) = Address of colors table
//            MLTCGP (#F3D5) = Address of pattern generator table
//            MLTATR (#F3D7) = Address of sprite attribute table
//            MLTPAT (#F3D9) = Address of sprite generator table
//            FORCLR (#F3E9) = Text color
//            BAKCLR (#F3EA) = Background color
//            BDRCLR (#F3EB) = Border color
// Registers: All
// void BIOS_InitScreen3() { Call(R_INIMLT); }
void BIOS_InitScreen3Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border)
{
	g_MLTNAM = pnt;    // Address of pattern name table
	g_MLTCOL = ct;     // Address of colors table
	g_MLTCGP = pgt;    // Address of pattern generator table
	g_MLTATR = sat;    // Address of sprite attribute table
	g_MLTPAT = sgt;    // Address of sprite generator table
	g_FORCLR = text;   // Text color
	g_BAKCLR = bg;     // Background color
	g_BDRCLR = border; // Border color
	Call(R_INIMLT);
}

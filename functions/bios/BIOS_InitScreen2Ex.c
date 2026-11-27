// Function: BIOS_InitScreen2Ex
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen2Ex
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void BIOS_InitScreen2Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 width, u8 text, u8 back, u8 border) { g_GRPNAM = pnt; g_GRPCOL = ct; g_GRPCGP = pgt; g_GRPATR = sat; g_GRPPAT = sgt; g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen2(); }

// Function: BIOS_InitScreen2Color
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen2Color
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void BIOS_InitScreen2Color(u8 width, u8 text, u8 back, u8 border) { g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen2(); }

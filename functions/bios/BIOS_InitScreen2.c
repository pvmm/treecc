// Function: BIOS_InitScreen2
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen2
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void BIOS_InitScreen2() { Call(R_INIGRP); }

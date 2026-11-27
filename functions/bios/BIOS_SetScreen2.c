// Function: BIOS_SetScreen2
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetScreen2
// Switches VDP to SCREEN 2 mode. Wrapper for SETGRP routine.
inline void BIOS_SetScreen2() { Call(R_SETGRP); }

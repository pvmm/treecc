// Function: BIOS_InitPSG
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitPSG
// Initialises PSG and sets initial value for the PLAY statement. Wrapper for GICINI routine.
//> GICINI
//> Address  : #0090
//> Function : Initialises PSG and sets initial value for the PLAY statement
//> Registers: All
//> Remarks  : Interrupts must be disabled to call this routine
inline void BIOS_InitPSG() { DisableInterrupt(); Call(R_GICINI); EnableInterrupt(); }

// Function: BIOS_Reboot
// Header: bios.h
// Source: bios.c

// Function: BIOS_Reboot
// Tests RAM and sets RAM slot for the system.
// Wrapper for CHKRAM routine.
inline void BIOS_Reboot() { Call(R_CHKRAM); }

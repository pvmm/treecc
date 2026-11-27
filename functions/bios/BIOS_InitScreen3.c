// Function: BIOS_InitScreen3
// Header: bios.h
// Source: bios.c

// Function: BIOS_InitScreen3
// Switches to SCREEN 3 (multi-color screen 64*48 pixels). Wrapper for INIMLT routine.
inline void BIOS_InitScreen3() { Call(R_INIMLT); }

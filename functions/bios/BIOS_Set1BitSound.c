// Function: BIOS_Set1BitSound
// Header: bios.h
// Source: bios.c

// Function: BIOS_Set1BitSound
// Enable or disable 1-bit sound. Wrapper for CHGSND routine.
inline void BIOS_Set1BitSound(bool on) { return CallA(R_CHGSND, on); }

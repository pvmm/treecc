// Function: BIOS_IsPSGPlaying
// Header: bios.h
// Source: bios.c

// Function: BIOS_IsPSGPlaying
// Tests whether the PLAY statement is being executed as a background task.
//
// Return:
//   FALSE if the PLAY statement is not being executed as a background task.
inline bool BIOS_IsPSGPlaying() { return g_PLYCNT != 0; }

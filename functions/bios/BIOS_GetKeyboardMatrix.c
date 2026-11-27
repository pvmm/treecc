// Function: BIOS_GetKeyboardMatrix
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetKeyboardMatrix
// Returns the value of the specified line from the keyboard matrix. Wrapper for SNSMAT routine.
inline u8 BIOS_GetKeyboardMatrix(u8 line) { return CallAToA(R_SNSMAT, line); }

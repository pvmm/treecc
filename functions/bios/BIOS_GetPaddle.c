// Function: BIOS_GetPaddle
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetPaddle
// Returns the paddle position.
// Wrapper for GTPDL routine.
//
// Parameters:
//   num - Paddle number (1-12):
//> Paddle | Port1 | Port2
//> -------+-------+-------
//>    A   |   1   |   2
//>    B   |   3   |   4
//>    C   |   5   |   6
//>    D   |   7   |   8
//>    E   |   9   |   10
//>    F   |   11  |   12
//
// Return:
//   Paddle position [0:255]
inline u8 BIOS_GetPaddle(u8 num) { return CallAToA(R_GTPDL, num); }

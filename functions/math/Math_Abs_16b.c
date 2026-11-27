// Function: Math_Abs_16b
// Header: math.h
// Source: math.c

// Function: Math_Abs_16b
// Get absolute value of a signed 16-bits integer
//
// Parameters:
//   val - Signed 16-bits value
//
// Return:
//   Absolute value of the input
inline u16 Math_Abs_16b(i16 val) { return (((u16)(val) & 0x8000) ? ~((u16)(val) - 1) : (val)); }

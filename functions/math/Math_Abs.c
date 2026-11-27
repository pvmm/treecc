// Function: Math_Abs
// Header: math.h
// Source: math.c

// Function: Math_Abs
// Get absolute value of a signed 8-bits integer
//
// Parameters:
//   val - Signed 8-bits value
//
// Return:
//   Absolute value of the input
inline u8 Math_Abs(i8 val) { return (((u8)(val) & 0x80) ? ~((u8)(val) - 1) : (val)); }

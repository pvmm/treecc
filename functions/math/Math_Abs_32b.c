// Function: Math_Abs_32b
// Header: math.h
// Source: math.c

// Function: Math_Abs_32b
// Get absolute value of a signed 32-bits integer
//
// Parameters:
//   val - Signed 32-bits value
//
// Return:
//   Absolute value of the input
inline u32 Math_Abs_32b(i32 val) { return (((u32)(val) & 0x80000000) ? ~((u32)(val) - 1) : (val)); }

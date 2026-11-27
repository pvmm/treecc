// Function: Math_GetRandomRange8
// Header: math.h
// Source: math.c

// Function: Math_GetRandomRange8
// Generates 8-bit pseudorandom numbers between min and max-1 value.
//
// Parameters:
//   min - Minimum value of the pseudorandom number.
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between min and max-1.
inline u8 Math_GetRandomRange8(u8 min, u8 max) { return min + Math_GetRandom8() % (max - min); }

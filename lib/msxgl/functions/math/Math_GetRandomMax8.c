// Function: Math_GetRandomMax8
// Header: math.h
// Source: math.c

// Function: Math_GetRandomMax8
// Generates 8-bit pseudorandom numbers between 0 and max-1 value.
//
// Parameters:
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between 0 and max-1.
inline u8 Math_GetRandomMax8(u8 max) { return Math_GetRandom8() % max; }

// Function: Math_GetRandomMax16
// Header: math.h
// Source: math.c

// Function: Math_GetRandomMax16
// Generates 16-bit pseudorandom numbers between 0 and max-1 value.
//
// Parameters:
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between 0 and max-1.
inline u16 Math_GetRandomMax16(u16 max) { return Math_GetRandom16() % max; }

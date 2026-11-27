// Function: Math_GetRandomRange16
// Header: math.h
// Source: math.c

// Function: Math_GetRandomRange16
// Generates 16-bit pseudorandom numbers between min and max-1 value.
//
// Parameters:
//   min - Minimum value of the pseudorandom number.
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between min and max-1.
inline u16 Math_GetRandomRange16(u16 min, u16 max) { return min + Math_GetRandom16() % (max - min); }

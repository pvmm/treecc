// Function: Print_SetTabSize
// Header: print.h
// Source: print.c

// Function: Print_SetTabSize
// Set tabulation size in pixel.
//
// Parameters:
//   size - Size of the tabulation in pixel (must be a power of 2 like 16, 32, 64, ...)
inline void Print_SetTabSize(u8 size) { PRINT_DATA.TabSize = size; }

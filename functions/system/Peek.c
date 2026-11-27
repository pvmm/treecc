// Function: Peek
// Header: system.h
// Source: system.c

// Function: Peek
// Get the 8-bit value at the given address
//
// Parameters:
//   addr - Address to get
//
// Return:
//   8-bit value stored at the given address
inline u8 Peek(u16 addr) { return *(u8*)addr; }

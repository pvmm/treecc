// Function: Peek16
// Header: system.h
// Source: system.c

// Function: Peek16
// Get the 16-bit value at the given address
//
// Parameters:
//   addr - Address to get
//
// Return:
//   16-bit value stored at the given address
inline u16 Peek16(u16 addr) { return *(u16*)addr; }

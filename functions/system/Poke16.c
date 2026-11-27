// Function: Poke16
// Header: system.h
// Source: system.c

// Function: Poke16
// Set a 16-bit value at the given address
//
// Parameters:
//   addr - Address to set
//   val  - 16-bit value to set
inline void Poke16(u16 addr, u16 val) { *(u16*)addr = val; }

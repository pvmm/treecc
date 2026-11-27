// Function: Poke
// Header: system.h
// Source: system.c

// Function: Poke
// Set a 8-bit value at the given address
//
// Parameters:
//   addr - Address to set
//   val  - 8-bit value to set
inline void Poke(u16 addr, u8 val) { *(u8*)addr = val; }

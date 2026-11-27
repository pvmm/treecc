// Function: CallToA
// Header: system.h
// Source: system.c

// Function: CallToA
// Direct call a routine at a given address (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//
// Return:
//   Value stored in register A
inline u8 CallToA(u16 addr) { return ((u8(*)(void))addr)(); }

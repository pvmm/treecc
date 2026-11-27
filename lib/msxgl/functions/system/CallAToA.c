// Function: CallAToA
// Header: system.h
// Source: system.c

// Function: CallAToA
// Direct call a routine at a given address with a 8-bits parameter in register A (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   a    - Function parameter to put in register A
//
// Return:
//   Value stored in register A
inline u8 CallAToA(u16 addr, u8 a) { return ((u8(*)(u8))addr)(a); }

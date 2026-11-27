// Function: CallA
// Header: system.h
// Source: system.c

// Function: CallA
// Direct call a routine at a given address with a 8-bits parameter in register A (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   a    - Function parameter to put in register A
inline void CallA(u16 addr, u8 a) { ((void(*)(u8))addr)(a); }

// Function: CallHL
// Header: system.h
// Source: system.c

// Function: CallHL
// Direct call a routine at a given address with a 16-bits parameter in register HL (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   val  - Function parameter to put in register HL
inline void CallHL(u16 addr, u16 hl) { ((void(*)(u16))addr)(hl); }

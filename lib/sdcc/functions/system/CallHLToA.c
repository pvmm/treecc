// Function: CallHLToA
// Header: system.h
// Source: system.c

// Function: CallHLToA
// Direct call a routine at a given address with a 16-bits parameter in register HL (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   val  - Function parameter to put in register HL
//
// Return:
//   Value stored in register A
inline u8 CallHLToA(u16 addr, u16 hl) { return ((u8(*)(u16))addr)(hl); }

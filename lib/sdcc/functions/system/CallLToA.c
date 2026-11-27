// Function: CallLToA
// Header: system.h
// Source: system.c

// Function: CallLToA
// Direct call a routine at a given address with a 8-bits parameter in register L (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   l    - Function parameter to put in register L
//
// Return:
//   Value stored in register A
inline u8 CallLToA(u16 addr, u8 l) { return ((call_l_a_t)addr)(l); }

// Function: CallL
// Header: system.h
// Source: system.c

// Function: CallL
// Direct call a routine at a given address with a 8-bits parameter in register L (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   l    - Function parameter to put in register L
inline void CallL(u16 addr, u8 l) { ((call_l_t)addr)(l); }

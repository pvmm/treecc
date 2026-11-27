// Function: Call
// Header: system.h
// Source: system.c

// Function: Call
// Direct call a routine at a given address (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
inline void Call(u16 addr) { ((void(*)(void))addr)(); }

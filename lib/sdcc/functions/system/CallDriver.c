// Function: CallDriver
// Header: system.h
// Source: system.c

// Function: CallDriver
// Direct call a routine at a given address with a 8-bits parameter in register A (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address of the driver
//   val  - 8-bit value to transfer to driver's main function
//
// Return:
//   Value returned by the driver
inline u8 CallDriver(u16 addr, u8 val) { return ((u8(*)(u8))addr)(val); }

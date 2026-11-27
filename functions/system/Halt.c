// Function: Halt
// Header: system.h
// Source: system.c

// Function: Halt
// Pause the CPU until a new interruption occured
inline void Halt() { __asm__("halt"); }

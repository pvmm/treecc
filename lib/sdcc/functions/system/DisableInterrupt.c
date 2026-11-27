// Function: DisableInterrupt
// Header: system.h
// Source: system.c

// Function: DisableInterrupt
// Disable interruption
inline void DisableInterrupt() { __asm__("di"); }

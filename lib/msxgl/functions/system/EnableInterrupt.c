// Function: EnableInterrupt
// Header: system.h
// Source: system.c

// Function: EnableInterrupt
// Enable interruption
inline void EnableInterrupt() { __asm__("ei"); }

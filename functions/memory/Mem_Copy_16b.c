// Function: Mem_Copy_16b
// Header: memory.h
// Source: memory.c

// Function: Mem_Copy_16b
// Copy a 16-bits memory block from a source address to another (minimal size of 1 word / 2 byte).
//
// Parameters:
//   src - Source address
//   dst - Destination address
//   size - The size of 16-bits data to copy. Note: size can be 1 to 32767. 0 mean 32768
inline void Mem_Copy_16b(const void* src, void* dest, u16 size) { Mem_Copy(src, dest, size * 2); }

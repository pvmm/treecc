// Function: Mem_FastCopy_16b
// Header: memory.h
// Source: memory.c

// Function: Mem_FastCopy_16b
// Fast copy a 16-bits memory block from a source address to a destination.
// Use 16 unrolled-LDI loop.
//
// Parameters:
//   src - Source address
//   dst - Destination address
//   size - The size of data to copy. Note: size can be 1 to 32767. 0 mean 32768
inline void Mem_FastCopy_16b(const void* src, void* dest, u16 size) { Mem_FastCopy(src, dest, size * 2); }

// Function: Mem_FastCopy
// Header: memory.h
// Source: memory.c

// Function: Mem_FastCopy
// Fast copy a memory block from a source address to a destination.
// Use 16 unrolled-LDI loop.
//
// Parameters:
//   src - Source address
//   dst - Destination address
//   size - The size of data to copy. Note: A size of 0 mean 65536
void Mem_FastCopy(const void* src, void* dest, u16 size);

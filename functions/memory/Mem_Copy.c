// Function: Mem_Copy
// Header: memory.h
// Source: memory.c

// Function: Mem_Copy
// Copy a memory block from a source address to another (minimal size of 1 byte).
//
// Parameters:
//   src - Source address
//   dst - Destination address
//   size - The size of data to copy. Note: A size of 0 mean 65536
void Mem_Copy(const void* src, void* dest, u16 size);

// Function: Mem_FastSet
// Header: memory.h
// Source: memory.c

// Function: Mem_FastSet
// Fast fill a memory chunk with a given value (minimal size of 2 bytes).
// Use 16 unrolled-LDI loop.
//
// Parameters:
//   val - Value to write
//   dst - Destination address
//   size - The size of data to fill. Note: A size of 0 mean 65536
void Mem_FastSet(u8 val, void* dest, u16 size);

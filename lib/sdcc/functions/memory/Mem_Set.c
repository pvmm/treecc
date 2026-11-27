// Function: Mem_Set
// Header: memory.h
// Source: memory.c

// Function: Mem_Set
// Fill a memory block with a given 8-bits value (minimal size of 2 bytes).
//
// Parameters:
//   val - Value to write
//   dst - Destination address
//   size - The size of data to fill. Note: A size of 0 mean 65536
void Mem_Set(u8 val, void* dest, u16 size);

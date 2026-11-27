// Function: Mem_Set_16b
// Header: memory.h
// Source: memory.c

// Function: Mem_Set_16b
// Fill a memory block with a given 16-bits value (minimal size of 2 bytes).
//
// Parameters:
//   val - Value to write
//   dst - Destination address
//   size - The size of data to fill. Note: size can be 2 to 32766
void Mem_Set_16b(u16 val, void* dest, u16 size);

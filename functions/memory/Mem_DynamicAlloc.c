// Function: Mem_DynamicAlloc
// Header: memory.h
// Source: memory.c

// Function: Mem_DynamicAlloc
// Allocate a memory chunk from the dynamic memory buffer
//
// Parameters:
//   size - Size of the memory chunk to allocate. Note: Even if you can allocate 0 sized chunk, this is useless
//
// Return:
//   Address of the allocated memory chunk (or NULL if no enough continuous space have been found)
void* Mem_DynamicAlloc(u16 size);

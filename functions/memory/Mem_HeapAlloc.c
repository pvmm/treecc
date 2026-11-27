// Function: Mem_HeapAlloc
// Header: memory.h
// Source: memory.c

// Function: Mem_HeapAlloc
// Alloc a part of the heap.
//
// Parameters:
//   size - The size of data to allocate
//
// Return:
//   Pointer to allocated heap chunk
inline void* Mem_HeapAlloc(u16 size) { u16 addr = g_HeapStartAddress; g_HeapStartAddress += size; return (void*)addr; }

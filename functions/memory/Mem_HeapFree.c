// Function: Mem_HeapFree
// Header: memory.h
// Source: memory.c

// Function: Mem_HeapFree
// Free the last allocated area of the heap.
//
// Parameters:
//   size - The size of data to allocate
inline void Mem_HeapFree(u16 size) { g_HeapStartAddress -= size; }

// Function: Mem_DynamicInitializeHeap
// Header: memory.h
// Source: memory.c

// Function: Mem_DynamicInitializeHeap
// Allocates a static memory chunk in the heap which can then be used to allocate chunks dynamically.
//
// Parameters:
//   size - Size of the dynamic allocator buffer (4 bytes chunk headers will consume some of this space)
inline void Mem_DynamicInitializeHeap(u16 size) { Mem_DynamicInitialize(Mem_HeapAlloc(size), size); }

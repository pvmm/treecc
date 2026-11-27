// Function: Mem_GetHeapSize
// Header: memory.h
// Source: memory.c

// Function: Mem_GetHeapSize
// Get the amount of free space in the heap.
//
// Return:
//   Get the current size of the heap (between heap top address and stack bottom one)
inline u16 Mem_GetHeapSize() { return Mem_GetStackAddress() - Mem_GetHeapAddress(); }

// Function: Mem_GetHeapAddress
// Header: memory.h
// Source: memory.c

// Function: Mem_GetHeapAddress
// Get the current address of the heap top (higher address).
//
// Return:
//   Current heap top address
inline u16 Mem_GetHeapAddress() { return g_HeapStartAddress; }

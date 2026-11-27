// Function: Mem_GetDynamicSize
// Header: memory.h
// Source: memory.c

// Function: Mem_GetDynamicSize
// Get the size of a dynamically allocated memory chunk
//
// Parameters:
//   ptr - Address of the allocated memory chunk
//
// Return:
//   Size of the allocated memory chunk
inline u16 Mem_GetDynamicSize(void* ptr) { MemChunkHeader* chunk = (MemChunkHeader*)((u16)ptr - sizeof(MemChunkHeader)); return chunk->Size; }

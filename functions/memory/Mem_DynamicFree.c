// Function: Mem_DynamicFree
// Header: memory.h
// Source: memory.c

// Function: Mem_DynamicFree
// Free a memory chunk from the dynamic memory buffer
//
// Parameters:
//   ptr - Address of the allocated memory chunk
void Mem_DynamicFree(void* ptr);

//-----------------------------------------------------------------------------
// Free a memory chunk from the dynamic memory buffer
void Mem_DynamicFree(void* ptr)
{
	MemChunkHeader* chunk = (MemChunkHeader*)((u16)ptr - sizeof(MemChunkHeader));
	chunk->Size |= MEM_CHUNK_FREE;
	Mem_DynamicMerge();
}

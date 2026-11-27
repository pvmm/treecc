// Function: Mem_DynamicInitialize
// Header: memory.h
// Source: memory.c

// Function: Mem_DynamicInitialize
// Allocates a static memory chunk which can then be used to allocate chunks dynamically.
// Memory chunk can be in any pages but:
// - The memory space have to be continuous from a Z80 perspective.
// - You have to ensure that all memory segments are selected in their proper place when allocating or releasing memory.
//
// Parameters:
//   base - Base address of the dynamic allocator memory buffer
//   size - Size of the dynamic allocator buffer (4 bytes chunk headers will consume some of this space)
void Mem_DynamicInitialize(void* base, u16 size);

//-----------------------------------------------------------------------------
// Allocates a static memory block which can then be used to allocate chunks dynamically.
void Mem_DynamicInitialize(void* base, u16 size)
{
	g_MemChunkRoot = (MemChunkHeader*)base;
	g_MemChunkRoot->Size = (size - sizeof(MemChunkHeader)) | MEM_CHUNK_FREE;
	g_MemChunkRoot->Next = NULL;
}

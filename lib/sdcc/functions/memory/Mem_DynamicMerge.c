// Function: Mem_DynamicMerge
// Source: memory.c

//-----------------------------------------------------------------------------
// Merge contiguous empty memory blocks
void Mem_DynamicMerge()
{
	MemChunkHeader* chunk = g_MemChunkRoot;
	while (chunk)
	{
		MemChunkHeader* nextChunk = chunk->Next;
		if ((nextChunk != NULL) && (chunk->Size & MEM_CHUNK_FREE) && (nextChunk->Size & MEM_CHUNK_FREE))
		{
			chunk->Size += (nextChunk->Size & ~MEM_CHUNK_FREE) + sizeof(MemChunkHeader);
			chunk->Next = nextChunk->Next; // Stay on the same chunk to merge following free chunks too
		}
		else
			chunk = nextChunk;
	}
}

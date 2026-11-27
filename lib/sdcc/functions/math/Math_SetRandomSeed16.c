// Function: Math_SetRandomSeed16
// Header: math.h
// Source: math.c

// Function: Math_SetRandomSeed16
// Initialize random generator seed
//
// Parameters:
//   seed - Initial seed value for the generator
void Math_SetRandomSeed16(u16 seed);

//-----------------------------------------------------------------------------
// Initialize random generator seed
void Math_SetRandomSeed16(u16 seed)
{
	g_RandomSeed16_1 = seed;
	g_RandomSeed16_2 = seed | 0x0001;
}

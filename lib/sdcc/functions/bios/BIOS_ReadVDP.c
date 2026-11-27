// Function: BIOS_ReadVDP
// Header: bios.h
// Source: bios.c

// Function: BIOS_ReadVDP
// Reads data from the VDP-register.
inline u8 BIOS_ReadVDP(u8 reg)
{
	switch (reg)
	{
	case 0: return g_RG0SAV;
	case 1: return g_RG1SAV;
	case 2: return g_RG2SAV;
	case 3: return g_RG3SAV;
	case 4: return g_RG4SAV;
	case 5: return g_RG5SAV;
	case 6: return g_RG6SAV;
	case 7: return g_RG7SAV;
	}
}

// Function: BIOS_IsKeyPressed
// Header: bios.h
// Source: bios.c

// Function: BIOS_IsKeyPressed
// Check if the given key is pressed
inline bool BIOS_IsKeyPressed(u8 key) {	return (g_NEWKEY[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0; }

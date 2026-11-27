// Function: MSXMusic_GetSlotId
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_GetSlotId
// Get the Slot ID of the first MSX-Music chip found
// <MSXMusic_Detect> or <MSXMusic_Initialize> must be called first
//
// Return:
//   Slot ID if found or SLOT_NOTFOUND otherwise
inline u8 MSXMusic_GetSlotId() { return g_MSXMusic_SlotId; }

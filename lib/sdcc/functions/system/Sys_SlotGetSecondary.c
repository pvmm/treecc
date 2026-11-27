// Function: Sys_SlotGetSecondary
// Header: system.h
// Source: system.c

// Function: Sys_SlotGetSecondary
// Get secondary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
//
// Return:
//   The secondary slot of the slot ID
inline u8 Sys_SlotGetSecondary(u8 slotId) { return SLOT_SEC(slotId); }

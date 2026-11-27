// Function: Sys_SlotGetPrimary
// Header: system.h
// Source: system.c

// Function: Sys_SlotGetPrimary
// Get primary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
//
// Return:
//   The primary slot of the slot ID
inline u8 Sys_SlotGetPrimary(u8 slotId) { return SLOT_PRIM(slotId); }

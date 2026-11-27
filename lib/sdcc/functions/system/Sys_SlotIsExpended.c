// Function: Sys_SlotIsExpended
// Header: system.h
// Source: system.c

// Function: Sys_SlotIsExpended
// Check expended flag from slot ID
//
// Parameters:
//   slotId - The slot ID
//
// Return:
//   FALSE if the slot is not expended
inline bool Sys_SlotIsExpended(u8 slotId) { return IS_SLOT_EXP(slotId); }

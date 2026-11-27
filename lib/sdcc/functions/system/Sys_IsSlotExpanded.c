// Function: Sys_IsSlotExpanded
// Header: system.h
// Source: system.c

// Function: Sys_IsSlotExpanded
// Check if slot is expanded
//
// Parameters:
//   slot - The slot to check
//
// Return:
//   FALSE if the slot is not expanded
inline bool Sys_IsSlotExpanded(u8 slot) { return g_EXPTBL[slot] & SLOT_EXP; }

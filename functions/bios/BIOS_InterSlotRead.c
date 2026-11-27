// Function: BIOS_InterSlotRead
// Header: bios.h
// Source: bios.c

// Function: BIOS_InterSlotRead
// Reads the value of an address in another slot.
// Wrapper for RDSLT routine.
u8 BIOS_InterSlotRead(u8 slot, u16 addr);

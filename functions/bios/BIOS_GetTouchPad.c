// Function: BIOS_GetTouchPad
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetTouchPad
// Returns the touch pad status.
// Wrapper for GTPAD routine.
//
// Parameters:
//   entry - Function call number. Fetch device data first, then read:
//> MSX1:
//>   0 = Fetch touch pad data from port 1 (#FF if available)
//>   1 = Read X-position
//>   2 = Read Y-position
//>   3 = Read touchpad status from port 1 (#FF if pressed
//>   4 = Fetch touch pad data from port 2 (#FF if available)
//>   5 = Read X-position
//>   6 = Read Y-position
//>   7 = Read touchpad status from port 2 (#FF if pressed)
//> MSX2/2+/turbo R:
//>   8 = Fetch light pen data (#FF if available)
//>   9 = Read X-position
//>  10 = Read Y-position
//>  11 = Read light pen status (#FF if pressed)
//>  12 = Fetch mouse data from port 1 (#FF if available)
//>  13 = Read X-position
//>  14 = Read Y-position
//>  15 = Read mouse button status from port 1 (#FF if pressed)
//>  16 = Fetch mouse data from port 2 (#FF if available)
//>  17 = Read X-position
//>  18 = Read Y-position
//>  19 = Read mouse button status from port 2 (#FF if pressed)
//
// Return:
//   Value depending on entry parameter
inline u8 BIOS_GetTouchPad(u8 entry) { return CallAToA(R_GTPAD, entry); }

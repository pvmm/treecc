// Function: Sys_CheckSlot
// Header: system.h
// Source: system.c

// Function: Sys_CheckSlot
// Check all slots with a given callback function
//
// Parameters:
//   cb - Callback function to call for each slot
//
// Return:
//   The slot ID that match the callback function
u8 Sys_CheckSlot(CheckSlotCallback cb);

//-----------------------------------------------------------------------------
// Check all slots with a given callback function
u8 Sys_CheckSlot(CheckSlotCallback cb)
{
	for (u8 slot = 0; slot < 4; ++slot)
	{
		if (g_EXPTBL[slot] & SLOT_EXP)
		{
			for (u8 sub = 0; sub < 4; ++sub)
			{
				u8 slotId = SLOTEX(slot, sub);
				if (cb(slotId))
					return slotId;
			}
		}
		else if (cb(slot))
			return slot;
	}
	return SLOT_NOTFOUND;
}

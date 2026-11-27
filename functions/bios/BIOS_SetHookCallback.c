// Function: BIOS_SetHookCallback
// Source: bios.c

//-----------------------------------------------------------------------------
// Set a safe hook jump to given function
void BIOS_SetHookCallback(u16 hook, callback cb)
{
	u8 slot = Sys_GetPageSlot((u16)cb >> 14);
	BIOS_SetHookInterSlotCallback(hook, slot, cb);
}

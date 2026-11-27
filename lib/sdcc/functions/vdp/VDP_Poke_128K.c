// Function: VDP_Poke_128K
// Header: vdp.h
// Source: vdp.c

	// Function: VDP_Poke_128K
	// Write a value to VRAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   val		- Value to write in VRAM
	//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
	//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
	void VDP_Poke_128K(u8 val, u16 destLow, u8 destHigh);

//-----------------------------------------------------------------------------
// Write a value to VRAM [MSX2/2+/TR]
//
// Parameters:
//   val		- Value to write in VRAM
//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
void VDP_Poke_128K(u8 val, u16 destLow, u8 destHigh)
{
	val;      // A
	destLow;  // DE
	destHigh; // SP+2 => IY+0

	__asm
		ld		b, a

		ld		iy, #2
		add		iy, sp

		// Set destination address bits 14~16 to R#14
		ld		a, 0(iy)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		add		a, c
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a			// RegPort = (destHigh << 2) + (destLow >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14);

		// Set destination address bits 0~7 to port #1
		ld		a, e
		out		(P_VDP_ADDR), a			// AddrPort = (destLow & 0x00FF);

		// Set destination address bits 8~13 to port #1
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((destLow >> 8) & 0x3F) + F_VDP_WRIT;

		// Write data
		ld		a, b					// 21cc
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		out		(P_VDP_DATA), a			// 12cc		DataPort = val

	__endasm;
}

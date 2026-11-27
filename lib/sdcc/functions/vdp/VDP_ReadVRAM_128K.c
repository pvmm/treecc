// Function: VDP_ReadVRAM_128K
// Header: vdp.h
// Source: vdp.c

	// Function: VDP_ReadVRAM_128K
	// Read data from VRAM to RAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
	//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
	//   dest		- Desitation data address in RAM
	//   count		- Number of byte to copy from VRAM. Note: A count of 0 mean 65536
	void VDP_ReadVRAM_128K(u16 srcLow, u8 srcHigh, u8* dest, u16 count);

//-----------------------------------------------------------------------------
// Read data from VRAM to RAM [MSX2/2+/TR]
//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
//   dest		- Desitation data address in RAM
//   count		- Number of byte to copy from VRAM
void VDP_ReadVRAM_128K(u16 srcLow, u8 srcHigh, u8* dest, u16 count)
{
	srcLow;		// HL
	srcHigh;	// SP+2 => IY+0
	dest;		// SP+3 => IY+1
	count;		// SP+5 => IY+3

	__asm
		ld		iy, #2
		add		iy, sp
		// Setup address register
		ld		a, 0(iy)				// R#14 A16 (move to bit 2)
		add		a, a
		add		a, a
		ld		c, a					// Backup A16
		ld		a, h					// R#14 A14-A15 (move to bit 1&0)
		rlca
		rlca
		and		a, #0x03
		add		a, c					// Merge R#14 A14-A16
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcHigh << 2) + (srcLow >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort  = (u8)(VDP_REG(14))
		ld		a, l
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0xFF)
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ
		// while (count--) *src++ = DataPort;
		ld		l, 1(iy)				// source address
		ld		h, 2(iy)
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 3(iy)				// count LSB
		and		a
		jp		z, rd_loop_init			// skip LSB
		ld		b, a					// retreive (count & 0x00FF) bytes
		inir
		// Handle count MSB
	rd_loop_init:
		ld		a, 4(iy)				// count MSB
	rd_loop_start:
		and		a
		jp		z, rd_loop_end			// finished
		ld		b, #0					// retreive 256 bytes packages
		inir
		dec		a
		jp		rd_loop_start

	rd_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

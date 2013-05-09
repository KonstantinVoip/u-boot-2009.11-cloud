/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc.
 *
 * Authors:	Suresh PV <pala@freescale.com>,
 *		Kumar Gopalpet <Sandeep.Kumar@freescale.com>,
 *		Chandu Drona Pratap <D.P.Chandu@freescale.com>,
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* File to Generate CRC */
/*
 *	1 + x1 + x2 + x4 + x5 + x7 + x8 + x10 + x11
 *	+ x12 + x16 + x22 + x23 + x26 + x32
*/

#include <linux/types.h>	/* for size_t */

/* CRC table */
unsigned long xor_mask[] = {
	0x690ce0ee,
	0x34867077,
	0x9823b6e0,
	0x4c11db70,
	0x2608edb8,
	0x130476dc,
	0x09823b6e,
	0x04c11db7,
	0x80000000,
	0x40000000,
	0x20000000,
	0x10000000,
	0x08000000,
	0x04000000,
	0x02000000,
	0x01000000,
	0x00800000,
	0x00400000,
	0x00200000,
	0x00100000,
	0x00080000,
	0x00040000,
	0x00020000,
	0x00010000,
	0x00008000,
	0x00004000,
	0x00002000,
	0x00001000,
	0x00000800,
	0x00000400,
	0x00000200,
	0x00000100
};

unsigned long data_mask[] = {
	0x690ce0ee,
	0x34867077,
	0x9823b6e0,
	0x4c11db70,
	0x2608edb8,
	0x130476dc,
	0x09823b6e,
	0x04c11db7
};

u32 crc32(const unsigned char *preload_array, u32 size)
{
	u32 i = 0;
	u32 j = 0;
	u32 preload_crc = 0xffffffff;
	u32 new_crc = 0x0;
	u32 temp_crc = 0x0;
	u32 temp_data = 0x0;

	for (i = 0; i < size; i++) {
		new_crc = 0;

		for (j = 0; j < 32; j++) {
			if (((preload_crc >> (31 - j)) & 0x1) == 1)
				temp_crc = xor_mask[j];
			else
				temp_crc = 0;
			new_crc ^= temp_crc;
		}
		for (j = 0; j < 8; j++) {
			if (((preload_array[i] >> (7 - j)) & 0x1) == 1)
				temp_data = data_mask[j];
			 else
				temp_data = 0;

			new_crc ^= temp_data;
		}
		preload_crc = new_crc;
	}

	return preload_crc;
}

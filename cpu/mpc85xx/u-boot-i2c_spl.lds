/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc.
 *
 * Authors:	Suresh PV <pala@freescale.com>,
 *		Kumar Gopalpet <Sandeep.Kumar@freescale.com>,
 *		Chandu Drona Pratap <D.P.Chandu@freescale.com>,
 *		Priyanka Jain <Priyanka.Jain@freescale.com>
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

OUTPUT_ARCH(powerpc:common)
SECTIONS
{
  . = 0x7f000;
  .reloc   :
  {
    *(.got)
    _GOT2_TABLE_ = .;
    *(.got2)
    _FIXUP_TABLE_ = .;
    *(.fixup)
  }
  __got2_entries = (_FIXUP_TABLE_ - _GOT2_TABLE_) >> 2;
  __fixup_entries = (. - _FIXUP_TABLE_) >> 2;

  .data    :
  {
    *(.rodata*)
    *(.data*)
    *(.sdata*)
  }

  _edata  =  .;

  .text      :
  { _sttext = .;
    start.o(.text)
    i2c_boot.o(.text)
    serial.o(.text)
    ns16550.o(.text)
    . = 0xffc - 0x600;
    start.o	(.bootpg)
    *(.resetvec)
    *(.text)

  }

  _etext = .;
  __init_end = .;
  __bss_start = .;
  .bss       :
  {
    *(.sbss)
    *(.scommon)
    *(.dynbss)
    *(.bss)
    *(COMMON)
  }
  _end = . ;
  PROVIDE (end = .);
}
ENTRY(_start)

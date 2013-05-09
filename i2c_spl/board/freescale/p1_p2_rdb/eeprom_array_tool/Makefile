#
# Copyright 2009-2010 Freescale Semiconductor, Inc.
#
# Authors:    Suresh PV <pala@freescale.com>,
#             Kumar Gopalpet <Sandeep.Kumar@freescale.com>,
#             Chandu Drona Pratap <D.P.Chandu@freescale.com>,
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

# The tools are compiled with host gcc.

CC=gcc

CFLAGS= -D__KERNEL__   -I../../../../../include  -Wall  -g

SRCS=	pre_init.c	\
	crc32.c

OBJS=$(SRCS:%.c=%.o)

%.o:%.c
	$(CC) $(CFLAGS) -c -g $< -o $@


all:		gen_eeprom_array

gen_eeprom_array:	$(OBJS)
		$(CC)  -o $@ $(OBJS)

clean:
	rm -rf *.o gen_eeprom_array

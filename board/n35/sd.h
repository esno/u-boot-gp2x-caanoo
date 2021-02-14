/*
 *      A     EEEE  SSSS   OOO  PPPP 
 *     A A    E     S     O   O P   P
 *    AAAAA   EEEE  SSSS  O   O PPPP
 *   A     A  E         S O   O P
 *   A     A  EEEE  SSSS   OOO  P
 *
 *  An Entertainment Solution On a Platform (AESOP) is a completely Open Source 
 *  based graphical user environment and suite of applications for PDAs and other 
 *  devices running Linux. It is included in various embedded Linux distributions 
 *  such as OpenZaurus - http://www.aesop-embedded.org 
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *
 *	Title           : sd.h
 *	Author          : 
 *	Created date    : 2005. 06. 26. 23:06:29 KST
 *	Description     : 
 *
 */ 
#include "mes_sdhc.h" 

int sd_read(unsigned char *rxd, unsigned long start_lba, unsigned long read_sectors);
int pollux_sd_init(void);




#define VIR_BASEADDR_SD			0xC0009800 // sd0

struct pollux_sdi_host {
	int         init;
	int         clock; /* clock source , GET_PCLK */
	int         channel;
	int         opcode; /* command */
	int         arg;    /* argument */
	u32         resp[4];
	u32         longrsp;
	
	int			size;		/* Total size of transfer */
	int			cd_state;	/* Card state: 1 when card is inserted, 0 when card is removed */
	int			readonly;	/* Write protect state: 1 when write-protection is enabled, 0 when disabled */
	u8          *buffer;
	int          length; // data read/write length ==> sg->length

	unsigned char		bus_width;	/* Current bus width */

	struct MES_SDHC_RegisterSet *sdi;
};


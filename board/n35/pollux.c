/*
 * For the MagicEyes POLLUX board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include "mach-types.h"
#include "board.h"
#include "sd.h"

DECLARE_GLOBAL_DATA_PTR;

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
			  "subs %0, %1, #1\n"
			  "bne 1b":"=r" (loops):"0" (loops));
}

int board_init (void)
{
	gd->bd->bi_arch_number = MACH_TYPE_POLLUX;
	gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x00000100); // ghcstop fix

	BOARD_Initialize();

	return 0;
}

int misc_init_r (void)
{
	return (0);
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size  = PHYS_SDRAM_1_SIZE;

	return 0;
}


//================================================================

struct systypes {
        unsigned char type;
        char *name;
};

struct systypes i386_sys_types[] = {
	{0x00,  "Empty"},
	{0x01,  "FAT12"},
	{0x02,  "XENIX root"},
	{0x03,  "XENIX usr"},
	{0x04,  "FAT16 <32M"},
	{0x05,  "Extended"},		/* DOS 3.3+ extended partition */
	{0x06,  "FAT16"},		/* DOS 16-bit >=32M */
	{0x07,  "HPFS/NTFS"},	/* OS/2 IFS, eg, HPFS or NTFS or QNX */
	{0x08,  "AIX"},		/* AIX boot (AIX -- PS/2 port) or SplitDrive */
	{0x09,  "AIX bootable"},	/* AIX data or Coherent */
	{0x0a,  "OS/2 Boot Manager"},/* OS/2 Boot Manager */
	{0x0b,  "W95 FAT32"},
	{0x0c,  "W95 FAT32 (LBA)"},/* LBA really is `Extended Int 13h' */
	{0x0e,  "W95 FAT16 (LBA)"},
	{0x0f,  "W95 Ext'd (LBA)"},
	{0x10,  "OPUS"},
	{0x11,  "Hidden FAT12"},
	{0x12,  "Compaq diagnostics"},
	{0x14,  "Hidden FAT16 <32M"},
	{0x16,  "Hidden FAT16"},
	{0x17,  "Hidden HPFS/NTFS"},
	{0x18,  "AST SmartSleep"},
	{0x1b,  "Hidden W95 FAT32"},
	{0x1c,  "Hidden W95 FAT32 (LBA)"},
	{0x1e,  "Hidden W95 FAT16 (LBA)"},
	{0x24,  "NEC DOS"},
	{0x39,  "Plan 9"},
	{0x3c,  "PartitionMagic recovery"},
	{0x40,  "Venix 80286"},
	{0x41,  "PPC PReP Boot"},
	{0x42,  "SFS"},
	{0x4d,  "QNX4.x"},
	{0x4e,  "QNX4.x 2nd part"},
	{0x4f,  "QNX4.x 3rd part"},
	{0x50,  "OnTrack DM"},
	{0x51,  "OnTrack DM6 Aux1"},	/* (or Novell) */
	{0x52,  "CP/M"},		/* CP/M or Microport SysV/AT */
	{0x53,  "OnTrack DM6 Aux3"},
	{0x54,  "OnTrackDM6"},
	{0x55,  "EZ-Drive"},
	{0x56,  "Golden Bow"},
	{0x5c,  "Priam Edisk"},
	{0x61,  "SpeedStor"},
	{0x63,  "GNU HURD or SysV"},	/* GNU HURD or Mach or Sys V/386 (such as ISC UNIX) */
	{0x64,  "Novell Netware 286"},
	{0x65,  "Novell Netware 386"},
	{0x70,  "DiskSecure Multi-Boot"},
	{0x75,  "PC/IX"},
	{0x80,  "Old Minix"},	/* Minix 1.4a and earlier */
	{0x81,  "Minix / old Linux"},/* Minix 1.4b and later */
	{0x82,  "Linux swap / Solaris"},
	{0x83,  "Linux"},
	{0x84,  "OS/2 hidden C: drive"},
	{0x85,  "Linux extended"},
	{0x86,  "NTFS volume set"},
	{0x87,  "NTFS volume set"},
	{0x88,  "Linux plaintext"},
	{0x8e,  "Linux LVM"},
	{0x93,  "Amoeba"},
	{0x94,  "Amoeba BBT"},	/* (bad block table) */
	{0x9f,  "BSD/OS"},		/* BSDI */
	{0xa0,  "IBM Thinkpad hibernation"},
	{0xa5,  "FreeBSD"},		/* various BSD flavours */
	{0xa6,  "OpenBSD"},
	{0xa7,  "NeXTSTEP"},
	{0xa8,  "Darwin UFS"},
	{0xa9,  "NetBSD"},
	{0xab,  "Darwin boot"},
	{0xb7,  "BSDI fs"},
	{0xb8,  "BSDI swap"},
	{0xbb,  "Boot Wizard hidden"},
	{0xbe,  "Solaris boot"},
	{0xbf,  "Solaris"},
	{0xc1,  "DRDOS/sec (FAT-12)"},
	{0xc4,  "DRDOS/sec (FAT-16 < 32M)"},
	{0xc6,  "DRDOS/sec (FAT-16)"},
	{0xc7,  "Syrinx"},
	{0xda,  "Non-FS data"},
	{0xdb,  "CP/M / CTOS / ..."},/* CP/M or Concurrent CP/M or
					   Concurrent DOS or CTOS */
	{0xde,  "Dell Utility"},	/* Dell PowerEdge Server utilities */
	{0xdf,  "BootIt"},		/* BootIt EMBRM */
	{0xe1,  "DOS access"},	/* DOS access or SpeedStor 12-bit FAT
					   extended partition */
	{0xe3,  "DOS R/O"},		/* DOS R/O or SpeedStor */
	{0xe4,  "SpeedStor"},	/* SpeedStor 16-bit FAT extended
					   partition < 1024 cyl. */
	{0xeb,  "BeOS fs"},
	{0xee,  "EFI GPT"},		/* Intel EFI GUID Partition Table */
	{0xef,  "EFI (FAT-12/16/32)"},/* Intel EFI System Partition */
	{0xf0,  "Linux/PA-RISC boot"},/* Linux/PA-RISC boot loader */
	{0xf1,  "SpeedStor"},
	{0xf4,  "SpeedStor"},	/* SpeedStor large partition */
	{0xf2,  "DOS secondary"},	/* DOS 3.3+ secondary */
	{0xfd,  "Linux raid autodetect"},/* New (2.2.x) raid partition with
					       autodetect using persistent
					       superblock */
	{0xfe,  "LANstep"},		/* SpeedStor >1024 cyl. or LANstep */
	{0xff,  "BBT"},		/* Xenix Bad Block Table */
	{ 0, 0 }
};


void partition_info( unsigned char part_type )
{
	int i;
	
	for( i=1; i386_sys_types[i].name != NULL; i++ )
	{
		if( part_type == i386_sys_types[i].type )
		{
			printf("Partition type: %s\n", i386_sys_types[i].name);
			break;
		}
	}
	
}


unsigned char read_data[4096]; // buffer for SD read/write
extern int mmc_init_once;

int board_late_init(void)
{	
	int ret = 0;
	int mbr_exist = 0;
	
	if(!fw_check()) goto skip_fwupdate;
	
	
	if( pollux_sd_init() < 0 )
	{
		printf("pollux sd block initialize error\n");
		goto skip_fwupdate;
	}
	
	ret = sd_read(read_data, 0, 1); // single block read test
	//ret = sd_read(read_data, 0, 8); // multi block read test
	if( ret < 0 )
	{
		printf("sd_read error\n");
		goto skip_fwupdate;
	}
#if 0   /* org */
	
	if( (read_data[511]==0xAA) && (read_data[510]==0x55) ) // MBR or Boot sector signature
	{
		
	    if ( (read_data[450] == 0x01) || (read_data[450] == 0x04) || (read_data[450] == 0x06) ||
	         (read_data[450] == 0x0B) ||   // FAT32 Partition 
	         (read_data[450] == 0x0C) ||   // FAT32 Partition 
	         (read_data[450] == 0x55) )    // FAT32 Partition 
	    {
	         mbr_exist = 1;
	         //printf("partition type = 0x%02x\n", read_data[450] );
	    }
	    printf("partition type = 0x%02x, ", read_data[450] );
		partition_info( read_data[450] );
		
		printf("MBR or Bootsector read done: %s\n", (mbr_exist)? "(with MBR)":"(without MBR)" );
	} 
#else
	
	if(read_data[511]==0xAA && read_data[510]==0x55)
	{
		if( (read_data[0x36] == 'F') && (read_data[0x37] == 'A') && (read_data[0x38] == 'T'))
	    {
			/* PBR only for FAT16 */ 
			mmc_init_once = 1;
			mbr_exist=1;
	    }
	    else if( (read_data[0x52] == 'F') && (read_data[0x53] == 'A') && (read_data[0x54] == 'T'))
	    {
			/* PBR only for FAT32 */ 
			mmc_init_once = 1;
		    mbr_exist=1;
	    }
	    else if((read_data[450] == 0x01) || (read_data[450] == 0x04) || (read_data[450] == 0x06) ||
	       (read_data[450] == 0x0B) ||   /* FAT32 Partition */
	       (read_data[450] == 0x0C) ||   /* FAT32 Partition */
	       (read_data[450] == 0x55) )    /* FAT32 Partition */
	    {
			mmc_init_once = 1;
		   	mbr_exist=1;
	    }
	    else goto skip_fwupdate;
	   
	}
   
    printf("MBR or Bootsector read done: %s\n", (mbr_exist)? "(with MBR)":"(without MBR)" );
#endif

	return 1;

skip_fwupdate:
	mmc_init_once = 0;
	mbr_exist = 0;
	
	
	return 1;
}





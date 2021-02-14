/*
 * Configuation settings for the MagicEyes POLLUX board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.		See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define DEBUG

/*-----------------------------------------------------------------------
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM926EJS		1	/* This is an arm926ejs CPU core	*/
#define CONFIG_POLLUX			1	/* in a MagicEyes POLLUX SoC  	 	*/
#define CONFIG_POLLUX_LF		1	/* in a MagicEyes POLLUX LF Board 	*/
#define CONFIG_GPH_N35          1

#define CONFIG_NAND_BOOT_MODE			1  		/* Pollux booting mode */
//#define CONFIG_LCD_NOINIT             1 

#define CONFIG_SST_39VF3201		        1
#define CONFIG_WIZ_LOGO                 1
//#define CONFIG_NO_CRC                   1   
#define CONFIG_POLLUX_SHADOW_ONE        
//#define CONFIG_SKIP_RELOCATE_UBOOT

/*-----------------------------------------------------------------------
 * input clock of PLL 
 */
#define CONFIG_SYS_CLK_FREQ		27000000	/* input clock			*/
#undef CONFIG_USE_IRQ			     		/* we don't need IRQ/FIQ stuff	*/

/*-----------------------------------------------------------------------
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN			(CFG_ENV_SIZE + 64*1024)/* unit is kbyte */
#define CFG_GBL_DATA_SIZE		128	     		/* size in bytes reserved for initial data */

/*-----------------------------------------------------------------------
 * Hardware drivers
 */
#define CONFIG_TIMER_ENABLE	   	1
#define CFG_HZ                  (261000000 / 50)

/*-----------------------------------------------------------------------
 * select serial console configuration
 */
#define CONFIG_DRIVER_POLLUX_SERIAL   	1

#define CONFIG_SERIAL1		   	1
#define CONFIG_CONS_INDEX	   	1
#define CONFIG_BAUDRATE		   	115200
#define CFG_BAUDRATE_TABLE	   	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_MMC 1
#define CONFIG_DOS_PARTITION 1
#define CONFIG_SUPPORT_VFAT 1

#define CONFIG_COMMANDS         	(CFG_CMD_BDI    | \
                                 	 CFG_CMD_NAND   | \
                                 	 CFG_CMD_ENV    | \
        	                         CFG_CMD_IMI    | \
	                                 CFG_CMD_LOADB  | \
	                                 CFG_CMD_MEMORY | \
                        	         CFG_CMD_FAT    | \
                                     CFG_CMD_MMC    | \
                        	         CFG_CMD_RUN)



#define CONFIG_BOOTP_MASK	   	CONFIG_BOOTP_DEFAULT

/*-----------------------------------------------------------------------
 * This must be included AFTER the definition of CONFIG_COMMANDS (if any)
 */
#include <cmd_confdefs.h>

#define CONFIG_BOOTDELAY	   	0
#define CONFIG_ETHADDR		   	11:00:33:22:55:44:66
#define CONFIG_NETMASK		   	255.255.255.0

//#define CONFIG_BOOTARGS4K   "mem=86M root=/dev/mtdblock1 rw rootfstype=yaffs2 console=ttySAC0,115200n81"
#define CONFIG_BOOTARGS4K   "mem=86M root=/dev/mtdblock1 rootfstype=cramfs console=ttySAC0,115200n81 init=/etc/rc.d/binscript"
//#define CONFIG_BOOTARGS4K   "mem=86M root=/dev/mtdblock1 rootfstype=cramfs console=ttySAC0,115200n81"
#define CONFIG_BOOTARGS		"mem=86M root=/dev/mtdblock1 rw rootfstype=yaffs2 console=ttySAC0,115200n81" 

#define CONFIG_RAMDISKARGS      "mem=86M root=/dev/ram0 rw initrd=0x800000,16M console=ttySAC0,115200n81"
#define CONFIG_LOADADDR			0x01000000
#define CONFIG_IPADDR			192.168.0.100
#define CONFIG_SERVERIP			192.168.0.250		
#define CONFIG_GATEWAYIP		192.168.0.1
#define CONFIG_BOOTFILE			"uImage"  	// File to load

//#define CONFIG_BOOTCOMMAND		"nandr 1 0x300000 0x2000000; bootm 0x02000000"
#define CONFIG_BOOTCOMMAND		"bootm 0x02000000"
#define CONFIG_RAMBOOTCOMMAND	"bootm 0x02000000"
#define CFG_PROMPT				"gpollux# "     /* Monitor Command Prompt   */

/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP				       				/* undef to save memory	   */
#define CFG_CBSIZE		   	256		      				/* Console I/O Buffer Size  */
#define CFG_PBSIZE		   	(CFG_CBSIZE+sizeof(CFG_PROMPT)+16)	/* Print Buffer Size */
#define CFG_MAXARGS			16		       					/* max number of command args   */
#define CFG_BARGSIZE			   CFG_CBSIZE	       			/* Boot Argument Buffer Size    */
	
#define CFG_MEMTEST_START		   0x00000000	       			/* memtest works on */
#define CFG_MEMTEST_END			   0x08000000	       			/* 128 MB in DRAM	   */

#undef CFG_CLKS_IN_HZ		     								/* everything, incl board info, in Hz */
#define CFG_LOAD_ADDR			   0x02000000	       			/* default load address, 32M offset */

/*-----------------------------------------------------------------------
 * Platform Board Specific 
 *
 */
#define	CONFIG_MISC_INIT_R
 
/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE		   		(128*1024)	  /* regular stack */

#ifdef  CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	  	 	(4*1024)	  /* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	   		(4*1024)	  /* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	   		1
#define PHYS_SDRAM_1		   		0x00000000	  /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	   		0x08000000	  /* 128 MB */

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_NO_FLASH
#define PHYS_FLASH_1		0x80000000 /* cs0, shadow=1 */
#define CFG_FLASH_BASE		PHYS_FLASH_1
//#define	CFG_ENV_IS_IN_FLASH	1
//#define CFG_ENV_SIZE		0x10000								/* Total Size of Environment Sector */
#define CFG_FW_BOOT         1
#define CFG_FW_KERNEL       2
#define CFG_FW_BSSECT       0
#define CFG_FW_BESECT       5
#define CFG_FW_KSSECT       6
#define CFG_FW_KESECT       63

#define CFG_FW_ADDR      0x1000000
#define CFG_KERNEL_LOAD_ADDR      0x2000000
#define CFG_FW_BDEST    0x80000000
//#define CFG_FW_BSIZE     0x3A000
#define CFG_FW_BSIZE     0x3F000
#define CFG_FW_KDEST    0x80060000
#define CFG_FW_KSIZE     0x3A0000  


#ifdef CONFIG_SST_39VF1601
	#define PHYS_FLASH_SIZE		0x00200000 					    /* 2MB */
	#define CFG_MAX_FLASH_SECT	(32)						    /* max number of sectors on one chip */
    #define CFG_ENV_ADDR		(CFG_FLASH_BASE + 0x0050000) 		/* addr of environment */
#endif

#ifdef CONFIG_SST_39VF3201
	#define PHYS_FLASH_SIZE		0x00400000 					/* 4MB */
	#define CFG_MAX_FLASH_SECT	(64)						/* max number of sectors on one chip */
    #define CFG_ENV_ADDR		(CFG_FLASH_BASE + 0x0050000) 		/* addr of environment */
#endif

#ifdef CONFIG_AMD_AM29LV320
	#define PHYS_FLASH_SIZE		0x00400000 					/* 4MB */
	#define CFG_MAX_FLASH_SECT	(71)						/* max number of sectors on one chip */
#endif

#ifdef CONFIG_EXCEL_CR29LV320E
	#define PHYS_FLASH_SIZE		0x00400000 					/* 4MB */
	#define CFG_MAX_FLASH_SECT	(71)						/* max number of sectors on one chip */
#endif

#define CFG_MAX_FLASH_BANKS	1	                            /* max number of memory banks */
#define CFG_FLASH_EMPTY_INFO		                        /* print 'E' for empty sector on flinfo */


/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(5*CFG_HZ) 				    /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(5*CFG_HZ) 				    /* Timeout for Flash Write */


/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_NAND_DEVICE			1
#define CFG_NAND_BASE				0xAC000000 /* shadow=1 */ 
#define CFG_NAND_BASE_LIST			{ CFG_NAND_BASE	}

#define CONFIG_MTD_DEBUG
#define CONFIG_MTD_DEBUG_VERBOSE	3
#define NAND_MAX_CHIPS				1
#define NAND_MAX_FLOORS				1
#define NAND_ChipID_UNKNOWN			0x00

#undef	CFG_ENV_IS_IN_FLASH	   0
#define CFG_ENV_IS_IN_NAND	   1

#undef  CFG_ENV_ADDR           
#define CFG_ENV_ADDR		   0x6000		/* addr of environment in NAND */
#define CFG_ENV_SIZE		   0x1000		/* Total Size of Environment Sector */
#define CFG_ENV_OFFSET		   0x6000		/* addr of environment in NAND */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* ghcstop: tag setting is transfered to kernel */
#define CONFIG_CMDLINE_TAG   		1
#define CONFIG_SETUP_MEMORY_TAGS  	1
#define CONFIG_INITRD_TAG  			1

#define CONFIG_B2  					1	
#define CONFIG_AUTO_COMPLETE       	1 


/* Board Late Init */
#define BOARD_LATE_INIT


#endif // __CONFIG_H


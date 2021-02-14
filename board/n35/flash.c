/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Modified 4/5/2001
 * Wait for completion of each sector erase command issued
 * 4/5/2001
 * Chris Hallinan - DS4.COM, Inc. - clh@net1plus.com
 */

#include <common.h>

flash_info_t	flash_info[CFG_MAX_FLASH_BANKS]; /* info for FLASH chips	*/

/*-----------------------------------------------------------------------
 * Functions
 */
static ulong flash_get_size (vu_long *addr, flash_info_t *info);
static void flash_get_offsets (ulong base, flash_info_t *info);
void inline spin_wheel (void);

#define ADDR0           0x5555
#define ADDR1           0x2aaa
#define FLASH_WORD_SIZE unsigned short

/*-----------------------------------------------------------------------
 */

unsigned long flash_init (void)
{
	unsigned long size_b, size_tot=0;
	int i;
	unsigned long	flash_base_addr[CFG_MAX_FLASH_BANKS]; 

	/* If you have more than 1 bank, add flash base address */
	flash_base_addr[0] = CFG_FLASH_BASE;
	
	/* Init: no FLASHes known */
	for (i=0; i < CFG_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
	/* Static FLASH Bank configuration here - FIXME XXX */
		size_b = flash_get_size((vu_long *)flash_base_addr[i], &flash_info[i]);
		if (flash_info[i].flash_id == FLASH_UNKNOWN) {
			printf ("## Unknown FLASH on Bank %d - Size = 0x%08lx = %ld MB\n",
				i, size_b, size_b<<20);
		}
		flash_info[i].size = size_b;
		size_tot += size_b;
		flash_get_offsets(flash_base_addr[i], &flash_info[i]);
	}

	flash_protect( FLAG_PROTECT_SET,
					CFG_FLASH_BASE,
					CFG_FLASH_BASE + monitor_flash_len - 1,
					&flash_info[0] );


	flash_protect( FLAG_PROTECT_SET,
					CFG_ENV_ADDR,
					CFG_ENV_ADDR + CFG_ENV_SIZE - 1,
					&flash_info[0] );


	return size_tot;
}



/*-----------------------------------------------------------------------
 */
static void flash_get_offsets (ulong base, flash_info_t *info)
{
	int i;

	/* set up sector start address table */
	if (((info->flash_id & FLASH_VENDMASK) == FLASH_MAN_SST) ||
		(info->flash_id  == FLASH_AM040)){
		for (i = 0; i < info->sector_count; i++)
			info->start[i] = base + (i * 0x00010000);
	} else {
		if (info->flash_id & FLASH_BTYPE) {
		/* set sector offsets for bottom boot block type	*/
			info->start[0] = base + 0x00000000;
			info->start[1] = base + 0x00004000;
			info->start[2] = base + 0x00006000;
			info->start[3] = base + 0x00008000;
			for (i = 4; i < info->sector_count; i++) {
				info->start[i] = base + (i * 0x00010000) - 0x00030000;
			}
		} else {
		/* set sector offsets for top boot block type		*/
			i = info->sector_count - 1;
			info->start[i--] = base + info->size - 0x00004000;
			info->start[i--] = base + info->size - 0x00006000;
			info->start[i--] = base + info->size - 0x00008000;
			for (; i >= 0; i--) {
				info->start[i] = base + i * 0x00010000;
			}
		}
	}
}

/*-----------------------------------------------------------------------
 */
void flash_print_info  (flash_info_t *info)
{
	int i;
        int k;
        int size;
        int erased;
        volatile unsigned long *flash;

	if (info->flash_id == FLASH_UNKNOWN) {
		printf ("missing or unknown FLASH type\n");
		return;
	}

	switch (info->flash_id & FLASH_VENDMASK) {
	case FLASH_MAN_AMD:	printf ("AMD ");		break;
	case FLASH_MAN_FUJ:	printf ("FUJITSU ");	break;
	case FLASH_MAN_SST:	printf ("SST ");		break;
	case EXCEL_MANUFACT : printf ("EXCEL/CORERIVER ");		break;
	default:		printf ("Unknown Vendor ");	break;
	}

	switch (info->flash_id & FLASH_TYPEMASK) {
	case FLASH_AM040:	printf ("AM29F040 (512 Kbit, uniform sector size)\n");
				break;
	case FLASH_AM400B:	printf ("AM29LV400B (4 Mbit, bottom boot sect)\n");
				break;
	case FLASH_AM400T:	printf ("AM29LV400T (4 Mbit, top boot sector)\n");
				break;
	case FLASH_AM800B:	printf ("AM29LV800B (8 Mbit, bottom boot sect)\n");
				break;
	case FLASH_AM800T:	printf ("AM29LV800T (8 Mbit, top boot sector)\n");
				break;
	case FLASH_AM160B:	printf ("AM29LV160B (16 Mbit, bottom boot sect)\n");
				break;
	case FLASH_AM160T:	printf ("AM29LV160T (16 Mbit, top boot sector)\n");
				break;
	case FLASH_AM320B:	printf ("AM29LV320B (32 Mbit, bottom boot sect)\n");
				break;
	case FLASH_AM320T:	printf ("AM29LV320T (32 Mbit, top boot sector)\n");
				break;
	case FLASH_SST800A:	printf ("SST39LF/VF800 (8 Mbit, uniform sector size)\n");
				break;
	case FLASH_SST160A:	printf ("SST39LF/VF160/VF1601 (16 Mbit, uniform sector size)\n");
				break;
	case FLASH_SST320:	printf ("SST39VF320/VF3201 (32 Mbit, uniform sector size)\n");
				break;
	case FLASH_LV320ET: printf ("CR29LV320ET (32 Mbit, top boot sector)\n");
				break;
	case FLASH_LV320EB: printf ("CR29LV320EB (32 Mbit, bottom boot sector)\n");
				break;
	default:	printf ("Unknown Chip Type\n");
				break;
	}

	printf ("  Size: %ld KB in %d Sectors\n",
		info->size >> 10, info->sector_count);

	printf ("  Sector Start Addresses:");
	for (i=0; i<info->sector_count; ++i) {
                /*
                 * Check if whole sector is erased
                 */
                if (i != (info->sector_count-1))
                  size = info->start[i+1] - info->start[i];
                else
                  size = info->start[0] + info->size - info->start[i];
                erased = 1;
                flash = (volatile unsigned long *)info->start[i];
                size = size >> 2;        /* divide by 4 for longword access */
                for (k=0; k<size; k++)
                  {
                    if (*flash++ != 0xffffffff)
                      {
                        erased = 0;
                        break;
                      }
                  }

		if ((i % 5) == 0)
			printf ("\n   ");
			printf (" %08lX%s%s",
			info->start[i],
			erased ? " E" : "  ",
			info->protect[i] ? "RO " : "   "
		);
	}
	printf ("\n");
	return;
}

/*-----------------------------------------------------------------------
 */


/*-----------------------------------------------------------------------
 */

/*
 * The following code cannot be run from FLASH!
 */
static ulong flash_get_size (vu_long *addr, flash_info_t *info)
{
	short i;
	FLASH_WORD_SIZE value;
	ulong base = (ulong)addr;
        volatile FLASH_WORD_SIZE *addr2 = (FLASH_WORD_SIZE *)addr;

	/* Write auto select command: read Manufacturer ID */
	addr2[ADDR0] = (FLASH_WORD_SIZE)0x00AA00AA;
	addr2[ADDR1] = (FLASH_WORD_SIZE)0x00550055;
	addr2[ADDR0] = (FLASH_WORD_SIZE)0x00900090;

	value = addr2[0];

	printf("manufacture ID = %ulx\n", value);
	switch (value) {
	case (FLASH_WORD_SIZE)AMD_MANUFACT:
		info->flash_id = FLASH_MAN_AMD;
		break;
	case (FLASH_WORD_SIZE)FUJ_MANUFACT:
		info->flash_id = FLASH_MAN_FUJ;
		break;
	case (FLASH_WORD_SIZE)SST_MANUFACT:
		info->flash_id = FLASH_MAN_SST;
		break;
	default:
		info->flash_id = FLASH_UNKNOWN;
		info->sector_count = 0;
		info->size = 0;
		return (0);			/* no or unknown flash	*/
	}

	value = addr2[1];			/* device ID		*/

	printf("device ID = %ulx\n", value);
	switch (value) {
	case (FLASH_WORD_SIZE)AMD_ID_F040B:
	        info->flash_id += FLASH_AM040;
		info->sector_count = 8;
		info->size = 0x0080000; /* => 512 ko */
		break;
	case (FLASH_WORD_SIZE)AMD_ID_LV400T:
		info->flash_id += FLASH_AM400T;
		info->sector_count = 11;
		info->size = 0x00080000;
		break;				/* => 0.5 MB		*/

	case (FLASH_WORD_SIZE)AMD_ID_LV400B:
		info->flash_id += FLASH_AM400B;
		info->sector_count = 11;
		info->size = 0x00080000;
		break;				/* => 0.5 MB		*/

	case (FLASH_WORD_SIZE)AMD_ID_LV800T:
		info->flash_id += FLASH_AM800T;
		info->sector_count = 19;
		info->size = 0x00100000;
		break;				/* => 1 MB		*/

	case (FLASH_WORD_SIZE)AMD_ID_LV800B:
		info->flash_id += FLASH_AM800B;
		info->sector_count = 19;
		info->size = 0x00100000;
		break;				/* => 1 MB		*/

	case (FLASH_WORD_SIZE)AMD_ID_LV160T:
		info->flash_id += FLASH_AM160T;
		info->sector_count = 35;
		info->size = 0x00200000;
		break;				/* => 2 MB		*/

	case (FLASH_WORD_SIZE)AMD_ID_LV160B:
		info->flash_id += FLASH_AM160B;
		info->sector_count = 35;
		info->size = 0x00200000;
		break;				/* => 2 MB		*/
	case (FLASH_WORD_SIZE)AMD_ID_LV320T:
		info->flash_id += FLASH_AM320T;
		info->sector_count = 71;
		info->size = 0x00400000;
		break;				/* => 4 MB		*/
	case (FLASH_WORD_SIZE)AMD_ID_LV320B:
		info->flash_id += FLASH_AM320B;
		info->sector_count = 71;
		info->size = 0x00400000;
		break;				/* => 4 MB		*/
	case (FLASH_WORD_SIZE)SST_ID_xF800A:
		info->flash_id += FLASH_SST800A;
		info->sector_count = 16;
		info->size = 0x00100000;
		break;				/* => 1 MB		*/

	case (FLASH_WORD_SIZE)SST_ID_xF160A:
		info->flash_id += FLASH_SST160A;
		info->sector_count = 32;
		info->size = 0x00200000;
		break;				/* => 2 MB		*/
	
	case (FLASH_WORD_SIZE)SST_ID_xF1601:
		info->flash_id += FLASH_SST160A;
		info->sector_count = 32;
		info->size = 0x00200000;
		break;				/* => 2 MB		*/
	
	case (FLASH_WORD_SIZE)SST_ID_xF3201:
		info->flash_id += FLASH_SST320;
		info->sector_count = 64;
		info->size = 0x00400000;
		break;				/* => 4 MB		*/
	
	case (FLASH_WORD_SIZE)EXCEL_ID_LV320ET:
		info->flash_id += FLASH_LV320ET;
		info->sector_count = 71;
		info->size = 0x00400000;
		break;				/* => 4 MB	*/
	
	case (FLASH_WORD_SIZE)EXCEL_ID_LV320EB:
		info->flash_id += FLASH_LV320EB;
		info->sector_count = 71;
		info->size = 0x00400000;
		break;				/* => 4 MB	*/	
	
	default:
		info->flash_id = FLASH_UNKNOWN;
		return (0);			/* => no or unknown flash */

	}

	/* set up sector start address table */
        if (((info->flash_id & FLASH_VENDMASK) == FLASH_MAN_SST) ||
	    (info->flash_id  == FLASH_AM040)){
	    for (i = 0; i < info->sector_count; i++)
		info->start[i] = base + (i * 0x00010000);
        } else {
	    if (info->flash_id & FLASH_BTYPE) {
		/* set sector offsets for bottom boot block type	*/
		info->start[0] = base + 0x00000000;
		info->start[1] = base + 0x00004000;
		info->start[2] = base + 0x00006000;
		info->start[3] = base + 0x00008000;
		for (i = 4; i < info->sector_count; i++) {
			info->start[i] = base + (i * 0x00010000) - 0x00030000;
		}
	    } else {
		/* set sector offsets for top boot block type		*/
		i = info->sector_count - 1;
		info->start[i--] = base + info->size - 0x00004000;
		info->start[i--] = base + info->size - 0x00006000;
		info->start[i--] = base + info->size - 0x00008000;
		for (; i >= 0; i--) {
			info->start[i] = base + i * 0x00010000;
		}
	    }
	}

	/* check for protected sectors */
	for (i = 0; i < info->sector_count; i++) {
		/* read sector protection at sector address, (A7 .. A0) = 0x02 */
		/* D0 = 1 if protected */
		addr2 = (volatile FLASH_WORD_SIZE *)(info->start[i]);
                if ((info->flash_id & FLASH_VENDMASK) == FLASH_MAN_SST)
                  info->protect[i] = 0;
                else
                  info->protect[i] = addr2[2] & 1;
	}

	/*
	 * Prevent writes to uninitialized FLASH.
	 */
	if (info->flash_id != FLASH_UNKNOWN) {
		addr2 = (FLASH_WORD_SIZE *)info->start[0];
		*addr2 = (FLASH_WORD_SIZE)0x00F000F0;	/* reset bank */
	}

	return (info->size);
}

int wait_for_DQ7(flash_info_t *info, int sect)
{
	ulong start, now, last;
	volatile FLASH_WORD_SIZE *addr = (FLASH_WORD_SIZE *)(info->start[sect]);

	start = get_timer (0);
    last  = start;
    while ((addr[0] & (FLASH_WORD_SIZE)0x00800080) != (FLASH_WORD_SIZE)0x00800080) {
        if ((now = get_timer(start)) > CFG_FLASH_ERASE_TOUT) {
            printf ("Timeout\n");
            return -1;
        }
        /* show that we're waiting */
        if ((now - last) > 1000) {  /* every second */
//            putc ('.');
            last = now;
        }
    }
	return 0;
}

/*-----------------------------------------------------------------------
 */

int	flash_erase (flash_info_t *info, int s_first, int s_last)
{
	volatile FLASH_WORD_SIZE *addr = (FLASH_WORD_SIZE *)(info->start[0]);
	volatile FLASH_WORD_SIZE *addr2;
	int flag, cflag, prot, sect, l_sect;
	int i;

	if ((s_first < 0) || (s_first > s_last)) {
		if (info->flash_id == FLASH_UNKNOWN) {
			printf ("- missing\n");
		} else {
			printf ("- no sectors to erase\n");
		}
		return 1;
	}

	if (info->flash_id == FLASH_UNKNOWN) {
		printf ("Can't erase unknown flash type - aborted\n");
		return 1;
	}

	prot = 0;
	for (sect=s_first; sect<=s_last; ++sect) {
		if (info->protect[sect]) {
			prot++;
		}
	}

	if (prot) {
		printf ("- Warning: %d protected sectors will not be erased!\n",
			prot);
	} else {
		printf ("\n");
	}

	l_sect = -1;

	/* Disable interrupts which might cause a timeout here */
	cflag = icache_status();
	icache_disable();
	flag = disable_interrupts();

	/* Start erase on unprotected sectors */
	for (sect = s_first; sect<=s_last; sect++) {
		if (info->protect[sect] == 0) {	/* not protected */
		    addr2 = (FLASH_WORD_SIZE *)(info->start[sect]);
			printf("Erasing sector %p\n", addr2);	/* CLH */

		    if ((info->flash_id & FLASH_VENDMASK) == FLASH_MAN_SST) {
				addr[ADDR0] = (FLASH_WORD_SIZE)0x00AA00AA;
				addr[ADDR1] = (FLASH_WORD_SIZE)0x00550055;
				addr[ADDR0] = (FLASH_WORD_SIZE)0x00800080;
				addr[ADDR0] = (FLASH_WORD_SIZE)0x00AA00AA;
				addr[ADDR1] = (FLASH_WORD_SIZE)0x00550055;
				addr2[0] 	= (FLASH_WORD_SIZE)0x00500050;  /* block erase */
				for (i=0; i<50; i++)
					udelay(1000);  /* wait 1 ms */
		    } else {
				addr[ADDR0] = (FLASH_WORD_SIZE)0x00AA00AA;
				addr[ADDR1] = (FLASH_WORD_SIZE)0x00550055;
				addr[ADDR0] = (FLASH_WORD_SIZE)0x00800080;
				addr[ADDR0] = (FLASH_WORD_SIZE)0x00AA00AA;
				addr[ADDR1] = (FLASH_WORD_SIZE)0x00550055;
				addr2[0] = (FLASH_WORD_SIZE)0x00300030;  /* sector erase */
		    }
		    l_sect = sect;
		    /*
		     * Wait for each sector to complete, it's more
		     * reliable.  According to AMD Spec, you must
		     * issue all erase commands within a specified
		     * timeout.  This has been seen to fail, especially
		     * if printf()s are included (for debug)!!
		     */
		    wait_for_DQ7(info, sect);
		}
	}

	/* re-enable interrupts if necessary */
	if (flag)
		enable_interrupts();
	if (cflag)
		icache_enable();

	/* wait at least 80us - let's wait 1 ms */
	udelay (1000);

	/* reset to read mode */
	addr = (FLASH_WORD_SIZE *)info->start[0];
	addr[0] = (FLASH_WORD_SIZE)0x00F000F0;	/* reset bank */

	printf (" done\n");
	return 0;
}

/*-----------------------------------------------------------------------
 * Write half word to flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 */
volatile static int write_hword (flash_info_t *info, ulong dest, ushort data)
{
	volatile FLASH_WORD_SIZE *addr2 = (FLASH_WORD_SIZE *) (info->start[0]);
	volatile FLASH_WORD_SIZE *dest2 = (FLASH_WORD_SIZE *) dest;
	volatile FLASH_WORD_SIZE *data2 = (FLASH_WORD_SIZE *) & data;
	ulong start;
	int i;

	/* Check if Flash is (sufficiently) erased */
	if ((*((volatile FLASH_WORD_SIZE *) dest) &
	    (FLASH_WORD_SIZE) data) != (FLASH_WORD_SIZE) data) {
		return (2);
	}

	for (i = 0; i < 2 / sizeof (FLASH_WORD_SIZE); i++) {
		int flag;

		/* Disable interrupts which might cause a timeout here */
		flag = disable_interrupts ();

		addr2[ADDR0] = (FLASH_WORD_SIZE) 0x00AA00AA;
		addr2[ADDR1] = (FLASH_WORD_SIZE) 0x00550055;
		addr2[ADDR0] = (FLASH_WORD_SIZE) 0x00A000A0;

		dest2[i] = data2[i];

		/* re-enable interrupts if necessary */
		if (flag)
			enable_interrupts ();

		/* data polling for D7 */
		start = get_timer (0);
		while ((dest2[i] & (FLASH_WORD_SIZE) 0x00800080) !=
		       (data2[i] & (FLASH_WORD_SIZE) 0x00800080)) {

			if (get_timer (start) > CFG_FLASH_WRITE_TOUT) {
				return (1);
			}
		}
	}

	return (0);
}

/*-----------------------------------------------------------------------
 * Copy memory to flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 */
int write_buff (flash_info_t *info, uchar *src, ulong addr, ulong cnt)
{
    ulong cp, wp;
    int l;
    int i, rc, count;
    ushort data;

    wp = (addr & ~1);	/* get lower word aligned address */

    /*
     * handle unaligned start bytes
     */
    if ((l = addr - wp) != 0) {
	data = 0;
	for (i=0, cp=wp; i<l; ++i, ++cp) {
	    data = (data >> 8) | (*(uchar *)cp << 8);
	}
	for (; i<2 && cnt>0; ++i) {
	    data = (data >> 8) | (*src++ << 8);
	    --cnt;
	    ++cp;
	}
	for (; cnt==0 && i<2; ++i, ++cp) {
	    data = (data >> 8) | (*(uchar *)cp << 8);
	}

	if ((rc = write_hword(info, wp, data)) != 0) {
	    return (rc);
	}
	wp += 2;
    }

    /*
     * handle word aligned part
     */
    count = 0;
    while (cnt >= 2) {
	data = *((vu_short*)src);
	if ((rc = write_hword(info, wp, data)) != 0) {
	    return (rc);
	}
	if (count++ > 0x800) {
	    spin_wheel ();
	    count = 0;
	}
	src += 2;
	wp  += 2;
	cnt -= 2;
    }

    if (cnt == 0) {
	return ERR_OK;
    }

    /*
     * handle unaligned tail bytes
     */
    data = 0;
    for (i=0, cp=wp; i<2 && cnt>0; ++i, ++cp) {
	data = (data >> 8) | (*src++ << 8);
	--cnt;
    }
    for (; i<2; ++i, ++cp) {
	data = (data >> 8) | (*(uchar *)cp << 8);
    }

    return write_hword(info, wp, data);
}

void inline spin_wheel (void)
{
	static int p = 0;
	static char w[] = "\\/-";

	printf ("\010%c", w[p]);
	(++p == 3) ? (p = 0) : 0;
}
/*-----------------------------------------------------------------------
 */

/*
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
 *
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 * Wolfgang Grandegger, DENX Software Engineering, wg@denx.de.
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
 * nand flash boot code programming routine
 *
 * godori of aesop
 *
 */

#include <common.h>

#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>

int do_brp(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
        int rc = 1;
        char buffer[100];

        if (argc  > 2)
        {
                printf ("Usage:\n%s\n", cmdtp->usage);
                return 1;
        }
        
        if (argc < 2)
        {
            printf("tftp 01000000 polluxb\n");
            sprintf (buffer,"tftp 01000000 polluxb ");
            rc = run_command(buffer, flag);

            if ( rc >= 0 )
            {
                printf("correct u-boot image!!!\n");
                run_command("protect off 1:0-12", flag);
                #if 0
                run_command("erase 1:0-0", flag);
                run_command("erase 1:1-1", flag);
                run_command("erase 1:2-2", flag);
                run_command("erase 1:3-3", flag);
                run_command("erase 1:4-4", flag);
                run_command("erase 1:5-5", flag);
                run_command("erase 1:6-6", flag);
                #else
                run_command("erase 1:0-12", flag);
                #endif
                sprintf (buffer,"cp.b 01000000 00000000 %s ",getenv("filesize"));
                run_command(buffer, flag);
                /*run_command("protect on 1:0-126 ",bd, flag);*/
                /* ghcstop fix */
                run_command("protect on 1:0-12", flag);

                printf("\nNow! - Power switch turn off & on~~~\n");
            }
        }

        if (argc == 2)
        {
            printf("tftp 01000000 %s\n",argv[1]);	
            sprintf (buffer,"tftp 01000000 %s ",argv[1]);
            rc = run_command(buffer, flag);

            if ( rc >= 0 )
            {
                printf("correct u-boot image!!!\n");
                run_command("protect off 1:0-12", flag);
                #if 0
                run_command("erase 1:0-0", flag);
                run_command("erase 1:1-1", flag);
                run_command("erase 1:2-2", flag);
                run_command("erase 1:3-3", flag);
                run_command("erase 1:4-4", flag);
                run_command("erase 1:5-5", flag);
                run_command("erase 1:6-6", flag);
                #else
                run_command("erase 1:0-12", flag);
                #endif
                sprintf (buffer,"cp.b 01000000 00000000 %s ",getenv("filesize"));
                run_command(buffer, flag);
                /*run_command("protect on 1:0-126 ",bd, flag);*/
                /* ghcstop fix */
                run_command("protect on 1:0-12", flag);
                printf("\nNow! - Power switch turn off & on~~~\n");
            }
        }

        return 0;
}
/***************************************************/


U_BOOT_CMD(
	brp,	2,	0,	do_brp,
	"bootrom programming ex> brp - get default 'aesop'image to NOR flash 0x00000000\n",
        "bootrom programming ex> brp [filename]"
);



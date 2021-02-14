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
#include <common.h>
#include <part.h>
#include "sd.h"


#if 0
	#define gprintf(fmt, x... ) printf( "%s: " fmt, __FUNCTION__ , ## x)
#else
	#define gprintf(x...) do { } while (0)
#endif


/*
 * u-boot MMC register stuff
 */
extern int fat_register_device(block_dev_desc_t *dev_desc, int part_no);
static block_dev_desc_t mmc_dev;
ulong mmc_bread(int dev_num, ulong blknr, ulong blkcnt, ulong *dst);



struct pollux_sdi_host ghost;

#define mdelay(n)           udelay((n)*1000) 
#define COMMAND_WAIT_TOUT	(3*CFG_HZ)

#define RES_LEN_SHORT		6
#define RES_LEN_LONG		17


static unsigned char resp_buf[RES_LEN_LONG];


typedef struct {
	unsigned char  mid;			/* [127:120] Manufacturer ID */		
	unsigned short oid;			/* [119:104] OEM/Application ID */
	unsigned char  pnm[7];  	/* [103: 56] Product Name + '\0' (MMCSD) */
	unsigned char  pnm_sd[6];	/* [103: 64] Product Name + '\0' (SD) */
	unsigned char  prv;			/* [ 55: 48] Product Version (MMCSD) */
								/* [ 63: 56] Product Version (SD) */
	unsigned long psn;			/* [ 47: 16] Product Serial Number (MMCSD) */
								/* [ 55: 24] Product Serial Number (SD) */
	unsigned char  mdt;			/* [ 15:  8] Manufacturing date (MMCSD) */
	unsigned short mdt_sd;   	/* [ 19:  8] Manufacturing date (SD) */
								/* [  7:  1] CRC (rwe) */
} CID_regs;

typedef struct {
	unsigned char csd;			/* [127:126] CSD structure */
	unsigned char spec_vers;	/* [125:122] Spec version (MMCSD) */
	struct {
		unsigned char man;		/* [118:115] time mantissa */
		unsigned char exp;		/* [114:113] time exponent */
	} taac;						/* [119:112] Data read access-time-1 */
	unsigned char nsac;			/* [111:104] Data read access-time-2 in CLK cycle */
	struct {
		unsigned char man;		/* [103:100] rate mantissa */
		unsigned char exp;		/* [ 99: 97] rate exponent */
	} tran_speed;				/* [103: 96] Max. data transfer rate */
	unsigned short ccc;			/* [ 95: 84]  Card command classes */
	unsigned char read_len;		/* [ 83: 80] Max. read data block length */
	unsigned char read_part;	/* [ 79: 79] Partial blocks for read allowed */
	unsigned char write_mis;	/* [ 78: 78] write block misalignment */
	unsigned char read_mis;		/* [ 77: 77] read block misalignment */
	unsigned char dsr;			/* [ 76: 76] DSR implemented */
	unsigned long c_size;		/* [ 73: 62] Device size (SDHC: [ 69: 48] )*/

	unsigned char vcc_r_min;	/* [ 61: 59] Max. read current at Vcc min */
	unsigned char vcc_r_max;	/* [ 58: 56] Max. read current at Vcc max */
	unsigned char vcc_w_min;	/* [ 55: 53] Max. write current at Vcc min */
	unsigned char vcc_w_max;	/* [ 52: 50] Max. write current at Vcc max */
	unsigned char c_size_mult;	/* [ 49: 47] Device size multiplier */
	unsigned char er_blk_en;	/* [ 46: 46] Erase single block enable (SD) */
	unsigned char er_size;		/* [ 46: 42] Erase sector size (MMCSD) */
								/* [ 45: 39] Erase sector size (SD) */
	unsigned char er_grp_size;	/* [ 41: 37] Erase group size (MMCSD) */
	unsigned char wp_grp_size;	/* [ 36: 32] Write protect group size (MMCSD)*/
								/* [ 38: 32] Write Protect group size (SD) */	
	unsigned char wp_grp_en;	/* [ 31: 31] Write protect group enable */
	unsigned char dflt_ecc;		/* [ 30: 29] Manufacturer default ECC (MMCSD) */
	unsigned char r2w_factor;	/* [ 28: 26] Write speed factor */
	unsigned char write_len;	/* [ 25: 22] Max. write data block length */
	unsigned char write_part;	/* [ 21: 21] Partial blocks for write allowed */
								/* [ 20: 17] Reserved */
								/* [ 16: 16] Content protection application */
	unsigned char ffmt_grp;		/* [ 15: 15] File format group (rw) */
	unsigned char copy;			/* [ 14: 14] Copy flag (OTP) (rw) */
	unsigned char perm_wp;		/* [ 13: 13] Permanent write protection (rw) */
	unsigned char tmp_wp;		/* [ 12: 12] temporary write protection (rwe) */
	unsigned char ffmt;			/* [ 11: 10] file format (rw) */
	unsigned char ecc;			/* [  9:  8] ECC (MMCSD) (rwe) */
								/* [  7:  1] CRC (rwe) */
} CSD_regs;


#define INICLK	400000
#define SDCLK	25000000	
#define MMCCLK	15000000	
 
int mmc_init_once = 0;
 
 
#define MMCSD_PCLK			50000000	// 50 MHz
#define MMCSD_MMC_NORCLK	8000000		//  8 MHz
#define MMCSD_SD_NORCLK		2000000		//  2 MHz
#define MMCSD_INICLK		400000		// 400 KHz

 
int isMMC;	// 1:MMC, 0:SD
int RCA=0;	// Relative card address
unsigned int CardSize=0; // SD capacity in byte	
int sdhc;

int card_bus_width = 0; // 0: 1bit, 1: 4bit



static void dump_sdi(struct pollux_sdi_host *host)
{
	struct MES_SDHC_RegisterSet *p = host->sdi;

	gprintf("CTRL    =  0x%08x\n", p->CTRL);       				// 0x000 : Control
	gprintf("CLKDIV  = 	0x%08x\n", p->CLKDIV);  				// 0x008 : Clock Divider 
	gprintf("CLKENA  = 	0x%08x\n", p->CLKENA);  				// 0x010 : Clock Enable
	gprintf("TMOUT   = 	0x%08x\n", p->TMOUT);					// 0x014 : Time-Out
	gprintf("CTYPE   = 	0x%08x\n", p->CTYPE);					// 0x018 : Card Type
	gprintf("BLKSIZ  = 	0x%08x\n", p->BLKSIZ);  				// 0x01C : Block Size
	gprintf("BYTCNT  = 	0x%08x\n", p->BYTCNT);  				// 0x020 : Byte Count
	gprintf("INTMASK =	0x%08x\n", p->INTMASK);   				// 0x024 : Interrupt Mask
	gprintf("CMDARG  = 	0x%08x\n", p->CMDARG);  				// 0x028 : Command Argument
	gprintf("CMD     = 	0x%08x\n", p->CMD);	    				// 0x02C : Command
	//gprintf("RESP0   = 	0x%08x\n", p->RESP0);					// 0x030 : Response 0
	//gprintf("RESP1   = 	0x%08x\n", p->RESP1);					// 0x034 : Response 1
	//gprintf("RESP2   = 	0x%08x\n", p->RESP2);					// 0x038 : Response 2
	//gprintf("RESP3   = 	0x%08x\n", p->RESP3);					// 0x03C : Response 3
	gprintf("MINTSTS = 	0x%08x\n", p->MINTSTS);   				// 0x040 : Masked Interrupt
	gprintf("RINTSTS =	0x%08x\n", p->RINTSTS);  				// 0x044 : Raw Interrupt
	gprintf("STATUS  = 	0x%08x\n", p->STATUS);  				// 0x048 : Status
	gprintf("FIFOTH  = 	0x%08x\n", p->FIFOTH);  				// 0x04C : FIFO Threshold Watermark
	gprintf("TCBCNT  = 	0x%08x\n", p->TCBCNT);  				// 0x05C : Transferred Card Byte Count
	gprintf("TBBCNT  = 	0x%08x\n", p->TBBCNT);  				// 0x060 : Transferred Host Byte Count
	//gprintf("DATA    = 	0x%08x\n", p->DATA);					// 0x100 : Data
	gprintf("CLKENB  = 	0x%08x\n", p->CLKENB);  				// 0x7C0 : Clock Enable for CLKGEN module
	gprintf("CLKGEN  = 	0x%08x\n", p->CLKGEN);  				// 0x7C4 : Clock Generator for CLKGEN module
}

	 
 

void str2cid( CID_regs *regs, unsigned char *buff)
{
	int i;
	regs->mid = buff[0];
	regs->oid = (buff[1] << 8) | (buff[2]);
	
	if(!isMMC) {
		for(i=0; i < 5; i++)
			regs->pnm_sd[i] = buff[3+i];
		regs->pnm_sd[5] = '\0';
		regs->prv = buff[8];
		regs->psn = (buff[9] << 24) | (buff[10] << 16) |(buff[11] << 8) | buff[12];
		regs->mdt_sd = (buff[13] << 8) | buff[14];
	} /* SD Only*/
	else {
		for(i=0; i < 6; i++)
			regs->pnm[i] = buff[3+i];
		regs->pnm[6] = '\0';
		regs->prv = buff[9];
		regs->psn = (buff[10] << 24) | (buff[11] << 16) |(buff[12] << 8) | buff[13];
		regs->mdt = buff[14];
	} /* MMC Only*/		
}
 
int str2csd( CSD_regs *regs, unsigned char  *buff, int verSD)
{
	int ret;
	regs->csd		= (buff[0] & 0xc0) >> 6;
	
	gprintf("regs->csd = %d, sdver = %d\n", regs->csd, verSD);
	
	if (regs->csd > 2)
		return -1;

	if(verSD)
	{
		gprintf("sdcheck.........");
		if(regs->csd != 1) 
			goto NOT_SDHC;
		/* CSD version 2.0; consists mostly of fixed values,
		 * which host must override and not bother parsing out.
		 * FIXME: we should parse CSD correctly for HC MMC cards */
gprintf("csd ==> SDHC\n");		 
		(regs->taac).man	= 1;
		(regs->taac).exp	= 6;
		 regs->nsac			= 0;
		(regs->tran_speed).man  = (buff[3] & 0x78) >> 3;
		(regs->tran_speed).exp  = (buff[3]) & 0x07;
		regs->ccc			= (buff[4] << 4) | ((buff[5] & 0xf0) >> 4);
		regs->read_len		= 9;
		regs->read_part		= 0;
		regs->write_mis		= 0;
		regs->read_mis		= 0;
		regs->dsr			= (buff[6] & 0x10) ? 1 : 0;
		regs->c_size		= ((buff[7] & 0x3f) << 16) | (buff[8] << 8) | buff[9];
		regs->vcc_r_min		= 7;
		regs->vcc_r_max		= 6;
		regs->vcc_w_min		= 7;
		regs->vcc_w_max		= 6;
		regs->c_size_mult 	= 10 - 2;
		regs->er_blk_en		= 1;
		regs->er_size		= 0x7f;
		regs->wp_grp_size	= 0;
		regs->wp_grp_en		= 0;
		regs->r2w_factor 	= 2;
		regs->write_len  	= 9;
		regs->write_part 	= 0;
		regs->ffmt_grp   	= 0;
		regs->copy			= (buff[14] & 0x40) ? 1 : 0;
		regs->perm_wp		= (buff[14] & 0x20) ? 1 : 0;
		regs->tmp_wp		= (buff[14] & 0x10) ? 1 : 0;
		regs->ffmt			= 0;
		
		ret					= 1;
	}
	else
	{
NOT_SDHC:
gprintf("csd ==> Not SDHC\n");		 
		if(isMMC) /* MMC */
			regs->spec_vers	= (buff[0] & 0x3c) >> 2;
		(regs->taac).man	= (buff[1] & 0x78) >> 3;
		(regs->taac).exp	= (buff[1]) & 0x07;
		regs->nsac			= buff[2];
		(regs->tran_speed).man  = (buff[3] & 0x78) >> 3;
		(regs->tran_speed).exp  = (buff[3]) & 0x07;
		regs->ccc			= (buff[4] << 4) | ((buff[5] & 0xf0) >> 4);
		regs->read_len		= (buff[5] & 0x0f);
		regs->read_part		= (buff[6] & 0x80) ? 1 : 0;
		regs->write_mis		= (buff[6] & 0x40) ? 1 : 0;
		regs->read_mis		= (buff[6] & 0x20) ? 1 : 0;
		regs->dsr			= (buff[6] & 0x10) ? 1 : 0;
		regs->c_size		= ((buff[6] & 0x03) << 10) | (buff[7] << 2) | ((buff[8] & 0xc0) >> 6);
		regs->vcc_r_min		= (buff[8] & 0x38) >> 3;
		regs->vcc_r_max		= (buff[8] & 0x07);
		regs->vcc_w_min		= (buff[9] & 0xe0) >> 5;
		regs->vcc_w_max		= (buff[9] & 0x1c) >> 2;
		regs->c_size_mult 	= ((buff[9] & 0x03) << 1) | ((buff[10] & 0x80) >> 7);
		if(!isMMC) {
			regs->er_blk_en	= (buff[10] & 0x40) ? 1: 0;
			regs->er_size	= ((buff[10] & 0x3f) | (buff[11] & 0x80)) >> 7;
			regs->wp_grp_size= (buff[11] & 0x7f);
		} /* SD */
		else {
			regs->er_size   = (buff[10] & 0x7c) >> 2;
			regs->er_grp_size= ((buff[10] & 0x03) << 3) |((buff[11] & 0xe0) >> 5);
			regs->wp_grp_size= (buff[11] & 0x1f);
		} /* MMC */
		regs->wp_grp_en		= (buff[12] & 0x80) ? 1 : 0;
		if(isMMC)	/* MMC */
			regs->dflt_ecc	= (buff[12] & 0x60) >> 5;
		regs->r2w_factor 	= (buff[12] & 0x1c) >> 2;
		regs->write_len  	= ((buff[12] & 0x03) << 2) | ((buff[13] & 0xc0) >> 6);
		regs->write_part 	= (buff[13] & 0x20) ? 1 : 0;
		regs->ffmt_grp   	= (buff[14] & 0x80) ? 1 : 0;
		regs->copy			= (buff[14] & 0x40) ? 1 : 0;
		regs->perm_wp		= (buff[14] & 0x20) ? 1 : 0;
		regs->tmp_wp		= (buff[14] & 0x10) ? 1 : 0;
		regs->ffmt			= (buff[14] & 0x0c) >> 2;
		if(isMMC)	/* MMC */
			regs->ecc		= (buff[14] & 0x03);
		
		ret=0;
	}

	/* calculate total card size in bytes */
	CardSize = (1 + regs->c_size) * (0x01 << (regs->c_size_mult + 2)) * (0x01 << regs->read_len);
	printf("Size = %d KBytes  \n", CardSize/1024);

	
	return ret;		/* unknown CSD version */
}


static int sdi_command(struct pollux_sdi_host *host)
{
	struct MES_SDHC_RegisterSet *p = host->sdi;	
	u32 start, now;
	int ret = 0;
	u32 flag = 0;
	int command = host->opcode;
	
	
//gprintf("......command = %d\n", command);	
	gprintf("command = 0x%x: %d\n", command, command);
    switch( command )
    {
    case GO_IDLE_STATE:        // No Response(이 녀석은 원래 mmc/sd 명령에 없다) 
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_SENDINIT |
            MES_SDHC_CMDFLAG_WAITPRVDAT;
		//gprintf("go........idle......state\n");            
        break;
    case IO_SEND_OP_COND:      // R4
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_SENDINIT |
            MES_SDHC_CMDFLAG_WAITPRVDAT | MES_SDHC_CMDFLAG_SHORTRSP;
        break;
    case ALL_SEND_CID:         // R2
    case SEND_CSD:             // R2
    case SEND_CID:             // R2
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_LONGRSP;
        break;
    case SEND_STATUS:          // R1
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_CHKRSPCRC |
            MES_SDHC_CMDFLAG_SHORTRSP;
        break;
    case SET_BLOCKLEN:         // R1
    case APP_CMD:              // R1
    case SET_CLR_CARD_DETECT:  // R1
    case SWITCH_FUNC:          // R1
    case SELECT_CARD:          // R1b
    case SEND_RELATIVE_ADDR:   // R6
    case SEND_IF_COND:         // R7
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP;
        break;
    // ghcstop: 이 두개는 mmc쪽 명령과 동일하다. 처리도 동일하기 때문에 그냥 없앤다.
    //case SET_BUS_WIDTH:        // R1
    //case SD_STATUS:            // R1
    case SEND_SCR:             // R1, ghcstop fix for debugging
    	command = ((APP_CMD<<8) | 51);
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP;
        gprintf("send......scr\n");
        break;
    case READ_SINGLE_BLOCK:    // R1
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP |
            MES_SDHC_CMDFLAG_BLOCK | MES_SDHC_CMDFLAG_RXDATA;
        break;
    case READ_MULTIPLE_BLOCK:  // R1
#if 0    
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP |
            MES_SDHC_CMDFLAG_BLOCK | MES_SDHC_CMDFLAG_RXDATA |
            MES_SDHC_CMDFLAG_SENDAUTOSTOP;
#else
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP |
            MES_SDHC_CMDFLAG_BLOCK | MES_SDHC_CMDFLAG_RXDATA;
#endif            
        gprintf("=================================== multiple_block, arg = %d\n", host->arg );
        break;
    case WRITE_BLOCK:          // R1
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP |
            MES_SDHC_CMDFLAG_BLOCK | MES_SDHC_CMDFLAG_TXDATA;
        break;
    case WRITE_MULTIPLE_BLOCK: // R1
#if 0    
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP |
            MES_SDHC_CMDFLAG_BLOCK | MES_SDHC_CMDFLAG_TXDATA |
            MES_SDHC_CMDFLAG_SENDAUTOSTOP;
#else
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_CHKRSPCRC | MES_SDHC_CMDFLAG_SHORTRSP |
            MES_SDHC_CMDFLAG_BLOCK | MES_SDHC_CMDFLAG_TXDATA;
#endif            
        break;
    case STOP_TRANSMISSION:    // R1
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_CHKRSPCRC |
            MES_SDHC_CMDFLAG_SHORTRSP | MES_SDHC_CMDFLAG_STOPABORT;
        break;
    case SD_SEND_OP_COND:      // R3
        flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_SHORTRSP;
        break;
    default:
		flag =
            MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_WAITPRVDAT |
            MES_SDHC_CMDFLAG_SHORTRSP;
        break;
    }

	MES_SDHC_ClearInterruptPendingAll(host->channel); // 없애도 될지 모르지만 코드에 있으니깐 일단은 동작시켜 본다.

    // Send Command
    start = get_timer(0);
    do
    {
        MES_SDHC_ClearInterruptPending(host->channel, MES_SDHC_INT_HLE);
        MES_SDHC_SetCommandArgument(host->channel, host->arg);
        MES_SDHC_SetCommand(host->channel, command & 0xFF, flag);
        while (MES_SDHC_IsCommandBusy(host->channel))
        {
	        if ((now = get_timer(start)) > COMMAND_WAIT_TOUT)
    	    {
        	    printf("1 Timeout\n");
            	ret = -1; goto command_end;
        	}
        }
    }
    while (MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_HLE));

	/*
	// If Data Transfer is busy, you have to clear FIFO to send this command.	
	if( MES_SDHC_IsDataTransferBusy(host->channel) )
	{
		if( MES_SDHC_IsFIFOEmpty(host->channel) == CFALSE )
		{
			RETAILMSG(SDMMC_DEBUG_MSG, (TEXT("SendCommandInternal Fifo is not empty\n")));
			MES_SDHC_ResetFIFO(host->channel);               	// Reest the FIFO.
			while( MES_SDHC_IsResetFIFO(host->channel) );    	// Wait until the FIFO reset is completed.
		}
	}
	*/

    // Wait until Command sent to card and got response from card.
    start = get_timer(0);
    while (CFALSE == MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_CD))
    {
        if ((now = get_timer(start)) > COMMAND_WAIT_TOUT)
   	    {
       	    printf("1 Timeout\n");
           	ret = -1; goto command_end;
       	}
    }


    // Check Response Error
    if( MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_RCRC) )
    {
        gprintf("crc fail\n");
        ret = -1; goto command_end;
    }
    if(MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_RE))
    {
        gprintf("response error\n");
        ret = -1; goto command_end;
    }
    if(MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_RTO))
    {
    	gprintf("status response timeout\n");
    	ret = -1; goto command_end;
    }

    if( host->longrsp )
    {
		// Read response, long response일 경우 register의 순서가 뒤집힘.
		host->resp[0] = p->RESP3;
		host->resp[1] = p->RESP2;
		host->resp[2] = p->RESP1;
		host->resp[3] = p->RESP0;
    }
    else
	{
		// Read response
		host->resp[0] = p->RESP0;
		host->resp[1] = p->RESP1;
		host->resp[2] = p->RESP2;
		host->resp[3] = p->RESP3;
	}

	
	gprintf("longresp[0] = 0x%x\n", host->resp[0] );
	gprintf("longresp[1] = 0x%x\n", host->resp[1] );
	gprintf("longresp[2] = 0x%x\n", host->resp[2] );
	gprintf("longresp[3] = 0x%x\n", host->resp[3] );
	
	if( command == SEND_SCR )
	{
		gprintf("SCR fifocnt = %d\n", MES_SDHC_GetFIFOCount(host->channel) );	
	}
	

command_end:

    MES_SDHC_SetInterruptEnableAll(host->channel, CFALSE);
    MES_SDHC_ClearInterruptPendingAll(host->channel);


	return ret;
}


int sdmmc_SetClock(int channel, U32 divider )
{
	U32 timeout;
	
	MES_ASSERT( (1==divider) || (0==(divider&1)) );		// 1 or even number
	MES_ASSERT( (0<divider) && (510>=divider) );		// between 1 and 510
	
	
	// 1. Confirm that no card is engaged in any transaction.
	//    If there's a transaction, wait until it finishes.
//	while( MES_SDHC_IsDataTransferBusy() );
//	while( MES_SDHC_IsCardDataBusy() );
	if(  MES_SDHC_IsCardDataBusy(channel) )
	{
		gprintf("MES_SDCARD_SetClock : ERROR - Data is busy\n");
		return CFALSE;
	}
	
	if( MES_SDHC_IsDataTransferBusy(channel) )
	{
		gprintf("MES_SDCARD_SetClock : ERROR - Data Transfer is busy\n");
		return CFALSE;
	}
	
	// 2. Disable the output clock.
	MES_SDHC_SetOutputClockEnable( channel, CFALSE );
	
	// 3. Program the clock divider as required.
	MES_SDHC_SetOutputClockDivider( channel, divider );
	
	// 4. Start a command with MES_SDHC_CMDFLAG_UPDATECLKONLY flag.
repeat_4 :
	MES_SDHC_SetCommand( channel, 0, MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_UPDATECLKONLY | MES_SDHC_CMDFLAG_WAITPRVDAT );
	
	// 5. Wait until a update clock command is taken by the SDHC module.
	//    If a HLE is occurred, repeat 4.
	timeout = 0;
	while( MES_SDHC_IsCommandBusy(channel) )
	{
		if( ++timeout > 0x1000000 )
		{
			gprintf("MES_SDCARD_SetClock : ERROR - TIme-out to update clock.\n");
			return CFALSE;
		}
	}
	
	if( MES_SDHC_GetInterruptPending( channel, MES_SDHC_INT_HLE ) )
	{
		MES_SDHC_ClearInterruptPending( channel, MES_SDHC_INT_HLE );
		goto repeat_4;
	}
	
	// 6. Enable the output clock.
	MES_SDHC_SetOutputClockEnable( channel, CTRUE );
	
	// 7. Start a command with MES_SDHC_CMDFLAG_UPDATECLKONLY flag.
repeat_7 :
	MES_SDHC_SetCommand( channel, 0, MES_SDHC_CMDFLAG_STARTCMD | MES_SDHC_CMDFLAG_UPDATECLKONLY | MES_SDHC_CMDFLAG_WAITPRVDAT );
	
	// 8. Wait until a update clock command is taken by the SDHC module.
	//    If a HLE is occurred, repeat 7.
	timeout = 0;
	while( MES_SDHC_IsCommandBusy(channel) )
	{
		if( ++timeout > 0x1000000 )
		{
			gprintf("MES_SDCARD_SetClock : ERROR - TIme-out to update clock2.\n");
			return CFALSE;
		}
	}
	
	if( MES_SDHC_GetInterruptPending( channel, MES_SDHC_INT_HLE ) )
	{
		MES_SDHC_ClearInterruptPending( channel, MES_SDHC_INT_HLE );
		goto repeat_7;
	}
	
	
	return CTRUE;
}


int pollux_sdi_set_ios(struct pollux_sdi_host *host, int bus_width, int clock) 
{
	int prescaler;
	unsigned long sdclk = 0;

	gprintf("MMC_POWER_ON\n");
//	MES_SDHC_SetOutputClockEnable(host->channel, CTRUE);
	
	if( clock == 400000 )
		prescaler = SDCLK_DIVIDER_400KHZ;
	else if( clock == 25000000 )
		prescaler = SDCLK_DIVIDER_25MHZ;
	else if ( clock == 125000000 )
	    prescaler = SDCLK_DIVIDER_12MHZ;
	else if ( clock == 10000000 )	
		prescaler = SDCLK_DIVIDER_10MHZ;
	else	
		return -1;
		
	sdmmc_SetClock(host->channel, prescaler);
	gprintf("SD/MMC New clock value: %d / prescaler is %d\n", clock, prescaler);

#if 0	
    MES_SDHC_ResetFIFO(host->channel);  // Reset the FIFO.
    while (MES_SDHC_IsResetFIFO(host->channel));    // Wait until the FIFO reset is completed.
#endif

	host->clock = clock;

	/* Wait after clock setting */
	mdelay(100);
	gprintf("bus_width = %d\n", bus_width);
	host->bus_width = bus_width; // ios->bus_width is set automatically by reading CSR(card status register)
	
	if( host->bus_width == 0 ) // 1bit
		MES_SDHC_SetDataBusWidth(host->channel, 1);
	else if( host->bus_width == 2 ) // 4bit
		MES_SDHC_SetDataBusWidth(host->channel, 4);
		
	return 0;
}

 

/*
 * Initialize MP2530F SD/MMC controller
 */
static int sdmmc_init(struct pollux_sdi_host *host)
{
	int ret = 0;

	if( host->channel == 0 )
	{
		gprintf("set....channel 0 base address = %x\n", VIR_BASEADDR_SD);
		MES_SDHC_SetBaseAddress(host->channel, VIR_BASEADDR_SD); // memory base setting for sdmmc channel 0
	}
	else
	{
		gprintf("set....channel 1 base address = %x\n", VIR_BASEADDR_SD);
		MES_SDHC_SetBaseAddress(host->channel, VIR_BASEADDR_SD); // memory base setting for sdmmc channel 0
	}


    // SDHC_Def.h, 각 입력값들은 여기에 선언되어 있음
    MES_SDHC_SetClockPClkMode(host->channel, MES_PCLKMODE_ALWAYS);
    MES_SDHC_SetClockSource(host->channel, 0, SDHC_CLKSRC);  // 0, pclk
    MES_SDHC_SetClockDivisor(host->channel, 0, SDHC_CLKDIV); // 3, 
    MES_SDHC_SetOutputClockDivider(host->channel, SDCLK_DIVIDER_400KHZ); 
    MES_SDHC_SetOutputClockEnable(host->channel, CTRUE); // clock register control......==> enable
    MES_SDHC_SetClockDivisorEnable(host->channel, CTRUE); // clock divider enable
    MES_SDHC_OpenModule(host->channel); // sdi control register에서 interrupt enable bit를 세팅한다. c000_9800, c000_c800
    MES_SDHC_ResetController(host->channel);    // Reset the controller.
    while (MES_SDHC_IsResetController(host->channel));
                                                   
	// fifo랑 controller는 normal mode로 세팅, DMA만 reset, contol register
    MES_SDHC_ResetDMA(host->channel);   // Reset the DMA interface.
    // dma가 normal mode인지 검사
    while (MES_SDHC_IsResetDMA(host->channel)); // Wait until the DMA reset is
                                            // completed
	// 위의 두 놈이랑 비슷함
    MES_SDHC_ResetFIFO(host->channel);  // Reset the FIFO.
    while (MES_SDHC_IsResetFIFO(host->channel));    // Wait until the FIFO reset is 
                                                // completed
	// dma mode disable: 초기라서 그런가....ㅋㅋ
    MES_SDHC_SetDMAMode(host->channel, CFALSE);
    // low power는 안함
    MES_SDHC_SetLowPowerClockMode(host->channel, CFALSE);
    //MES_SDHC_SetLowPowerClockMode(host->channel, CTRUE);
	// data timeout 세팅해주고    
    MES_SDHC_SetDataTimeOut(host->channel, 0xFFFFFF);
    // command response timeout setting해주고
    MES_SDHC_SetResponseTimeOut(host->channel, 0xff);   // 0x64 );
    // data bit는 1bit로....아무래도 초기라서 그런 듯(mmc일지 모르니깐)
    MES_SDHC_SetDataBusWidth(host->channel, 1);

    //MES_SDHC_SetOutputClockEnable(host->channel, CTRUE); // clock register control......==> enable
    //MES_SDHC_SetClockDivisorEnable(host->channel, CTRUE); // clock divider enable

	// sdhc_def.h, block length는 역쉬 512
    MES_SDHC_SetBlockSize(host->channel, BLOCK_LENGTH);
    
    // 정확히 8이 의미하는 바를 모르겠음...메뉴얼에도 안나와 있음.
    // 32bytes의 half full에 대한 interrupt level을 말하는것 같기는 한데....웃긴다.
    MES_SDHC_SetFIFORxThreshold(host->channel, 8 - 1);  // Issue when RX FIFO Count  >= 8 x 4 bytes
    //MES_SDHC_SetFIFORxThreshold(host->channel, 2);  // Issue when RX FIFO Count  >= 2 x 4 bytes  ==> 안먹는다....이상함...
    MES_SDHC_SetFIFOTxThreshold(host->channel, 8);      // Issue when TX FIFO Count <= 8 x 4 bytes

	// 0으로 write한다....즉, 모든 mask clear ==> interrupt all enable?: 함수에서의 설명과는 틀린 듯...하여간 몽땅 enable시키는 것이기는 함
    MES_SDHC_SetInterruptEnableAll(host->channel, CFALSE);
	// interrupt status register all clear(write all bit to 1)
    MES_SDHC_ClearInterruptPendingAll(host->channel);
    
    host->sdi = (struct MES_SDHC_RegisterSet *)VIR_BASEADDR_SD;

	dump_sdi(host);

	return ret;
}


int Chk_SD_OCR(struct pollux_sdi_host *host, unsigned char *sdver)
{
    int i=0;
	int CntVer=0;
	int ret = 0;
	unsigned long respChk;

#if 0
   	if(*sdver) 
   		respChk = 0xc0ff8000;
    else 
    	respChk = 0x80ff8000;
#else
	respChk = 0x80ff8000;
#endif    	
    	
    gprintf("respChk = 0x%08x\n", respChk );
    
    //-- Negotiate operating condition for SD, it makes card ready state
	for(i=0;i<150;i++)
	{
		// command 55
		host->opcode = APP_CMD; host->arg = 0; host->longrsp = 0;
		ret = sdi_command(host);
		if( ret < 0 )
		{
			gprintf("sdi_command error8\n");
			return -1;
		}

        // ACMD41
		host->opcode = SD_SEND_OP_COND;
 		if(*sdver) 
 			host->arg = 0xff8000 | (1<<30);	// ACMD41(OCR:2.7V~3.6V), HCS bit 1
    	else 
    		host->arg = 0xff8000;
    	host->longrsp = 0;
    	
		ret = sdi_command(host);
		if( ret < 0 )
		{
			gprintf("sdi_command error9\n");
			return -1;
		}
		gprintf("host->resp[0] = 0x%08x\n", host->resp[0] );
		

	   	if(host->resp[0]==respChk) 
	    {
	    	udelay(20000);	 	// Wait Card power up status
	    	return 1;			// Success	    
		}
		else
		{
			if( *sdver )
			{
				if(host->resp[0] == 0x80ff8000) CntVer++;
				else CntVer=0;
			}
		
			if( CntVer==5 )
			{
				*sdver = 0; 	
				udelay(20000);
				return 1;
			}
			udelay(20000); 
    	}
    }
    
    return 0;			// Fail
}


int CMD9(struct pollux_sdi_host *host)//SEND_CSD
{
	int ret = 0;

	host->opcode = SEND_CSD; host->arg = ((RCA<<16) & 0xffff0000); host->longrsp = 1;	
	ret = sdi_command(host);
	if( ret < 0 )
	{
		gprintf("sdi_command error10\n");
		return -1;
	}
    
  	udelay(20000);
    return 1;
}


int CMD6(struct pollux_sdi_host *host) 
{
	int ret = 0;

	// command 55
	host->opcode = APP_CMD; host->arg = ((RCA<<16) & 0xffff0000); host->longrsp = 0;
	ret = sdi_command(host);
	if( ret < 0 )
	{
		gprintf("sdi_command error\n");
		return -1;
	}

	// ACMD 6
	host->opcode = SWITCH_FUNC; host->arg = (card_bus_width<<1); host->longrsp = 0;	 //Width 0: 1bit, 1: 4bit
	ret = sdi_command(host);
	if( ret < 0 )
	{
		gprintf("sdi_command error\n");
		return -1;
	}
    
    return 1;

}


void set_response_to_buf(struct pollux_sdi_host *host, unsigned char *rbuf)
{
	unsigned long tmp;
	
	tmp = (host->resp[0] >> 24) | ( (host->resp[0] >> 8) & 0xff00 ) | ( (host->resp[0] & 0xff00) << 8 ) | ( (host->resp[0] & 0xff) << 24); 
	memcpy(rbuf ,&tmp ,sizeof(tmp));
	tmp = (host->resp[1] >> 24) | ( (host->resp[1] >> 8) & 0xff00 ) | ( (host->resp[1] & 0xff00) << 8 ) | ( (host->resp[1] & 0xff) << 24); 
	memcpy(rbuf + (1 * sizeof(tmp)) ,&tmp ,sizeof(tmp));
	tmp = (host->resp[2] >> 24) | ( (host->resp[2] >> 8) & 0xff00 ) | ( (host->resp[2] & 0xff00) << 8 ) | ( (host->resp[2] & 0xff) << 24); 
	memcpy(rbuf + (2 * sizeof(tmp)) ,&tmp ,sizeof(tmp));
	tmp = (host->resp[3] >> 24) | ( (host->resp[3] >> 8) & 0xff00 ) | ( (host->resp[3] & 0xff00) << 8 ) | ( (host->resp[3] & 0xff) << 24); 
	memcpy(rbuf + (3 * sizeof(tmp)) ,&tmp ,sizeof(tmp));
}

 
 
int pollux_sd_init(void)
{
	int i;
	int ret,retry;
	unsigned char sdver;
	unsigned long blkSizeCnt = 0;
	CSD_regs csd;
	
	if(mmc_init_once == 1) return 1;	// already done !!
	
	MES_SDHC_Initialize(); //mes_sdhc.c 의 해당 레지스터 block 초기화 루틴
		
	/*
	 * Initialize MP2530F SDI
	 */
	ghost.channel = 0; /* channel #0 */
	ret = sdmmc_init(&ghost);
	if(ret != 0)
	{
		printf("sdinit error\n");
		return -1;
	}
	
	ret = pollux_sdi_set_ios(&ghost, 0, 400000); // initial state: bus width = 1bit, clock = 400khz
	if( ret < 0 )
	{
		printf("pollux_sdi set ios error\n");
		return -1;
	}
    
    retry = 5;
RETERY_CMD8:	
	// command 0
	ghost.opcode = 0; ghost.arg = 0; ghost.longrsp = 0;
	ret = sdi_command(&ghost);
	if( ret < 0 )
	{
		printf("sdi_command error1 %d \n",ghost.opcode);
		return -1;
	}
	
	// command 8	
	ghost.opcode = SEND_IF_COND; ghost.arg = (0xAA|(1<<8)); ghost.longrsp = 0;
	ret = sdi_command(&ghost);
	if( ret < 0 )
	    if(!retry--) goto RETERY_CMD8;
	
    if( (ghost.resp[0] & 0xff) == 0xAA ) 
	{
		printf("SD ver 2.0 \n");	
		sdver = 1;
	}
	else
	{
		printf("SD ver 1.0 or card none \n");
		sdver = 0;
	}
	
	if(Chk_SD_OCR(&ghost, &sdver)) 
	{
		isMMC=0;
		printf("\nSD found : ");
	}
    else {
        printf("SD not found \n");
        return -1; /* Not insert card */
    }
    
    
#if 0	
	else
	{
		printf("\nSD Initialize fail..\n\n");
		if( Chk_MMC_OCR(&ghost) )
		{
			printf("\nMMC found.\n");
			isMMC=1;
		}
		else
			return -1;
	}
#endif	
    
    
	// command 2
	ghost.opcode = ALL_SEND_CID; ghost.arg = 0; ghost.longrsp = 1;
	ret = sdi_command(&ghost);
	if( ret < 0 )
	{
		printf("sdi_command error2\n");
		return -1;
	}


	// command 3
	ghost.opcode = SEND_RELATIVE_ADDR; ghost.arg = 0; ghost.longrsp = 0;
	ret = sdi_command(&ghost);
	if( ret < 0 )
	{
		printf("sdi_command error3\n");
		return -1;
	}


	if(!(ghost.resp[0] & 0x600)) 
	{
		printf("CMD3( Read CSD) error \n");
		return -1;
	} 	
	
	
	// relative card address setting
	if(isMMC) 
	{
		RCA=1;
	}
	else
	{
		RCA=( ghost.resp[0] & 0xffff0000 )>>16;
		gprintf("RCA: 0x%04x\n", RCA);
	}	
	
	
	// command 9
	if( CMD9(&ghost)<0 ) // cmd9(send_CSD)
	{		
		printf("CMD9( Read CSD) error \n");
		return -1;
	}	 

	// CSD parsing
    set_response_to_buf(&ghost, resp_buf);
    gprintf("sdver = %d\n", sdver);
 	sdhc = str2csd(&csd, resp_buf, sdver);
 	if( sdhc < 0 ) 
 	{
 		printf("Not support SD VERSION !!!\n");
 		return sdhc; 
 	}	
	
	if( sdhc ) 
		blkSizeCnt = (1 + csd.c_size) * (0x01 << (csd.c_size_mult + 2));
    
	
	//printf("block size = %lu\n", blkSizeCnt );
	
	// command 7: card select/deselect ==> only select
	ghost.opcode = SELECT_CARD; ghost.arg = (RCA<<16) & 0xffff0000; ghost.longrsp = 0;
	ret = sdi_command(&ghost);
	if( ret < 0 )
	{
		printf("sdi_command error4\n");
		return -1;
	}
	
	if( ghost.resp[0] & (0x1e00!=0x800) ) // State(transfer) check
	{
		printf("cmd7 resp status error\n");
		return -1;
	}
	
	// command 55 & app command 51 not implement
	// clock to ==> 25Mhz, bus width = 4bit
	//ret = pollux_sdi_set_ios(&ghost, 2, 25000000); // initial state: bus width = 4bit, clock = 25Mhz
	ret = pollux_sdi_set_ios(&ghost, 2, 10000000); // initial state: bus width = 4bit, clock = 25Mhz
	if( ret < 0 )
	{
		printf("pollux_sdi set ios error\n");
		return -1;
	}

	// command 55 & app command 6, SD_APP_SET_BUS_WIDTH
	if( isMMC )
	{
		gprintf("card bus width ==> 1bit\n");
		card_bus_width=0;
		CMD6(&ghost);	// For MMC
	}
	else
	{
		gprintf("card bus width ==> 4bit\n");
		card_bus_width=1;
		CMD6(&ghost);	// For SD
	}
	udelay(50000);	
	
	// command 16: set block length
	ghost.opcode = SET_BLOCKLEN; ghost.arg = 0x200; ghost.longrsp = 0;
	ret = sdi_command(&ghost);
	if( ret < 0 )
	{
		printf("sdi_command error5\n");
		return -1;
	}
	
   
    {
		/* fill in device description */
		mmc_dev.if_type = IF_TYPE_MMC;
		mmc_dev.dev = 0;
		mmc_dev.lun = 0;
		mmc_dev.type = 0;
		/* FIXME fill in the correct size (is set to 32MByte) */
		mmc_dev.blksz = 512;
		//mmc_dev.lba = 0x10000;
		mmc_dev.lba = CardSize / 512;
		mmc_dev.part_type = PART_TYPE_DOS;
		/*
		sprintf(mmc_dev.vendor,"Man %02x%02x%02x Snr %02x%02x%02x",
				cid->id[0], cid->id[1], cid->id[2],
				cid->sn[0], cid->sn[1], cid->sn[2]);
		sprintf(mmc_dev.product,"%s",cid->name);
		sprintf(mmc_dev.revision,"%x %x",cid->hwrev, cid->fwrev);
		*/

		sprintf(mmc_dev.vendor,"Man AESOP snr pollux");
		sprintf(mmc_dev.product,"%s","SDxxxMB");
		sprintf(mmc_dev.revision,"%x %x",0x123,0x456);

		mmc_dev.removable = 0;
		mmc_dev.block_read = mmc_bread;

		ghost.init    = 1;
		fat_register_device(&mmc_dev,1); /* partitions start counting with 1 */

	}
	
	mmc_init_once = 1; // initialize done
	return 1;
}
 

int sd_read(unsigned char *rxd, unsigned long start_lba, unsigned long read_sectors)
{
	int ret = 0;
	int count = 0;
	int command = 0;
	int argument = 0;
	unsigned int *pbuf = (unsigned int *) rxd;
	struct pollux_sdi_host *host = &ghost;
	
	
	if( host->init == 0 )
	{
		printf("sd_read: host is not initialized\n");
		return -1;
	}
	
    count = read_sectors * BLOCK_LENGTH;
    MES_SDHC_SetByteCount(host->channel, count); // set read count
    
    gprintf("read count = %d\n", count);


    command  = (read_sectors > 1) ? READ_MULTIPLE_BLOCK : READ_SINGLE_BLOCK;
    argument = (sdhc) ? start_lba : start_lba * BLOCK_LENGTH;

	// command 17(single block read) or 18(multi block read)
	host->opcode = command; host->arg = argument; host->longrsp = 0;
	ret = sdi_command(host);
	if( ret < 0 )
	{
		printf("sdi_command error6\n");
		return -1;
	}

    while (1)
    {
        if (MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_RXDR))
        {
            MES_SDHC_GetData32(host->channel, pbuf);
            pbuf += (32 / 4);
            MES_SDHC_ClearInterruptPending(host->channel, MES_SDHC_INT_RXDR);

            count -= 32;
            if (0 == count)
            {
            	gprintf("rxd[510] = 0x%02x, rxd[511] = 0x%02x\n ", rxd[510], rxd[511] );
            	gprintf("break........\n");
                break;
            }
        }

        if (MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_DTO))
        {
            if (count >= 64)
            {
                printf("ERROR : MES_SDCARD_ReadSectors -MES_SDHC_INT_DTO count = %d\n", count);
                printf("ERROR : MES_SDCARD_ReadSectors -MES_SDHC_INT_DTO fifocount = %d\n", MES_SDHC_GetFIFOCount(host->channel) * 4);
                printf("ERROR : MES_SDCARD_ReadSectors dwReadSize = %d, D.TRCBCNT = %d\n", read_sectors * BLOCK_LENGTH, MES_SDHC_GetDataTransferSize(host->channel) );
                printf("ERROR : MES_SDCARD_ReadSectors dwReadSize = %d, D.TRFBCNT = %d\n", read_sectors * BLOCK_LENGTH, MES_SDHC_GetFIFOTransferSize(host->channel) );

                dump_sdi(host);

                ret = -1;
                goto data_end;
            }
			gprintf("DTO.............\n");
            MES_SDHC_GetData32(host->channel, pbuf);
            pbuf += (32 / 4);
            MES_SDHC_ClearInterruptPending(host->channel, MES_SDHC_INT_DTO);

            count -= 32;
            if (0 == count)
            {
            	gprintf("DTO rxd[510] = 0x%02x, rxd[511] = 0x%02x\n ", rxd[510], rxd[511] );
            	gprintf("DTO break........\n");
                break;
            }
            break;
        }

		// debug....?
	    if( CTRUE ==  MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_HTO) ) // Data Startvation by Host Timeout
    	{
        	printf("ERROR : Detect MES_SDHC_INT_HTO \n");
			MES_SDHC_ClearInterruptPending(host->channel, MES_SDHC_INT_HTO);			
    	}
    	
        if (CTRUE == MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_FRUN))
        {
            printf("ERROR : MES_SDCARD_ReadSectors - MES_SDHC_INT_FRUN\n");
            MES_SDHC_ClearInterruptPending(host->channel, MES_SDHC_INT_FRUN);
            // goto End; // ghcstop: why?
        }
    }

    /* 
     * Check Errors
     *
     MES_SDHC_INT_DRTO: Data Read Timeout, Data timeout occurred. Data Transfer Over (MES_SDHC_INT_DTO) also set if data timeout occurs.
     MES_SDHC_INT_EBE : End Bit Error for reading, Write no CRC for wrting.
     MES_SDHC_INT_SBE : Start Bit Error.
     MES_SDHC_INT_DCRC: Data CRC Error.
     */
    if( MES_SDHC_GetInterruptPending32(host->channel) & ( (1<<MES_SDHC_INT_DRTO)|(1<<MES_SDHC_INT_EBE)|(1<<MES_SDHC_INT_SBE)|(1<<MES_SDHC_INT_DCRC)  )  )
    {
        if( MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_DRTO) )
			printf("ERROR : MES_SDCARD_ReadSectors - MES_SDHC_INT_DRTO\n");
        if( MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_EBE) )
           	printf("ERROR : MES_SDCARD_ReadSectors - MES_SDHC_INT_EBE\n");
        if( MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_SBE) )
           	printf("ERROR : MES_SDCARD_ReadSectors - MES_SDHC_INT_SBE\n");
        if( MES_SDHC_GetInterruptPending(host->channel, MES_SDHC_INT_DCRC) )
            printf("ERROR : MES_SDCARD_ReadSectors - MES_SDHC_INT_DCRC\n");

        printf("READ ERROR : MES_SDHC_GetInterruptPending32()\n");
        ret = -1;
        goto data_end;
    }


    MES_SDHC_ClearInterruptPendingAll(host->channel);


    if( 0 != MES_SDHC_GetFIFOCount(host->channel) )
    {
        printf("ERROR : MES_SDCARD_ReadSectors not empty then ?? fifocount*4 = %d\n\n\n",  MES_SDHC_GetFIFOCount(host->channel) * 4 );
    }


    if( CFALSE == MES_SDHC_IsFIFOEmpty(host->channel) )
    {
    	printf("Fifo is not empty...........1\n");
    	ret = -1;
    	goto data_end;
    }
    
    if( CTRUE == MES_SDHC_IsDataTransferBusy(host->channel) )
    {
    	printf("Data transfer busy...........1\n");
    	ret = -1;
    	goto data_end;
    }



	if(read_sectors>1) // if muiti block read ==> send stop command( command 12 )
	{
		// command 12(single block read) or 18(multi block read)
		host->opcode = STOP_TRANSMISSION; host->arg = 0; host->longrsp = 0;
		ret = sdi_command(host);
		if( ret < 0 )
		{
			printf("sdi_command error7\n");
			return -1;
		}
	}
	
data_end:
    //MES_SDHC_SetInterruptEnableAll(host->channel, CFALSE);
    MES_SDHC_ClearInterruptPendingAll(host->channel);
    
    //dump_sdi(host);

	return ret; // no error일 경우는 MMC_ERR_NONE, 나머지는 에러에 따라서
	
}
 


/*
 **************************************************************************
 *  MMC stuff
 **************************************************************************
 */


block_dev_desc_t * mmc_get_dev(int dev)
{
	return ((block_dev_desc_t *)&mmc_dev);
}


ulong mmc_bread(int dev_num, ulong blknr, ulong blkcnt, ulong *dst)
{
//	int mmc_block_size = MMC_BLOCK_SIZE;
//	ulong src = blknr * mmc_block_size + CFG_MMC_BASE;

	if(blkcnt==0)
	{
		printf("Why block_cnt == 0?? \n");
		return 0;
	}
//	mmc_read(src, (uchar *)dst, blkcnt*mmc_block_size);
//	printf("mmc_bread LBA=%d, Count=%d\n",blknr,blkcnt);
	sd_read((uchar *)dst, blknr, blkcnt);
//	printf("mmc_bread end\n");	
	return blkcnt;
}

int mmc_write(uchar *src, ulong dst, int size)
{
	printf("\nmmc_write should not be called !!!!\n");
}


int mmc_read(ulong src, uchar *dst, int size)
{
	printf("\nmmc_read should not be called !!!!\n");
}

#define INICLK	400000
#define SDCLK	25000000	
#define MMCCLK	15000000	


int mmc_init(int verbose)
{
	pollux_sd_init();
	return 0;
}

#define CFG_MMC_BASE 0
int mmc2info(ulong addr)
{
	/* FIXME hard codes to 32 MB device */
	if (addr >= CFG_MMC_BASE && addr < CFG_MMC_BASE + (CardSize/512))
	{
		return 1;
	}
	return 0;
}



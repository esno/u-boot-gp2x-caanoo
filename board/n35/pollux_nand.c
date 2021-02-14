//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

#include <common.h>
#include <asm/arch/mes_pollux.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>
#include "pollux_nand_config.h"

#define NAND_WRENCDONE		0x1
#define NAND_RDDECDONE		0x2
#define NAND_RDCHECKERR		0x4

#define ECC_SIZE        	16        

extern short BCH_AlphaToTable[8192];
extern short BCH_IndexOfTable[8192];

volatile PNANDECC	pECC;
volatile PNANDOFFSET   pNANDOffset;
volatile unsigned int *p_WP;

static unsigned int		ggnCycle = NAND_ADDRESS_TYPE;
BedCheck bed_cntl;
unsigned char	wData[8]= {0xff, 0xff, 0xff, 0xff, 0xff, NAND_MAGIC1, NAND_MAGIC2, NAND_MAGIC3};

void reset_ecc( void )
{
    const U32   ECCRST_MASK = 1UL << 11;
    const U32   IRQPEND_MASK = 1UL << 15;

    register U32 regvalue;
 
    regvalue = pECC->reg_NFCONTROL;

    regvalue &= ~(IRQPEND_MASK | ECCRST_MASK );
    regvalue |= ECCRST_MASK;

    pECC->reg_NFCONTROL = regvalue;
}

void ReadOddSyndrome(int  *s)
{
    int syn_val13, syn_val57;
    
    syn_val13 = pECC->reg_NFSYNDRONE31;
    syn_val57 = pECC->reg_NFSYNDRONE75;

    s[0] = syn_val13 & 0x1fff;
    s[2] = (syn_val13>>13) & 0x1fff;
    s[4] = syn_val57 & 0x1fff;
    s[6] = (syn_val57>>13) & 0x1fff;
}

//------------------------------------------------------------------------------
int	MES_NAND_GetErrorLocation(  int  *s, int *pLocation )
{
	register int i, j, elp_sum ;
	int count;
	int r;				// Iteration steps
	int Delta; 			// Discrepancy value
	int elp[8+1][8+2]; 	// Error locator polynomial (ELP)
	int L[8+1];			// Degree of ELP 
	int B[8+1][8+2];	// Scratch polynomial
	int reg[8+1];		// Register state


	// Even syndrome = (Odd syndrome) ** 2
	for( i=1,j=0 ; i<8 ; i+=2, j++ )
	{ 	
		if( s[j] == 0 )		s[i] = 0;
		else				s[i] = BCH_AlphaToTable[(2 * BCH_IndexOfTable[s[j]]) % 8191];
	}
	
	// Initialization of elp, B and register
	for( i=0 ; i<=8 ; i++ )
	{	
		L[i] = 0 ;
		for( j=0 ; j<=8 ; j++ )
		{	
			elp[i][j] = 0 ;
			B[i][j] = 0 ;
		}
	}

	for( i=0 ; i<=4 ; i++ )
	{	
		reg[i] = 0 ;
	}
	
	elp[1][0] = 1 ;
	elp[1][1] = s[0] ;

	L[1] = 1 ;
	if( s[0] != 0 )
		B[1][0] = BCH_AlphaToTable[(8191 - BCH_IndexOfTable[s[0]]) % 8191];
	else
		B[1][0] = 0;
	
	for( r=3 ; r<=8-1 ; r=r+2 )
	{	
		// Compute discrepancy
		Delta = s[r-1] ;
		for( i=1 ; i<=L[r-2] ; i++ )
		{	
			if( (s[r-i-1] != 0) && (elp[r-2][i] != 0) )
				Delta ^= BCH_AlphaToTable[(BCH_IndexOfTable[s[r-i-1]] + BCH_IndexOfTable[elp[r-2][i]]) % 8191];
		}
		
		if( Delta == 0 )
		{	
			L[r] = L[r-2] ;
			for( i=0 ; i<=L[r-2] ; i++ )
			{	
				elp[r][i] = elp[r-2][i];
				B[r][i+2] = B[r-2][i] ;
			}
		}
		else
		{	
			// Form new error locator polynomial
			for( i=0 ; i<=L[r-2] ; i++ )
			{	
				elp[r][i] = elp[r-2][i] ;
			}

			for( i=0 ; i<=L[r-2] ; i++ )
			{	
				if( B[r-2][i] != 0 )
					elp[r][i+2] ^= BCH_AlphaToTable[(BCH_IndexOfTable[Delta] + BCH_IndexOfTable[B[r-2][i]]) % 8191];
			}
			
			// Form new scratch polynomial and register length
			if( 2 * L[r-2] >= r )
			{	
				L[r] = L[r-2] ;
				for( i=0 ; i<=L[r-2] ; i++ )
				{	
					B[r][i+2] = B[r-2][i];
				}
			}
			else
			{	
				L[r] = r - L[r-2];
				for( i=0 ; i<=L[r-2] ; i++ )
				{	
					if( elp[r-2][i] != 0 )
						B[r][i] = BCH_AlphaToTable[(BCH_IndexOfTable[elp[r-2][i]] + 8191 - BCH_IndexOfTable[Delta]) % 8191];
					else
						B[r][i] = 0;
				}
			}
		}
	}
	
	if( L[8-1] > 4 )
	{	
		//return L[8-1];
		return -1;
	}
	else
	{	
		// Chien's search to find roots of the error location polynomial
		// Ref: L&C pp.216, Fig.6.1
		for( i=1 ; i<=L[8-1] ; i++ )
			reg[i] = elp[8-1][i];
		
		count = 0;
		for( i=1 ; i<=8191 ; i++ )
		{ 	
			elp_sum = 1;
			for( j=1 ; j<=L[8-1] ; j++ )
			{	
				if( reg[j] != 0 )
				{ 	
					reg[j] = BCH_AlphaToTable[(BCH_IndexOfTable[reg[j]] + j) % 8191] ;
					elp_sum ^= reg[j] ;
				}
			}
			
			if( !elp_sum )		// store root and error location number indices
			{ 	
				// Convert error location from systematic form to storage form 
				pLocation[count] = 8191 - i;
				
				if (pLocation[count] >= 52)
				{	
					// Data Bit Error
					pLocation[count] = pLocation[count] - 52;
					pLocation[count] = 4095 - pLocation[count];
				}
				else
				{
					// ECC Error
					pLocation[count] = pLocation[count] + 4096;
				}

				if( pLocation[count] < 0 )	return -1;

				count++;
			}
		}
		
		if( count == L[8-1] )	// Number of roots = degree of elp hence <= 4 errors
		{	
			return L[8-1];
		}
		else	// Number of roots != degree of ELP => >4 errors and cannot solve
		{	
			return -1;
		}
	}
}

void *fmd_Init(void)
{
	// Caller should have specified NAND controller address.
	//	
	pNANDOffset = (PNANDOFFSET)CFG_NAND_BASE;
	pECC	    = (PNANDECC)PHY_BASEADDR_NANDECC;

	CLEAR_RnB();
	NF_CMD(CMD_RESET);		// Send reset command.

	return((void *)pNANDOffset);
}


CBOOL fmd_ReadSector(
	unsigned long  startSectorAddr, 
	unsigned char *pSectorBuff,     
	PSectorInfo    pSectorInfoBuff, // spare area buffer, 위의 buffer와 or이다....즉, spare만도 읽을 수 있음.
	unsigned int   dwNumSectors)    
{
	unsigned long SectorAddr = (unsigned long)startSectorAddr;
	unsigned int *pdwSectorAddr;
	unsigned int dwECC[ECC_SIZE];
	int i, j;
	unsigned int loop;
    unsigned int dwByteCnt = NAND_PAGE_SIZE;
	
	CLEAR_RnB();

	while (dwNumSectors--) 
	{
		unsigned long blockPage	= SectorAddr; 
		unsigned long DataOffset= 0, SpareOffset= 0;
        SpareOffset	= NAND_PAGE_SIZE;

		if (pSectorBuff)
		{			
			CLEAR_RnB();
			NF_SETREAD(blockPage, SpareOffset, ggnCycle, CTRUE);  // sector 시작의 spare area를 읽는다.		
			// Read page/sector information.
			for ( i=0; i<16; i++) // 64 bytes skip
			{
				unsigned int ch;
				ch= NF_RDDATA32(); 					
			}
			
			// Read the ECC info
			for( i=0; i<ECC_SIZE; i++)
			{
				dwECC[i] = NF_RDDATA32(); 	
			}
		}

		
		for(loop = 0 ; loop < dwByteCnt/512 ; loop++) 
		{
			if (pSectorBuff)
			{			
				pECC->reg_NFORGECCL	= dwECC[((loop * 2) + 0)]; 
				pECC->reg_NFORGECCH	= dwECC[((loop * 2) + 1)]; 

				CLEAR_RnB();
				NF_SETREAD(blockPage, DataOffset + loop * 512, ggnCycle, CTRUE); 
	
				//  Handle unaligned buffer pointer
				if( ((unsigned int) pSectorBuff) & 0x3 ) 
				{
					for ( i=0; i<512; i++) 
					{
						unsigned char ch;
	
						ch= NF_RDDATA();
						*((unsigned char *)pSectorBuff+i)= ch;
					}
				}
				else 
				{
					for ( i=0; i<512/4; i++)
					{
						unsigned int ch;
	
						ch= NF_RDDATA32();
						*((unsigned int *)pSectorBuff+i)= ch;
						
					}
				}

				while(0 == (pECC->reg_NFECCSTATUS & (NAND_RDDECDONE ))); 
				
				if( pECC->reg_NFECCSTATUS & NAND_RDCHECKERR) 	
				{
					int dSyndrome[8];
				   	int	ErrCnt, ErrPos[4];
					
			        ReadOddSyndrome(dSyndrome);
       				pdwSectorAddr = (unsigned int *)pSectorBuff;

					ErrCnt = MES_NAND_GetErrorLocation( dSyndrome, ErrPos );

					for( j=0 ; j<ErrCnt ; j++ )
					{
						pdwSectorAddr[ErrPos[j]/32] ^= 1<<(ErrPos[j]%32);
					}		        
				}											

				pSectorBuff += 512; 
			}
		}
	
		if( pSectorBuff ) 
		{
	        NF_SETREADSPARE_SEQ( SpareOffset );
		}
		else 
		{
		    CLEAR_RnB();
			NF_SETREAD(blockPage, SpareOffset, ggnCycle, CTRUE);
		}
		
		if (pSectorInfoBuff) 
		{
			// Read page/sector information.
			for( i=0; i<8; i++)
			{
				unsigned char ch;
				ch= NF_RDDATA();
				*((unsigned char *)pSectorInfoBuff+i)= ch;
			}

			pSectorInfoBuff++;
		}

		++SectorAddr;
	}
		
	return(CTRUE);
}

CBOOL fmd_EraseBlock(unsigned long page) 
{
	unsigned char Status;
    
	CLEAR_RnB();
    NF_SETERASE( page, ggnCycle );
	CHECK_RnB();

	NF_CMD ( CMD_READ_STATUS );
	Status = NF_RDDATA();

    return((Status & 1) ? CFALSE : CTRUE);
}


CBOOL fmd_WriteSector(
	unsigned long  startSectorAddr,   
	unsigned char *pSectorBuff,       
	PSectorInfo    pSectorInfoBuff,   
	unsigned int   dwNumSectors)      
{
	unsigned long SectorAddr = (unsigned long)startSectorAddr;
	unsigned int  dwByteCnt= NAND_PAGE_SIZE;
	unsigned int  dwECC[ECC_SIZE];
	
	int i;
	unsigned int loop;
	
	if (!pSectorBuff && !pSectorInfoBuff) 
		return(CFALSE);

	while (dwNumSectors--)
	{
		unsigned long blockPage	= SectorAddr;
		unsigned long DataOffset= 0, SpareOffset= 0;
        SpareOffset	= NAND_PAGE_SIZE;
	    dwByteCnt 	= NAND_PAGE_SIZE;
				
		CLEAR_RnB();			
        if (!pSectorBuff) //spare만 write할 경우
		{
            // If we are asked just to write the SectorInfo,
            // we will do that separately
				
            NF_SETWRITE( blockPage, SpareOffset, ggnCycle );
			for ( i=0; i< 8; i++)
			{
                unsigned char ch;
	            ch= *((unsigned char *)pSectorInfoBuff+i);
				NF_WRDATA(ch);
			}
			pSectorInfoBuff++;
		}
		else
		{
			NF_SETWRITE( blockPage, DataOffset , ggnCycle );
	        for(loop = 0 ; loop < dwByteCnt/512 ; loop++)
			{
                // Special case to handle un-aligned buffer pointer.
                if( ((unsigned int) pSectorBuff) & 0x3)
				{	
                    for ( i=0; i<512; i++)
					{
					    unsigned char ch;
		                ch= *((unsigned char *)pSectorBuff+i);
						NF_WRDATA(ch);
					}	
				}
				else
				{	
				    for ( i=0; i<512/4; i++)
					{
					    unsigned int ch;					
						ch= *((unsigned int *)pSectorBuff+i);
						NF_WRDATA32(ch);
					}
				}				

				if(pSectorBuff)
				{			
					while(0 == (pECC->reg_NFECCSTATUS & NAND_WRENCDONE));
					
					dwECC[((loop * 2) + 0)] = pECC->reg_NFECCL;
					dwECC[((loop * 2) + 1)] = pECC->reg_NFECCH;
					NF_SETWRITERANDOM((loop + 1) * 512);			
				}				

				pSectorBuff += 512;
			}
		}

		if (pSectorBuff)
		{
			// Write the SectorInfo data to the media.
			//			
			NF_SETWRITESPARE_SEQ( SpareOffset );
			if(pSectorInfoBuff)
			{
				for ( i=0; i<8; i++)
				{
					unsigned char ch;
					ch= *((unsigned char *)pSectorInfoBuff+i);
					NF_WRDATA(ch);
				}
				pSectorInfoBuff++;
			}
			else
			{
				// Make sure we advance the Flash's write pointer
				// (even though we aren't writing the SectorInfo data)
				unsigned char TempInfo[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
				for ( i=0; i<8; i++)
				{
					unsigned char ch;
					ch= *((unsigned char *)TempInfo+i);
					NF_WRDATA(ch);
				}
			}		
			
			{
				unsigned char ch = 0xff; 
				
				for(i=0;i<(24+32);i++) NF_WRDATA(ch);
			
				for( i=0; i<ECC_SIZE; i++) {
					NF_WRDATA32(dwECC[i]);
				}
			}
						
		}

		NF_CMD( CMD_WRITE_CONFIRM );

		CHECK_RnB();

		++SectorAddr;
	}

	return(CTRUE);
}


void fmd_check_kblocks(void)
{
    int i;
    unsigned long page;
    int j, bed_cnt =0, good_cnt=0;
         
    bed_cntl.all_bed_block0 = 0; 
    bed_cntl.all_bed_block1 = 0; 
    
    for( i = START_BLOCK_KERNEL0; i < MAX_BLOCK_KERNEL0; i++ )
    {
        page = i * NAND_PAGE_CNT;
        if ( IsBlockBad(page) ){
             bed_cnt++;
        }
        else  {
            bed_cntl.good_block0[j] = i;
            good_cnt++; 
        }
        
        if( good_cnt == MAX_GOOD_BLOCKS) break;
        j++;
    }
        
    bed_cntl.bed_cnt0 = bed_cnt;
    if( bed_cnt >= MAX_BAD_BLOCKS ) bed_cntl.all_bed_block0 = 1;
    
    bed_cnt = 0, good_cnt = 0, j=0;
    for( i = START_BLOCK_KERNEL1; i < MAX_BLOCK_KERNEL1; i++ )
    {
        page = i * NAND_PAGE_CNT;
        if ( IsBlockBad(page) ){
             bed_cnt++;
        }
        else {
            bed_cntl.good_block1[j] = i;
            good_cnt++; 
        }
        
        if( good_cnt == MAX_GOOD_BLOCKS ) break;
        j++;
    }
    
    bed_cntl.bed_cnt1 = bed_cnt;
    if( bed_cnt > MAX_BAD_BLOCKS ) bed_cntl.all_bed_block1 = 1; 
}

CBOOL IsBlockBad(unsigned long blockPage)
{
	unsigned char Data[16];	
	
	CLEAR_RnB();
    // Read Spare
	fmd_ReadSector( blockPage, NULL, (PSectorInfo)Data, 1);

    if( (0xff != Data[0]) || (0xff != Data[1]) )
	    return(CTRUE);
	       
    
    return(CFALSE);
}

CBOOL MarkBlockBad(unsigned int blockID)
{
	unsigned char	Data[8]= {0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff};
	CBOOL	bRet;
	unsigned long 	blockPage = (blockID * NAND_PAGE_CNT);

	CLEAR_RnB();
	bRet= fmd_WriteSector( (unsigned long)blockPage, NULL, (PSectorInfo)Data, 1);

	return bRet;
}

void fmd_checkid(void)
{
	unsigned char ch;
	int i;
	
	CLEAR_RnB();
	NF_CMD(CMD_READ_ID);		// Send reset command.
	NF_ADDR(0x0)	
	
	for(i=0; i<10; i++);
	
	ch= NF_RDDATA();
	
	printf("maf: 0x%02x\n", ch);
	ch= NF_RDDATA();
	
	printf("dev: 0x%02x\n", ch);
	CLEAR_RnB();
}
	
	
int do_nandE( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[] )
{
	int startblk, size, eraseblocks, i;
	CBOOL ret;
    unsigned long page;
    
	if(flag)
	{
	    if( flag == CFG_FW_BOOT)
	    {
	        startblk = 0;
            size     = 0x80000;          
	    }
	    else if(flag == CFG_FW_KERNEL)
	    {
	        int i, e_cnt =0, is_all_bed = bed_cntl.all_bed_block0;
            unsigned char *p_eblock = bed_cntl.good_block0;
            
ERASE_KERNEL:            
            if(!is_all_bed)
            {
                eraseblocks = MAX_GOOD_BLOCKS;
                for ( i = 0; i < eraseblocks; i++)
                {
                    startblk = p_eblock[i];
                    page = startblk * NAND_PAGE_CNT;
	                ret = fmd_EraseBlock( page);
                    if( ret == CFALSE )
                        printf("Block erase error num:  %d  \n", startblk );    
                }        
            } /* is_all_bed == 0) */
            e_cnt++;
                     
            if(e_cnt < 2)
            {
                is_all_bed = bed_cntl.all_bed_block1;
                p_eblock = bed_cntl.good_block1;
                goto ERASE_KERNEL;
            }           
            
            return 0;
        } /* flag == CFG_FW_KERNEL */		    
	}
	else
	{
	    if ( argc != 3 )
	    {
    	    printf ("Usage:\n%s\n", cmdtp->help);
            return 1;
        }
        
        startblk = simple_strtoul(argv[1], NULL, 10);
        size     = simple_strtoul(argv[2], NULL, 16);        
    }
           
    eraseblocks = size / NAND_BLOCK_SIZE;
    printf("StartBlockNo %d (addr :0x%x) : Erase Blocks %d (0x%x) \n", startblk, startblk * NAND_BLOCK_SIZE, eraseblocks, size);
    
    for ( i=0 ; i < eraseblocks;  i++) 
    {
        page = startblk * NAND_PAGE_CNT; 
        ret = fmd_EraseBlock( page);
        if( ret == CFALSE )
            printf("Block erase error num:  %d  \n", startblk );
        
        startblk++;
    }
    
	return 0;
}
	
U_BOOT_CMD(
	nande,	4,	1,	do_nandE,
       "nande	- delete block \n",
       "[startblock in hex] [size in hex] \n"		\
);



int do_nandw    (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong startblk, size, memadr, startpage, numofpages;
	CBOOL ret;

    if(flag)
	{
        memadr = CFG_FW_ADDR;
	    if( flag == CFG_FW_BOOT)
	    {
	        startblk = 0;
            size     = 0x3F000;     //252kb	        
	    }
	    else if(flag == CFG_FW_KERNEL)
	    {
            int i, wflag =0, is_all_bed = bed_cntl.all_bed_block0;
            int w_fail = 0, bed_cnt = bed_cntl.bed_cnt0;
            unsigned char *p_wblock = bed_cntl.good_block0;
            unsigned char *ptr = NULL;
            
UPDATA_KERNEL:            
            if(!is_all_bed)
            {
                bed_cnt = 1;   // hyun
                if(!bed_cnt)
                {
                    size     = 0x300000; //3MB
                    startblk = *p_wblock;
                    startpage = startblk * NAND_PAGE_CNT;
	                numofpages = size / NAND_PAGE_SIZE;
	                                    
                    if( CFALSE == (fmd_WriteSector(startpage, (unsigned char *)memadr, NULL, numofpages ) ) )
                    {        
                        w_fail = 1;
                        printf(" write error \n");
                    }                   	                
                }
                else
                {
                    size = 0x80000;
                    for ( i = 0; i < MAX_GOOD_BLOCKS; i++)
                    {
                        startblk = p_wblock[i];
                        startpage = startblk * NAND_PAGE_CNT;
	                    numofpages = size/ NAND_PAGE_SIZE;
	                    printf("startpage: %d(%d*%d), numofpages: %d, memaddr = 0x%x\n", startpage, startblk, NAND_PAGE_CNT, numofpages, memadr);
	                    
                        if( i == 0 ) ptr = wData;
                        else ptr = NULL;

                        if( CFALSE == (fmd_WriteSector(startpage, (unsigned char *)memadr, (PSectorInfo)ptr, numofpages) ) )
                        {    
                            w_fail = 1;
                            printf(" write error \n");
                            break;
                        }                   
	                    memadr += size;
                    }
                }        
            } else w_fail = 1;  /* is_all_bed == 0) */
#if 0            
            if (!w_fail ){
            
                startpage = *p_wblock * NAND_PAGE_CNT;
	            printf(" start page:%d  \n", startpage);
	            CLEAR_RnB();
	            fmd_WriteSector( startpage, NULL, (PSectorInfo)wData, 1);
            }    
#endif
            
            if(!wflag)
            {
                wflag = 1;
                w_fail = 0 ;
                memadr = CFG_FW_ADDR;
                is_all_bed = bed_cntl.all_bed_block1;
                p_wblock = bed_cntl.good_block1;
                bed_cnt = bed_cntl.bed_cnt1;
                goto UPDATA_KERNEL;
            }
            return 0;        
        }	/* flag == CFG_FW_KERNEL */	    
	}  
	else
	{
	    if (argc !=  4) 
	    {
		    printf ("Usage:\n%s\n", cmdtp->help);
		    return 1;
	    }

	    startblk = simple_strtoul(argv[1], NULL, 10);
	    size     = simple_strtoul(argv[2], NULL, 16);
	    memadr   = simple_strtoul(argv[3], NULL, 16);	    	
	}

	startpage = startblk * NAND_PAGE_CNT;
	numofpages = size/NAND_PAGE_SIZE;
	
	if( size%NAND_PAGE_SIZE )
		numofpages++;
		
	printf("startpage: %d(%d*%d), numofpages: %d, memaddr = 0x%x\n", startpage, startblk, NAND_PAGE_CNT, numofpages, memadr);

	ret = fmd_WriteSector(startpage, (unsigned char *)memadr, NULL, numofpages );
    if( ret == CFALSE )
        printf("write error\n");

	return 0;
}


U_BOOT_CMD(
	nandw,	4,	1,	do_nandw,
       "nandw	- NAND Flash Write Program \n",
       "[startblock in decimal] [size in hex] [memaddr in hex] \n"		\
);


int do_nandr    (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	startblk, size, memadr, startpage, numofpages;
	CBOOL ret;
	
	if(flag)
	{
        int i, is_all_bed = bed_cntl.all_bed_block0;
        int r_fail = 0, read_cnt = 0, bed_cnt = bed_cntl.bed_cnt0;
        unsigned char *p_rblock = bed_cntl.good_block0;

LOAD_KERNEL:            
        if(!is_all_bed)
        {
            unsigned char Data[16];	
	        CLEAR_RnB();
            startpage = *p_rblock * NAND_PAGE_CNT;

            fmd_ReadSector( startpage, NULL, (PSectorInfo)Data, 1);
            if( (Data[NAND_POS_MAGIC1] != NAND_MAGIC1) || (Data[NAND_POS_MAGIC2] != NAND_MAGIC2) || 
                   (Data[NAND_POS_MAGIC3] != NAND_MAGIC3) )
                r_fail = 1;

            memadr = CFG_KERNEL_LOAD_ADDR;
            if(!bed_cnt && !r_fail)
            {
                size     = 0x300000; //3MB
                startblk = *p_rblock;
                startpage = startblk * NAND_PAGE_CNT;
	            numofpages = size/ NAND_PAGE_SIZE;
	                
                if( CFALSE == (fmd_ReadSector(startpage, (unsigned char *)memadr, NULL, numofpages ) ) )
                {        
                    r_fail = 1;
                    printf(" read error \n");
                }                   	                
            }
            else if(!r_fail)
            {
                size = 0x80000;
                for ( i = 0; i < MAX_GOOD_BLOCKS; i++)
                {
                    startblk = p_rblock[i];
                    startpage = startblk * NAND_PAGE_CNT;
	                numofpages = size/ NAND_PAGE_SIZE;
	                
                    if( CFALSE == (fmd_ReadSector(startpage, (unsigned char *)memadr, NULL, numofpages ) ) )
                    {    
                        r_fail = 1;
                        printf(" write error \n");
                        break;
                    }                   
	                memadr += size;
                }
            }                    
        }else r_fail = 1;
                 
        read_cnt++;
                        
        if (r_fail && read_cnt < 2)
        {
            r_fail = 0;
            memadr = CFG_KERNEL_LOAD_ADDR;
            is_all_bed = bed_cntl.all_bed_block1;
            p_rblock = bed_cntl.good_block1;
            bed_cnt = bed_cntl.bed_cnt1;
            goto LOAD_KERNEL;
        }           
        
        return 0;
    } /* flag == 1 */
	else
	{
	    if(argc !=  4) 
	    {
		    printf ("Usage:\n%s\n", cmdtp->help);
		    return 1;
	    }
    
	    startblk = simple_strtoul(argv[1], NULL, 0);
	    size     = simple_strtoul(argv[2], NULL, 16);
	    memadr   = simple_strtoul(argv[3], NULL, 16);
    }
	
	startpage = startblk * NAND_PAGE_CNT;
    numofpages = size / NAND_PAGE_SIZE;
	
	if( size%NAND_PAGE_SIZE )
		numofpages++;
		
	printf("startpage: %d(%d*%d), numofpages: %d, memaddr = 0x%x\n", startpage, startblk, NAND_PAGE_CNT, numofpages, memadr);

	ret = fmd_ReadSector(startpage, (unsigned char *)memadr, NULL, numofpages );
    if( ret == CFALSE )
    {
        printf("read error\n");
    }
	return 0;
}


U_BOOT_CMD(
	 nandr,	4,	1,	do_nandr,
       "nandr	- NAND Flash Read Program \n",
       "[startblock in decimal] [size in hex] [memaddr in hex] \n"		\
);

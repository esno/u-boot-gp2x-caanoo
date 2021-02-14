
#ifndef __NAND_CONFIG_H__
#define __NAND_CONFIG_H__

#define PHY_BASEADDR_NANDECC			0xC0015874

typedef volatile struct _tag_NAND_ECC 
{
	volatile U32	reg_NFCONTROL;			// 0x5874
	volatile U32	reg_NFECCL;				// 0x5878
	volatile U32	reg_NFECCH;				// 0x587c
	volatile U32	reg_NFORGECCL;			// 0x5880
	volatile U32	reg_NFORGECCH;			// 0x5884
	volatile U32	reg_NFCNT;				// 0x5888
	volatile U32	reg_NFECCSTATUS;		// 0x588c
	volatile U32	reg_NFSYNDRONE31;		// 0x5890
	volatile U32	reg_NFSYNDRONE75;		// 0x5894
} NANDECC , *PNANDECC;


typedef volatile struct _tag_NAND 
{
	volatile U32	NFDATA;		// 0x00
	U32		dummy0[3];
	volatile U8	NFCMD ;		    // 0x10
	U8		dummy1[3];
	U32		dummy2;
	volatile U8	NFADDR;		    // 0x18
} NANDOFFSET, *PNANDOFFSET;


#define	CMD_READ_SPARE						0x50		//	Read2 for Spare
#define	CMD_ERASE							0x60
#define	CMD_ERASE_CONFIRM					0xd0
#define	CMD_READ_ID							0x90
#define	CMD_READ							0x00
#define	CMD_RESET							0xff
#define	CMD_WRITE							0x80
#define	CMD_WRITE_CONFIRM					0x10
#define	CMD_READ_CONFIRM					0x30		// only for Large block
#define	CMD_READ_STATUS						0x70
#define	CMD_RANDOM_DATA_OUTPUT				0x05
#define	CMD_RANDOM_DATA_OUTPUT_CONFIRM		0xe0
#define	CMD_RANDOM_DATA_INPUT				0x85
#define	CMD_READ_FOR_COPY_BACK				0x35
#define	CMD_CACHE_PROGRAM					0x15


#define PNL_3CYCLES				            3
#define PNL_4CYCLES				            4
#define PNL_5CYCLES				            5
	
#define STATUS_READY						0x40		//  Ready
#define STATUS_ERROR						0x01		//  Error



#define	NF_CMD(cmd)				{pNANDOffset->NFCMD	=  (U8)(cmd);}
#define	NF_ADDR(addr)			{pNANDOffset->NFADDR=  (U8)(addr);}
#define	NF_RDDATA()				(*(volatile U8*)(&pNANDOffset->NFDATA))
#define	NF_RDDATA32()			(pNANDOffset->NFDATA)
#define	NF_WRDATA(data)			{*(volatile U8*)(&pNANDOffset->NFDATA)=  (U8)(data);}
#define	NF_WRDATA32(data)		{pNANDOffset->NFDATA=  (data);}
#define CLEAR_RnB()		        pECC->reg_NFCONTROL |= 0x8000
#define CHECK_RnB()		        while(1)					    \
	{															\
		if (pECC->reg_NFCONTROL & 0x8000)						\
		{														\
			CLEAR_RnB();										\
			break;												\
		}														\
	}		


#define	NF_WAITB()				{	NF_CMD( CMD_READ_STATUS );			\
									while (!(NF_RDDATA() & 0x40)); }	
#define NAND_SIZE_UNIT_MB (4095)

#define NAND_SMALL_ADDRESSING	PNL_4CYCLES
#define NAND_LARGE_ADDRESSING	PNL_5CYCLES

	
#define NAND_ADDRESS_TYPE	    PNL_5CYCLES
#define NAND_PAGE_CNT           128		                    // Num of Logical Page
#define NAND_PAGE_SIZE          4096	
#define NAND_SPARE_SIZE         64

#define NAND_BLOCK_SIZE         (NAND_PAGE_CNT * NAND_PAGE_SIZE)
#define NAND_BLOCK_CNT          (U32)((float)NAND_SIZE_UNIT_MB * (1024*1024/NAND_PAGE_CNT/NAND_PAGE_SIZE))

#define NF_SETREAD(nPage, nOffset, nCycles, bChkWait)		    \
    {															\
        NF_CMD	( CMD_READ );									\
	    NF_ADDR	( ((nOffset)     )  & 0xff);					\
		NF_ADDR	( ((nOffset) >> 8)  & 0xff);					\
		NF_ADDR	( ((nPage)       )  & 0xff);		\
	    NF_ADDR	( ((nPage)   >> 8)  & 0xff);		\
		if (nCycles == PNL_5CYCLES)								\
	        NF_ADDR( ((nPage) >> 16) & 0xff);		\
		    NF_CMD	( CMD_READ_CONFIRM );						\
		if (bChkWait==CTRUE)	{ CHECK_RnB() }					\
    }

#define NF_SETREADSPARE_SEQ( Offset )							\
	{                                                           \
	    NF_CMD	( CMD_RANDOM_DATA_OUTPUT );						\
	    NF_ADDR	( ((Offset)       )  & 0xff);					\
        NF_ADDR	( ((Offset)   >> 8)  & 0xff);					\
        NF_CMD	( CMD_RANDOM_DATA_OUTPUT_CONFIRM );				\
	}

#define NF_SETWRITERANDOM(nOffset)						        \
	{															\
	    NF_CMD	( CMD_RANDOM_DATA_INPUT );						\
	    NF_ADDR	( ((nOffset)     )  & 0xff);					\
	    NF_ADDR	( ((nOffset) >> 8)  & 0xff);					\
	}

#define NF_SETWRITE(nPage, nOffset, nCycles)					\
	{															\
        NF_CMD	( CMD_WRITE );									\
        NF_ADDR	( ((nOffset)     )  & 0xff);					\
        NF_ADDR	( ((nOffset) >> 8)  & 0xff);					\
        NF_ADDR	( ((nPage)       )  & 0xff);		\
        NF_ADDR	( ((nPage)   >> 8)  & 0xff);		\
	    if (nCycles == PNL_5CYCLES)								\
	        NF_ADDR( ((nPage) >> 16) & 0xff);		\
    }
#define NF_SETWRITESPARE_SEQ( Offset )							\
	{                                                           \
        NF_CMD	( CMD_RANDOM_DATA_INPUT );                      \
		NF_ADDR	( ((Offset)       )  & 0xff);					\
        NF_ADDR	( ((Offset)   >> 8)  & 0xff);					\
	}
#define NF_SETERASE( nPage, nCycles )							\
    {                                                           \
	    NF_CMD	( CMD_ERASE );									\
		NF_ADDR	( ((nPage)       )  & 0xff );					\
	    NF_ADDR	( ((nPage)   >> 8)  & 0xff );					\
	    if (nCycles == PNL_5CYCLES)								\
		    NF_ADDR( ((nPage) >> 16) & 0x0f );					\
	    NF_CMD	( CMD_ERASE_CONFIRM );							\
	}

typedef struct _SectorInfo {
  unsigned int   dwReserved1;
  unsigned char  bOEMReserved;
  unsigned char  bBadBlock;
  unsigned short wReserved2;
} SectorInfo, *PSectorInfo;


#define START_BLOCK_KERNEL0          1
#define MAX_BLOCK_KERNEL0            16
#define START_BLOCK_KERNEL1          17
#define MAX_BLOCK_KERNEL1            32
#define MAX_GOOD_BLOCKS              6
#define MAX_BAD_BLOCKS               (MAX_BLOCK_KERNEL0 - START_BLOCK_KERNEL0 - MAX_GOOD_BLOCKS )  

#define NAND_MAGIC1    0x47     //'G'
#define NAND_MAGIC2    0x50     //'P'    
#define NAND_MAGIC3    0x48     //'H'    

#define NAND_POS_MAGIC1   5 
#define NAND_POS_MAGIC2   6
#define NAND_POS_MAGIC3   7 


typedef struct _BedCheck {
#if 0    
    unsigned long BedMarkBis;
#else
    unsigned char good_block0[MAX_GOOD_BLOCKS];
    unsigned char good_block1[MAX_GOOD_BLOCKS];
    int all_bed_block0;
    int all_bed_block1;
#endif    
    int bed_cnt0;
    int bed_cnt1;    
}BedCheck;


void *fmd_Init(void);
CBOOL fmd_ReadSector(unsigned long  startSectorAddr,unsigned char *pSectorBuff,    PSectorInfo    pSectorInfoBuff,unsigned int   dwNumSectors); 
CBOOL fmd_EraseBlock(unsigned long page); // block number
CBOOL fmd_WriteSector(	unsigned long  startSectorAddr,	unsigned char *pSectorBuff,	PSectorInfo    pSectorInfoBuff,	unsigned int   dwNumSectors);
void fmd_check_kblocks(void);
CBOOL IsBlockBad(unsigned long blockPage);
CBOOL MarkBlockBad(unsigned int blockID);
void fmd_checkid(void);




#endif // __NAND_CONFIG_H__


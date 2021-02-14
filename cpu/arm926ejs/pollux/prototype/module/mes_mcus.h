//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : MCUS
//	File       : mes_mcus.h
//	Description: 
//	Author     : Firmware Team
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_MCUS_H__
#define __MES_MCUS_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   MCUS MCUS
//------------------------------------------------------------------------------
//@{
         ///	@brief	Static BUS ID
        typedef enum 
        {
        	MES_MCUS_SBUSID_STATIC0		=  0, 	///< Static BUS 0
        	MES_MCUS_SBUSID_STATIC1		=  1, 	///< Static BUS 1
        	MES_MCUS_SBUSID_STATIC2		=  2, 	///< Static BUS 2
        	MES_MCUS_SBUSID_STATIC3		=  3, 	///< Static BUS 3
        	MES_MCUS_SBUSID_STATIC4		=  4, 	///< Static BUS 4
        	MES_MCUS_SBUSID_STATIC5		=  5, 	///< Static BUS 5
        	MES_MCUS_SBUSID_STATIC6		=  6, 	///< Static BUS 6
        	MES_MCUS_SBUSID_STATIC7		=  7, 	///< Static BUS 7
        	MES_MCUS_SBUSID_STATIC8		=  8, 	///< Static BUS 8
        	MES_MCUS_SBUSID_STATIC9		=  9, 	///< Static BUS 9
        	MES_MCUS_SBUSID_NAND		=  11 	///< NAND Flash
        
        } MES_MCUS_SBUSID ;
        
        ///	@brief	wait control
        typedef enum 
        {
        	MES_MCUS_WAITMODE_DISABLE    	= 1, 	///< wait control is disabled
        	MES_MCUS_WAITMODE_ACTIVEHIGH 	= 2, 	///< nSWAIT is active high
        	MES_MCUS_WAITMODE_ACTIVELOW  	= 3 	///< nSWAIT is active low
        
        } MES_MCUS_WAITMODE ;
        
        /// @brief  Burst Mode control
        typedef enum 
        {
        	MES_MCUS_BURSTMODE_DISABLE		= 0,	///< Disable burst access
        	MES_MCUS_BURSTMODE_4BYTEBURST	= 1,	///< 4 byte burst access
        	MES_MCUS_BURSTMODE_8BYTEBURST	= 2,	///< 8 byte burst access
        	MES_MCUS_BURSTMODE_16BYTEBURST	= 3	    ///< 16 byte burst access
        
        } MES_MCUS_BURSTMODE ;
        
        ///	@brief	DMA mode
        typedef enum 
        {
        	MES_MCUS_DMAMODE_MULTIWORDREAD  	= 0, 	///< Multi-word DMA read
        	MES_MCUS_DMAMODE_MULTIWORDWRITE 	= 1 	///< Multi-word DMA write
        
        } MES_MCUS_DMAMODE ;
        
        ///	@brief	signal polarity
        typedef enum 
        {
        	MES_MCUS_POLARITY_ACTIVELOW	    = 0, 	///< active low signal
        	MES_MCUS_POLARITY_ACTIVEHIGH    = 1	    ///< active high signal
        
        } MES_MCUS_POLARITY ;        
 
         /// @brief	Nand Flash address type.
        typedef enum 
        {
        	MES_MCUS_NFTYPE_SBADDR3	= 0, 	///< Small block 3 address type 
        	MES_MCUS_NFTYPE_SBADDR4	= 1, 	///< Small block 4 address type
        	MES_MCUS_NFTYPE_LBADDR4	= 2, 	///< Large block 4 address type
        	MES_MCUS_NFTYPE_LBADDR5	= 3 	///< Large block 5 address type
        
        } MES_MCUS_NFTYPE ;
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_MCUS_Initialize( void );
U32     MES_MCUS_SelectModule( U32 ModuleIndex );
U32     MES_MCUS_GetCurrentModule( void );
U32     MES_MCUS_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_MCUS_GetPhysicalAddress( void );
U32     MES_MCUS_GetSizeOfRegisterSet( void );
void    MES_MCUS_SetBaseAddress( U32 BaseAddress );
U32     MES_MCUS_GetBaseAddress( void );
CBOOL   MES_MCUS_OpenModule( void );
CBOOL   MES_MCUS_CloseModule( void );
CBOOL   MES_MCUS_CheckBusy( void );
CBOOL   MES_MCUS_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_MCUS_GetInterruptNumber( void );
void    MES_MCUS_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_MCUS_GetInterruptEnable( S32 IntNum );
CBOOL   MES_MCUS_GetInterruptPending( S32 IntNum );
void    MES_MCUS_ClearInterruptPending( S32 IntNum );
void    MES_MCUS_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_MCUS_GetInterruptEnableAll( void );
CBOOL   MES_MCUS_GetInterruptPendingAll( void );
void    MES_MCUS_ClearInterruptPendingAll( void );
S32     MES_MCUS_GetInterruptPendingNumber( void );  // -1 if None
//@} 

//------------------------------------------------------------------------------
///  @name   DMA Interface
//@{
U32     MES_MCUS_GetDMAIndex( void );
U32     MES_MCUS_GetDMABusWidth( void );
//@}

//--------------------------------------------------------------------------
///	@name	Static BUS Configuration.
//--------------------------------------------------------------------------
//@{
void MES_MCUS_SetStaticBUSConfig	( MES_MCUS_SBUSID Id, U32 BitWidth,
					  U32 tACS, U32 tCAH, U32 tCOS, U32 tCOH,
					  U32 tACC, U32 tSACC,
					  MES_MCUS_WAITMODE WaitMode,
					  MES_MCUS_BURSTMODE BurstRead,
					  MES_MCUS_BURSTMODE BurstWrite  );

void MES_MCUS_GetStaticBUSConfig	( MES_MCUS_SBUSID Id, U32* pBitWidth,
					  U32* ptACS, U32* ptCAH, U32* ptCOS, U32* ptCOH,
					  U32* ptACC, U32* ptSACC,
					  MES_MCUS_WAITMODE* pWaitMode,
					  MES_MCUS_BURSTMODE* pBurstRead,
					  MES_MCUS_BURSTMODE* pBurstWrite );
//@}

//--------------------------------------------------------------------------
///	@name	NAND Flash Memory operations
//--------------------------------------------------------------------------
//@{
void	        MES_MCUS_SetNFType ( MES_MCUS_NFTYPE type );
MES_MCUS_NFTYPE	MES_MCUS_GetNFType ( void );
void	MES_MCUS_SetNFBootEnable ( CBOOL bEnb );
CBOOL	MES_MCUS_IsNFBootEnable ( void );
void	MES_MCUS_SetNFBank ( U32 Bank );
U32		MES_MCUS_GetNFBank ( void );
CBOOL	MES_MCUS_IsNFReady ( void );
void    MES_MCUS_ResetNFECCBlock( void );
void    MES_MCUS_GetNFECC( U32* pECCH, U32* pECCL );
void    MES_MCUS_SetNFOriginECC( U32 ECCH, U32 ECCL );
void    MES_MCUS_GetNFOriginECC( U32* pECCH, U32* pECCL );
U32     MES_MCUS_GetNFWriteDataCount( void );
U32     MES_MCUS_GetNFReadDataCount( void );
CBOOL   MES_MCUS_IsNFError( void );
CBOOL   MES_MCUS_IsNFReadEnd( void );    
CBOOL   MES_MCUS_IsNFWriteEnd( void );
void    MES_MCUS_GetNFECCDecoderResult( U32* pSyndrome7, U32* pSyndrome5, U32* pSyndrome3, U32* pSyndrome1 );
//@}

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_MCUS_H__

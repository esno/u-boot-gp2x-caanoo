//------------------------------------------------------------------------------
//
//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : DMA
//	File       : mes_dma.h
//	Description:
//	Author     : sunny
//	History    :
//                  2007/04/03  first
//------------------------------------------------------------------------------
#ifndef __MES_DMA_H__
#define __MES_DMA_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   DMA DMA
//------------------------------------------------------------------------------
//@{

    /// @brief Indicate DMA module's maximum transfer size
    enum MES_DMA_TS
    {
        MES_DMA_MAX_TRANSFER_SIZE       = 0x10000UL    ///< Max Transfer Size of DMA Module
    };
    
    
    /// @brief Use for DMA's Operation Mode
    typedef enum
    {
        MES_DMA_OPMODE_MEM_TO_MEM	= 0UL,	        ///< Memory to Memory operation
        MES_DMA_OPMODE_MEM_TO_IO	= 1UL,	        ///< Memory to Peripheral operation
        MES_DMA_OPMODE_IO_TO_MEM	= 2UL	        ///< Peripheral to Memory operation
    
    } MES_DMA_OPMODE ;

//------------------------------------------------------------------------------
/// @name   Module Interface
//@{

CBOOL   MES_DMA_Initialize( void );
U32     MES_DMA_SelectModule( U32 ModuleIndex );
U32     MES_DMA_GetCurrentModule( void );
U32     MES_DMA_GetNumberOfModule( void );
//@}

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{

U32     MES_DMA_GetPhysicalAddress( void );
U32     MES_DMA_GetSizeOfRegisterSet( void );
void    MES_DMA_SetBaseAddress( U32 BaseAddress );
U32     MES_DMA_GetBaseAddress( void );
CBOOL   MES_DMA_OpenModule( void );
CBOOL   MES_DMA_CloseModule( void );
CBOOL   MES_DMA_CheckBusy( void );
CBOOL   MES_DMA_CanPowerDown( void );
//@}

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{

S32     MES_DMA_GetInterruptNumber( void );
void    MES_DMA_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_DMA_GetInterruptEnable( S32 IntNum );
CBOOL   MES_DMA_GetInterruptPending( S32 IntNum );
void    MES_DMA_ClearInterruptPending( S32 IntNum );
void    MES_DMA_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_DMA_GetInterruptEnableAll( void );
CBOOL   MES_DMA_GetInterruptPendingAll( void );
void    MES_DMA_ClearInterruptPendingAll( void );
S32     MES_DMA_GetInterruptPendingNumber( void );  // -1 if None
//@}

//------------------------------------------------------------------------------
///  @name   DMA Configuration Function
//@{
void 	MES_DMA_TransferMemToMem( const void* pSource, void* pDestination, U32 TransferSize );
void 	MES_DMA_TransferMemToIO(  const void* pSource,
                				  U32 DestinationBaseAddress,
                				  U32 DestinationPeriID,
                				  U32 DestinationBitWidth,
                				  U32 TransferSize );
void 	MES_DMA_TransferIOToMem( U32 SourceBaseAddress,
                				  U32 SourcePeriID,
                				  U32 SourceBitWidth,
                				  void* pDestination,
                				  U32 TransferSize );

U32 	MES_DMA_GetMaxTransferSize ( void );
void	MES_DMA_SetSourceAddress( U32 dwAddr );
U32		MES_DMA_GetSourceAddress( void );
void	MES_DMA_SetDestinationAddress( U32 dwAddr );
U32		MES_DMA_GetDestinationAddress( void );
void	MES_DMA_SetDataSize( U32 dwSize );
U32		MES_DMA_GetDataSize( void );
void	MES_DMA_SetIOIndex( U32 dwIndex );
U32		MES_DMA_GetIOIndex( void );
void	MES_DMA_SetAttribute( MES_DMA_OPMODE OpMode, U32 IOBitWidth, CBOOL bSrcMemInc, CBOOL bDstMemInc );
//@}

//------------------------------------------------------------------------------
///  @name   DMA Operation Function
//@{
void    MES_DMA_SetCommandBufferMode( CBOOL bEnb );
CBOOL   MES_DMA_GetCommandBufferMode( void );
void	MES_DMA_Run( void );
CBOOL 	MES_DMA_CheckRunning ( void );
void 	MES_DMA_Stop ( CBOOL Enable );
void    MES_DMA_CommandBufferFlush( void );
CBOOL   MES_DMA_IsCommandBufferFlush( void );
CBOOL   MES_DMA_IsCommandBufferEmpty( void );
CBOOL   MES_DMA_IsCommandBufferReady( void );
U32     MES_DMA_GetCommandBufferFreeCount( void );
//@}

//@}

#ifdef  __cplusplus
}
#endif


#endif // __MES_DMA_H__


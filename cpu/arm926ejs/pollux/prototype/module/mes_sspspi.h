//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : SSPSPI
//	File       : mes_sspspi.h
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------
#ifndef __MES_SSPSPI_H__
#define __MES_SSPSPI_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   SSPSPI SSPSPI
//------------------------------------------------------------------------------
//@{
        /// @brief  Data Transfer Format
        typedef enum
        {
            MES_SSPSPI_FORMAT_A   = 0,    ///< Format A
            MES_SSPSPI_FORMAT_B   = 1     ///< Format B

        }MES_SSPSPI_FORMAT;

        /// @brief  Communication protocol
        typedef enum
        {
            MES_SSPSPI_PROTOCOL_SSP        = 0,   ///< SSP (Synchronous Serial Protocol)
            MES_SSPSPI_PROTOCOL_SPI        = 1    ///< SPI (Serial Peripheral Interface Protocol )

        } MES_SSPSPI_PROTOCOL ;

//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_SSPSPI_Initialize( void );
U32     MES_SSPSPI_SelectModule( U32 ModuleIndex );
U32     MES_SSPSPI_GetCurrentModule( void );
U32     MES_SSPSPI_GetNumberOfModule( void );
//@}

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_SSPSPI_GetPhysicalAddress( void );
U32     MES_SSPSPI_GetSizeOfRegisterSet( void );
void    MES_SSPSPI_SetBaseAddress( U32 BaseAddress );
U32     MES_SSPSPI_GetBaseAddress( void );
CBOOL   MES_SSPSPI_OpenModule( void );
CBOOL   MES_SSPSPI_CloseModule( void );
CBOOL   MES_SSPSPI_CheckBusy( void );
CBOOL   MES_SSPSPI_CanPowerDown( void );
//@}

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_SSPSPI_GetInterruptNumber( void );
void    MES_SSPSPI_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_SSPSPI_GetInterruptEnable( S32 IntNum );
CBOOL   MES_SSPSPI_GetInterruptPending( S32 IntNum );
void    MES_SSPSPI_ClearInterruptPending( S32 IntNum );
void    MES_SSPSPI_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_SSPSPI_GetInterruptEnableAll( void );
CBOOL   MES_SSPSPI_GetInterruptPendingAll( void );
void    MES_SSPSPI_ClearInterruptPendingAll( void );
S32     MES_SSPSPI_GetInterruptPendingNumber( void );  // -1 if None
//@}

//------------------------------------------------------------------------------
///  @name   DMA Interface
//@{
U32     MES_SSPSPI_GetDMAIndex_Tx( void );
U32     MES_SSPSPI_GetDMAIndex_Rx( void );
U32     MES_SSPSPI_GetDMABusWidth( void );
//@}

//------------------------------------------------------------------------------
///  @name   Clock Control Interface
//@{
void            MES_SSPSPI_SetClockPClkMode( MES_PCLKMODE mode );
MES_PCLKMODE    MES_SSPSPI_GetClockPClkMode( void );
void            MES_SSPSPI_SetClockSource( U32 Index, U32 ClkSrc );
U32             MES_SSPSPI_GetClockSource( U32 Index );
void            MES_SSPSPI_SetClockDivisor( U32 Index, U32 Divisor );
U32             MES_SSPSPI_GetClockDivisor( U32 Index );
void            MES_SSPSPI_SetClockDivisorEnable( CBOOL Enable );
CBOOL           MES_SSPSPI_GetClockDivisorEnable( void );
//@}

//--------------------------------------------------------------------------
/// @name Configuration Function
//--------------------------------------------------------------------------
//@{
void    MES_SSPSPI_SetHiZTXD( CBOOL bTXD );
CBOOL   MES_SSPSPI_GetHiZTXD( void );
void    MES_SSPSPI_SetDMATransferMode( CBOOL bDMA );        
CBOOL   MES_SSPSPI_GetDMATransferMode( void );
void    MES_SSPSPI_SetExternalClockSource( CBOOL bExt );
CBOOL   MES_SSPSPI_GetExternalClockSource( void );
void    MES_SSPSPI_SetBitWidth( U32 bitWidth );             
U32     MES_SSPSPI_GetBitWidth( void );
void    MES_SSPSPI_SetDividerCount( U32 dividerCNT );
U32     MES_SSPSPI_GetDividerCount( void );
void    MES_SSPSPI_SetByteSwap( CBOOL bSwap );              
CBOOL   MES_SSPSPI_GetByteSwap( void );
void    MES_SSPSPI_SetSlaveMode( CBOOL bSlave );
CBOOL   MES_SSPSPI_GetSlaveMode( void );
void    MES_SSPSPI_SetClockPolarityInvert( CBOOL bInvert );
CBOOL   MES_SSPSPI_GetClockPolarityInvert( void );

void                MES_SSPSPI_SetSPIFormat( MES_SSPSPI_FORMAT format);
MES_SSPSPI_FORMAT   MES_SSPSPI_GetSPIFormat( void );
void                MES_SSPSPI_SetProtocol( MES_SSPSPI_PROTOCOL protocol );
MES_SSPSPI_PROTOCOL MES_SSPSPI_GetProtocol( void );

//@}

//--------------------------------------------------------------------------
/// @name Operation Function  
//--------------------------------------------------------------------------
//@{
void      MES_SSPSPI_ResetFIFO( void );                         
void      MES_SSPSPI_StartRxBurst( void );
void      MES_SSPSPI_SetRxBurstEnable( CBOOL bEnable );
CBOOL     MES_SSPSPI_GetRxBurstEnable( void );
void      MES_SSPSPI_SetBurstRxSize( U32 size );              
U32       MES_SSPSPI_GetBurstRxSize( void );
void      MES_SSPSPI_SetEnable( CBOOL bEnable );
CBOOL     MES_SSPSPI_GetEnable( void );
U8        MES_SSPSPI_GetByte(void);
U16       MES_SSPSPI_GetHalfWord(void);
void      MES_SSPSPI_PutByte(U8 ByteData);
void      MES_SSPSPI_PutHalfWord(U16 HalfWordData);
//@}


//--------------------------------------------------------------------------
/// @name FIFO State Check Function 
//--------------------------------------------------------------------------
//@{
CBOOL     MES_SSPSPI_IsTxShiftEmpty( void );
CBOOL     MES_SSPSPI_IsTxFIFOFull(void);
CBOOL     MES_SSPSPI_IsTxFIFOEmpty(void);
CBOOL     MES_SSPSPI_IsRxFIFOFull(void);
CBOOL     MES_SSPSPI_IsRxFIFOEmpty(void);
CBOOL     MES_SSPSPI_IsTxRxEnd( void );
//@}

//@}

#ifdef  __cplusplus
}
#endif


#endif // __MES_SSPSPI_H__

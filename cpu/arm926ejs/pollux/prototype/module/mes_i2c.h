//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : I2C
//	File       : mes_i2c.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_I2C_H__
#define __MES_I2C_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   I2C I2C
//------------------------------------------------------------------------------
//@{
 
     /// @brief Select I2C Mode 
    typedef enum 
    {
        MES_I2C_TXRXMODE_SLAVE_RX    = 0,     ///< Slave Receive Mode
        MES_I2C_TXRXMODE_SLAVE_TX    = 1,     ///< Slave Transmit Mode
        MES_I2C_TXRXMODE_MASTER_RX   = 2,     ///< Master Receive Mode
        MES_I2C_TXRXMODE_MASTER_TX   = 3     ///< Master Transmit Mode

    } MES_I2C_TXRXMODE ;

    /// @brief  Start/Stop signal
    typedef enum
    {
        MES_I2C_SIGNAL_STOP  = 0,            ///< Stop signal generation
        MES_I2C_SIGNAL_START = 1             ///< Start signal generation
        
    } MES_I2C_SIGNAL ;
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_I2C_Initialize( void );
U32     MES_I2C_SelectModule( U32 ModuleIndex );
U32     MES_I2C_GetCurrentModule( void );
U32     MES_I2C_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_I2C_GetPhysicalAddress( void );
U32     MES_I2C_GetSizeOfRegisterSet( void );
void    MES_I2C_SetBaseAddress( U32 BaseAddress );
U32     MES_I2C_GetBaseAddress( void );
CBOOL   MES_I2C_OpenModule( void );
CBOOL   MES_I2C_CloseModule( void );
CBOOL   MES_I2C_CheckBusy( void );
CBOOL   MES_I2C_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_I2C_GetInterruptNumber( void );
void    MES_I2C_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_I2C_GetInterruptEnable( S32 IntNum );
CBOOL   MES_I2C_GetInterruptPending( S32 IntNum );
void    MES_I2C_ClearInterruptPending( S32 IntNum );
void    MES_I2C_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_I2C_GetInterruptEnableAll( void );
CBOOL   MES_I2C_GetInterruptPendingAll( void );
void    MES_I2C_ClearInterruptPendingAll( void );
S32     MES_I2C_GetInterruptPendingNumber( void );  // -1 if None
//@} 

//------------------------------------------------------------------------------
///  @name   Clock Control Interface
//@{
void            MES_I2C_SetClockPClkMode( MES_PCLKMODE mode );
MES_PCLKMODE    MES_I2C_GetClockPClkMode( void );

//@}

//--------------------------------------------------------------------------
/// @name Configuration Function
//--------------------------------------------------------------------------
//@{
void    MES_I2C_SetClockPrescaler( U32 PclkDivider, U32 Prescaler );
void    MES_I2C_GetClockPrescaler( U32* pPclkDivider, U32* pPrescaler );
void    MES_I2C_SetExtendedIRQEnable( CBOOL bEnable );
CBOOL   MES_I2C_GetExtendedIRQEnable( void );
void    MES_I2C_SetSlaveAddress(U8 Address);
U8      MES_I2C_GetSlaveAddress(void);
void    MES_I2C_SetDataDelay( U32 delay );
U32     MES_I2C_GetDataDelay (void);
//@}

//------------------------------------------------------------------------------
/// @name Operation Function
//------------------------------------------------------------------------------
//@{
void        MES_I2C_SetAckGenerationEnable( CBOOL bAckGen );
CBOOL       MES_I2C_GetAckGenerationEnable( void );
void        MES_I2C_ClearOperationHold  ( void );
void        MES_I2C_ControlMode ( MES_I2C_TXRXMODE TxRxMode, MES_I2C_SIGNAL Signal );
CBOOL       MES_I2C_IsBusy( void );
void        MES_I2C_WriteByte (U8 WriteData);
U8          MES_I2C_ReadByte (void);
void        MES_I2C_BusDisable( void );
//@}

//------------------------------------------------------------------------------
/// @name Checking Function ( extra Interrupt source )
//------------------------------------------------------------------------------
//@{    
CBOOL       MES_I2C_IsSlaveAddressMatch( void );
void        MES_I2C_ClearSlaveAddressMatch( void );    
CBOOL       MES_I2C_IsGeneralCall( void );
void        MES_I2C_ClearGeneralCall( void );
CBOOL       MES_I2C_IsSlaveRxStop( void );
void        MES_I2C_ClearSlaveRxStop( void );
CBOOL       MES_I2C_IsBusArbitFail( void );
CBOOL       MES_I2C_IsACKReceived( void );
//@}

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_I2C_H__

//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : UART
//	File       : mes_uart.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_UART_H__
#define __MES_UART_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   UART    UART
//------------------------------------------------------------------------------
//@{

    /// @brief Parity generation/check type
    typedef enum 
    {
    	MES_UART_PARITYMODE_NONE 	= 0,	///< No parity
    	MES_UART_PARITYMODE_ODD 	= 4,	///< Odd parity
    	MES_UART_PARITYMODE_EVEN 	= 5,	///< Even parity
    	MES_UART_PARITYMODE_FORCED1	= 6,	///< Forced/checked as 1
    	MES_UART_PARITYMODE_FORCED0 = 7 	///< Forced/checked as 0
    
    } MES_UART_PARITYMODE ;

    /// @brief Tx/Rx IRQ Type
    typedef enum 
    {
    	MES_UART_IRQTYPE_PULSE		= 0,	///< Pulse 
    	MES_UART_IRQTYPE_LEVEL		= 1	    ///< Level 

    } MES_UART_IRQTYPE; 

    /// @brief Tx/Rx Operation Mode
    typedef enum 
    {
    	MES_UART_OPERMODE_DISABLE 	= 0,	///< Disable
    	MES_UART_OPERMODE_NORMAL 	= 1,	///< Interrupt or Polling
    	MES_UART_OPERMODE_DMA 		= 2 	///< DMA

    } MES_UART_OPERMODE  ;

        /// @brief Smartmedia Card Operation Mode
        typedef enum 
        {
            MES_UART_SCMODE_DISABLE  =   0,      ///< Smart Card Tx/Rx Disable
            MES_UART_SCMODE_TX       =   1,      ///< Smart Card Tx Enable
            MES_UART_SCMODE_RX       =   2       ///< Smart Card Rx Enable

        } MES_UART_SCMODE ;

        ///	@brief Tx/Rx Status
        typedef enum 
        {
        	MES_UART_RX_BUFFER_READY 	= 0x01,	        ///< Rx buffer contains valid data
        	MES_UART_TX_BUFFER_EMPTY	= 0x02,	        ///< Tx buffer is empty
        	MES_UART_TRANSMITTER_EMPTY	= 0x04          ///< Tx buffer and the transmit shifter are empty

        } MES_UART_TXRXSTATUS ;
        
        /// @brief Error Status
        typedef enum 
        {
            MES_UART_ERRSTAT_OVERRUN     = 0x01,         ///< Overrun Error 
            MES_UART_ERRSTAT_PARITY      = 0x02,         ///< Parity Error
            MES_UART_ERRSTAT_FRAME       = 0x04,         ///< Frame Error
            MES_UART_ERRSTAT_BREAK       = 0x08          ///< Break Signal receive

        } MES_UART_ERRSTAT ;
        
        /// @brief FIFO Status
        typedef enum 
        {
            MES_UART_FIFOSTAT_RXFULL     = 0x01,         ///< RX FIFO Full
            MES_UART_FIFOSTAT_TXFULL     = 0x02,         ///< RX FIFO Full
            MES_UART_FIFOSTAT_RXERR      = 0x04          ///< RX FIFO Error

        } MES_UART_FIFOSTAT ;
        
        
        /// @brief Modem Status
        typedef enum
        {
            MES_UART_MODEMSTAT_DCD_CHANGED  = 1 << 7,    ///< Modem Status DCD Pin Changed
            MES_UART_MODEMSTAT_RI_CHANGED   = 1 << 6,    ///< Modem Status RI Pin Changed
            MES_UART_MODEMSTAT_DSR_CHANGED  = 1 << 5,    ///< Modem Status DSR Pin Changed
            MES_UART_MODEMSTAT_CTS_CHANGED  = 1 << 4,    ///< Modem Status CTS Pin Changed
            MES_UART_MODEMSTAT_DCD_LOW      = 1 << 3,    ///< Modem DCD Pin High 
            MES_UART_MODEMSTAT_RI_LOW       = 1 << 2,    ///< Modem RI Pin  Low 
            MES_UART_MODEMSTAT_DSR_LOW      = 1 << 1,    ///< Modem DSR Pin Low 
            MES_UART_MODEMSTAT_CTS_ACTIVE   = 1 << 0     ///< Modem CTS Pin Active 
           
        } MES_UART_MODEMSTAT ;
        
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_UART_Initialize( void );
U32     MES_UART_SelectModule( U32 ModuleIndex );
U32     MES_UART_GetCurrentModule( void );
U32     MES_UART_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_UART_GetPhysicalAddress( void );
U32     MES_UART_GetSizeOfRegisterSet( void );
void    MES_UART_SetBaseAddress( U32 BaseAddress );
U32     MES_UART_GetBaseAddress( void );
CBOOL   MES_UART_OpenModule( void );
CBOOL   MES_UART_CloseModule( void );
CBOOL   MES_UART_CheckBusy( void );
CBOOL   MES_UART_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_UART_GetInterruptNumber( void );
void    MES_UART_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_UART_GetInterruptEnable( S32 IntNum );
CBOOL   MES_UART_GetInterruptPending( S32 IntNum );
void    MES_UART_ClearInterruptPending( S32 IntNum );
void    MES_UART_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_UART_GetInterruptEnableAll( void );
CBOOL   MES_UART_GetInterruptPendingAll( void );
void    MES_UART_ClearInterruptPendingAll( void );
S32     MES_UART_GetInterruptPendingNumber( void );  // -1 if None
//@} 

//------------------------------------------------------------------------------
///  @name   DMA Interface
//@{
U32     MES_UART_GetDMAIndex_Tx( void );
U32     MES_UART_GetDMAIndex_Rx( void );
U32     MES_UART_GetDMABusWidth( void );
//@}

//------------------------------------------------------------------------------
///  @name   Clock Control Interface
//@{

void            MES_UART_SetClockPClkMode( MES_PCLKMODE mode );
MES_PCLKMODE    MES_UART_GetClockPClkMode( void );
void            MES_UART_SetClockSource( U32 Index, U32 ClkSrc );
U32             MES_UART_GetClockSource( U32 Index );
void            MES_UART_SetClockDivisor( U32 Index, U32 Divisor );
U32             MES_UART_GetClockDivisor( U32 Index );
void            MES_UART_SetClockDivisorEnable( CBOOL Enable );
CBOOL           MES_UART_GetClockDivisorEnable( void );

//@}

//--------------------------------------------------------------------------
/// @name	Configuration operations
//--------------------------------------------------------------------------
//@{
void	MES_UART_SetSIRMode( CBOOL bEnb );
CBOOL   MES_UART_GetSIRMode( void );
void	MES_UART_SetLoopBackMode( CBOOL bEnb );
CBOOL   MES_UART_GetLoopBackMode( void );
void    MES_UART_SetIntEnbWhenExceptionOccur( CBOOL bEnb );
CBOOL   MES_UART_GetIntEnbWhenExceptionOccur( void );
void	MES_UART_SetFrameConfiguration( MES_UART_PARITYMODE Parity,  U32 DataWidth,  U32 StopBit );
void    MES_UART_GetFrameConfiguration( MES_UART_PARITYMODE* pParity, U32* pDataWidth, U32* pStopBit );
void	MES_UART_SetBRD( U16 BRD );
U16     MES_UART_GetBRD( void );
U16		MES_UART_MakeBRD( U32 BaudRate, U32 CLKinHz );
void    MES_UART_SetRxTimeOutEnb( CBOOL bEnb );
CBOOL   MES_UART_GetRxTimeOutEnb( void );
void	MES_UART_SetRxTimeOut( U16 TimeOut );
U16     MES_UART_GetRxTimeOut( void );
void    MES_UART_SetSYNCPendClear( void );
void	            MES_UART_SetTxIRQType( MES_UART_IRQTYPE IrqType );
MES_UART_IRQTYPE	MES_UART_GetTxIRQType( void );
void	            MES_UART_SetRxIRQType( MES_UART_IRQTYPE IrqType );
MES_UART_IRQTYPE	MES_UART_GetRxIRQType( void );	
void	MES_UART_SendBreak( void );
void        	    MES_UART_SetTxOperationMode( MES_UART_OPERMODE mode );
MES_UART_OPERMODE	MES_UART_GetTxOperationMode( void );
void 	            MES_UART_SetRxOperationMode( MES_UART_OPERMODE mode );
MES_UART_OPERMODE	MES_UART_GetRxOperationMode( void );
//@}

//--------------------------------------------------------------------------
/// @name	FIFO control operations
//--------------------------------------------------------------------------
//@{
void	MES_UART_SetTxFIFOTriggerLevel( U32 level );
U32     MES_UART_GetTxFIFOTriggerLevel( void );
void	MES_UART_SetRxFIFOTriggerLevel( U32 level );
U32     MES_UART_GetRxFIFOTriggerLevel( void );
void	MES_UART_ResetTxFIFO( void );
CBOOL   MES_UART_IsTxFIFOReset( void );
void	MES_UART_ResetRxFIFO( void );
CBOOL   MES_UART_IsRxFIFOReset( void );    
void	MES_UART_SetFIFOEnb( CBOOL bEnb );
CBOOL   MES_UART_GetFIFOEnb( void );
//@}
	
//--------------------------------------------------------------------------
/// @name	Smartmedia card control operations
//--------------------------------------------------------------------------
//@{
void	MES_UART_SetSCTxEnb( CBOOL bEnb );
CBOOL   MES_UART_GetSCTxEnb( void );
void	MES_UART_SetSCRxEnb( CBOOL bEnb );
CBOOL	MES_UART_GetSCRxEnb( void );
void	MES_UART_SetSCTxMode( void );
void	MES_UART_SetSCRxMode( void );
MES_UART_SCMODE  MES_UART_GetSCMode( void );
//@}

//--------------------------------------------------------------------------
/// @name	Status operations
//--------------------------------------------------------------------------
//@{
U32		MES_UART_GetTxRxStatus( void );
U32		MES_UART_GetErrorStatus( void );
U32     MES_UART_GetFIFOStatus( void );
U32		MES_UART_GetTxFIFOCount( void );
U32		MES_UART_GetRxFIFOCount( void );
//@}

//--------------------------------------------------------------------------
/// @name	Modem operations
//--------------------------------------------------------------------------
//@{
void	MES_UART_SetDTR( CBOOL bActive );
CBOOL   MES_UART_GetDTR( void );
void	MES_UART_SetRTS( CBOOL bActive );
CBOOL	MES_UART_GetRTS( void );	
void    MES_UART_SetAutoFlowControl( CBOOL enable );
CBOOL   MES_UART_GetAutoFlowControl( void );
void    MES_UART_SetHalfChannelEnable( CBOOL enable );
CBOOL   MES_UART_GetHalfChannelEnable( void );
U32 	MES_UART_GetModemStatus( void );


//@}

//--------------------------------------------------------------------------
/// @name	Basic operations
//--------------------------------------------------------------------------
//@{
void	MES_UART_SendByte( U8 Data );
U8      MES_UART_GetByte( void );
//@}

//--------------------------------------------------------------------------
/// @name   UART Configuration Function
//--------------------------------------------------------------------------
//@{
void    MES_UART_SetLineConfig( CBOOL sirMode, MES_UART_PARITYMODE Parity, U32 DataWidth, U32 StopBit );
void    MES_UART_SetControlConfig( CBOOL syncPendClear, MES_UART_IRQTYPE txType, MES_UART_IRQTYPE rxType, MES_UART_OPERMODE txOper, MES_UART_OPERMODE rxOper );
void    MES_UART_SetFIFOConfig( CBOOL fifoEnb, CBOOL txFIFOReset, CBOOL rxFIFOReset, U32 txLevel, U32 rxLevel );
void    MES_UART_SetBaudRateConfig( U16 baudRate, U16 timeOut );
void    MES_UART_SetModemConfig( CBOOL autoflow, CBOOL halfChannel, CBOOL SCTxEnb, CBOOL SCRxEnb, CBOOL DTR, CBOOL RTS );

void    MES_UART_GetLineConfig( U32* pSirMode, MES_UART_PARITYMODE* pParity, U32* pDataWidth, U32* pStopBit );
void    MES_UART_GetControlConfig( MES_UART_IRQTYPE* pTxType, MES_UART_IRQTYPE* pRxType, MES_UART_OPERMODE* pTxOper, MES_UART_OPERMODE* pRxOper );
void    MES_UART_GetFIFOConfig( U32* pFIFOEnb, U32* pTxFIFOReset, U32* pRxFIFOReset, U32* pTxLevel, U32* pRxLevel );
void    MES_UART_GetBaudRateConfig( U16* pBaudRate, U16* pTimeOut );
void    MES_UART_GetModemConfig( U32* pAutoflow, U32* pHalfChannel, U32* pSCTxEnb, U32* pSCRxEnb, U32* pDTR, U32* pRTS );
//@}

//--------------------------------------------------------------------------
/// @name   Register Backup & Store Function
//--------------------------------------------------------------------------
//@{
void    MES_UART_SetLCON( U16 value );		
void    MES_UART_SetUCON( U16 value );		
void    MES_UART_SetFCON( U16 value );       
void    MES_UART_SetMCON( U16 value );       
void    MES_UART_SetTIMEOUT( U16 value );	
void    MES_UART_SetINTCON( U16 value );		

U16     MES_UART_GetLCON( void );
U16     MES_UART_GetUCON( void );
U16     MES_UART_GetFCON( void );
U16     MES_UART_GetMCON( void );
U16     MES_UART_GetTIMEOUT( void );
U16     MES_UART_GetINTCON( void );
//@}


//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_UART_H__

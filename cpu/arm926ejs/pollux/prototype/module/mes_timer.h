//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : TIMER
//	File       : mes_timer.h
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------
#ifndef __MES_TIMER_H__
#define __MES_TIMER_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   TIMER TIMER
//------------------------------------------------------------------------------
//@{

    /// @brief	timer clock source
    typedef enum 
    {
        MES_TIMER_CLOCK_TCLK  = 3,	///< TCLK = Source clock
        MES_TIMER_CLOCK_TCLK2 = 0,	///< TCLK = Source clock / 2
        MES_TIMER_CLOCK_TCLK4 = 1,	///< TCLK = Source clock / 4
        MES_TIMER_CLOCK_TCLK8 = 2	///< TCLK = Source clock / 8

    } MES_TIMER_CLOCK;

//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_TIMER_Initialize( void );
U32     MES_TIMER_SelectModule( U32 ModuleIndex );
U32     MES_TIMER_GetCurrentModule( void );
U32     MES_TIMER_GetNumberOfModule( void );
//@}

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_TIMER_GetPhysicalAddress( void );
U32     MES_TIMER_GetSizeOfRegisterSet( void );
void    MES_TIMER_SetBaseAddress( U32 BaseAddress );
U32     MES_TIMER_GetBaseAddress( void );
CBOOL   MES_TIMER_OpenModule( void );
CBOOL   MES_TIMER_CloseModule( void );
CBOOL   MES_TIMER_CheckBusy( void );
CBOOL   MES_TIMER_CanPowerDown( void );
//@}

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_TIMER_GetInterruptNumber( void );
void    MES_TIMER_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_TIMER_GetInterruptEnable( S32 IntNum );
CBOOL   MES_TIMER_GetInterruptPending( S32 IntNum );
void    MES_TIMER_ClearInterruptPending( S32 IntNum );
void    MES_TIMER_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_TIMER_GetInterruptEnableAll( void );
CBOOL   MES_TIMER_GetInterruptPendingAll( void );
void    MES_TIMER_ClearInterruptPendingAll( void );
S32     MES_TIMER_GetInterruptPendingNumber( void );  // -1 if None
//@}

//------------------------------------------------------------------------------
///  @name   Clock Control Interface
//@{
void            MES_TIMER_SetClockPClkMode( MES_PCLKMODE mode );
MES_PCLKMODE    MES_TIMER_GetClockPClkMode( void );
void            MES_TIMER_SetClockDivisorEnable( CBOOL Enable );
CBOOL           MES_TIMER_GetClockDivisorEnable( void );

void            MES_TIMER_SetClockSource( U32 Index, U32 ClkSrc );
U32             MES_TIMER_GetClockSource( U32 Index );
void            MES_TIMER_SetClockDivisor( U32 Index, U32 Divisor );
U32             MES_TIMER_GetClockDivisor( U32 Index );
//@}

//------------------------------------------------------------------------------
///  @name  TIMER Operation.
//@{
void MES_TIMER_Run( void );
void MES_TIMER_Stop( void );
void MES_TIMER_SetWatchDogEnable( CBOOL Enable );

void MES_TIMER_SetTClkDivider( MES_TIMER_CLOCK Clock );

void MES_TIMER_SetMatchCounter( U32 MatchCounter );
U32  MES_TIMER_GetMatchCounter( void );
void MES_TIMER_SetTimerCounter( U32 TimerCounter );
U32  MES_TIMER_GetTimerCounter( void );
//@}

//@}

#ifdef  __cplusplus
}
#endif


#endif // __MES_TIMER_H__

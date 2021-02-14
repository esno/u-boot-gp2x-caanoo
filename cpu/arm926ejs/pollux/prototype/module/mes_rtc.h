//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : RTC
//	File       : mes_rtc.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_RTC_H__
#define __MES_RTC_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   RTC RTC
//------------------------------------------------------------------------------
//@{
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_RTC_Initialize( void );
U32     MES_RTC_SelectModule( U32 ModuleIndex );
U32     MES_RTC_GetCurrentModule( void );
U32     MES_RTC_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_RTC_GetPhysicalAddress( void );
U32     MES_RTC_GetSizeOfRegisterSet( void );
void    MES_RTC_SetBaseAddress( U32 BaseAddress );
U32     MES_RTC_GetBaseAddress( void );
CBOOL   MES_RTC_OpenModule( void );
CBOOL   MES_RTC_CloseModule( void );
CBOOL   MES_RTC_CheckBusy( void );
CBOOL   MES_RTC_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_RTC_GetInterruptNumber( void );
void    MES_RTC_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_RTC_GetInterruptEnable( S32 IntNum );
CBOOL   MES_RTC_GetInterruptPending( S32 IntNum );
void    MES_RTC_ClearInterruptPending( S32 IntNum );
void    MES_RTC_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_RTC_GetInterruptEnableAll( void );
CBOOL   MES_RTC_GetInterruptPendingAll( void );
void    MES_RTC_ClearInterruptPendingAll( void );
S32     MES_RTC_GetInterruptPendingNumber( void );  // -1 if None
//@} 

//------------------------------------------------------------------------------
///  @name  RTC Operation.
//@{
void  	MES_RTC_SetAlarmCounter( U32 AlarmCounter );
U32   	MES_RTC_GetAlarmCounter( void );
CBOOL 	MES_RTC_IsBusyAlarmCounter( void );
void 	MES_RTC_SetRTCCounter( U32 RTCCounter );
U32  	MES_RTC_GetRTCCounter( void );
CBOOL 	MES_RTC_IsBusyRTCCounter( void );
void    MES_RTC_SetRTCCounterWriteEnable( CBOOL enable );
//@} 

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_xxx_H__

//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : ALIVE
//	File       : mes_alive.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_ALIVE_H__
#define __MES_ALIVE_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   ALIVE   ALIVE
//------------------------------------------------------------------------------
//@{
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_ALIVE_Initialize( void );
U32     MES_ALIVE_SelectModule( U32 ModuleIndex );
U32     MES_ALIVE_GetCurrentModule( void );
U32     MES_ALIVE_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_ALIVE_GetPhysicalAddress( void );
U32     MES_ALIVE_GetSizeOfRegisterSet( void );
void    MES_ALIVE_SetBaseAddress( U32 BaseAddress );
U32     MES_ALIVE_GetBaseAddress( void );
CBOOL   MES_ALIVE_OpenModule( void );
CBOOL   MES_ALIVE_CloseModule( void );
CBOOL   MES_ALIVE_CheckBusy( void );
CBOOL   MES_ALIVE_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name  Alive Operation.
//@{
void    MES_ALIVE_SetWriteEnable( CBOOL Enable );
CBOOL   MES_ALIVE_GetWriteEnable( void );
void    MES_ALIVE_SetVDDPower( CBOOL Enable );
CBOOL   MES_ALIVE_GetVDDPower( void );
void    MES_ALIVE_SetAliveGpio( U32 GpioNum, CBOOL Enable );
CBOOL   MES_ALIVE_GetAliveGpio( U32 GpioNum );
void    MES_ALIVE_SetScratchReg( U32 Data );
U32     MES_ALIVE_GetScratchReg( void );
CBOOL   MES_ALIVE_GetVddPowerToggle( void );
//@} 

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_GPIOALIVE_H__

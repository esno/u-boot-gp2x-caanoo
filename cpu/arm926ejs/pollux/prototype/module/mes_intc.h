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
//	Module     : Interrupt Controller
//	File       : mes_intc.h
//	Description: 
//	Author     : sunny
//	History    : 
//                  2007/04/03  first 
//------------------------------------------------------------------------------
#ifndef __MES_INTC_H__
#define __MES_INTC_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   INTC INTERRUPT
//------------------------------------------------------------------------------
//@{

 	enum MES_INTC_NOI { MES_INTC_NUM_OF_INT = 44UL };
	
	/// @brief	interrupt mode.
	typedef enum 
	{
		MES_INTC_INTMODE_IRQ = 0UL,	    ///< IRQ mode
		MES_INTC_INTMODE_FIQ = 1UL		///< FIQ mode

	}MES_INTC_INTMODE ; 
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{

CBOOL   MES_INTC_Initialize( void );
U32     MES_INTC_SelectModule( U32 ModuleIndex );
U32     MES_INTC_GetCurrentModule( void );
U32     MES_INTC_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{

U32     MES_INTC_GetPhysicalAddress( void );
U32     MES_INTC_GetSizeOfRegisterSet( void );
void    MES_INTC_SetBaseAddress( U32 BaseAddress );
U32     MES_INTC_GetBaseAddress( void );
CBOOL   MES_INTC_OpenModule( void );
CBOOL   MES_INTC_CloseModule( void );
CBOOL   MES_INTC_CheckBusy( void );
CBOOL   MES_INTC_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{

void    MES_INTC_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_INTC_GetInterruptEnable( S32 IntNum );
CBOOL   MES_INTC_GetInterruptPending( S32 IntNum );
void    MES_INTC_ClearInterruptPending( S32 IntNum );
void    MES_INTC_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_INTC_GetInterruptEnableAll( void );
CBOOL   MES_INTC_GetInterruptPendingAll( void );
void    MES_INTC_ClearInterruptPendingAll( void );
S32     MES_INTC_GetInterruptPendingNumber( void );  // -1 if None
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Controller Operation.
//------------------------------------------------------------------------------
//@{ 
void 	MES_INTC_SetInterruptMode ( U32 IntNum, MES_INTC_INTMODE IntMode  );
void	MES_INTC_SetPriorityMode( U32 ArbiterNum, U32 OrderSel );
//@} 

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_INTC_H__


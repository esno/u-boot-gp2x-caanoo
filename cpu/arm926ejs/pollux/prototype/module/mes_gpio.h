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
//	Module     : GPIO
//	File       : mes_gpio.h
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------
#ifndef __MES_GPIO_H__
#define __MES_GPIO_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   GPIO GPIO
//------------------------------------------------------------------------------
//@{

	/// @brief	GPIO interrupt mode
	typedef enum
	{
		MES_GPIO_INTMODE_LOWLEVEL    = 0UL,	    ///< Low level detect
		MES_GPIO_INTMODE_HIGHLEVEL   = 1UL,	    ///< High level detect
		MES_GPIO_INTMODE_FALLINGEDGE = 2UL,	    ///< Falling edge detect
		MES_GPIO_INTMODE_RISINGEDGE  = 3UL     ///< Rising edge detect

	}MES_GPIO_INTMODE;

	/// @brief	I/O mode
	typedef enum
	{
		MES_GPIO_PADFUNC_GPIO    = 0UL,	        ///< GPIO mode
		MES_GPIO_PADFUNC_1 		 = 1UL,	        ///< Alternate function 1
		MES_GPIO_PADFUNC_2 		 = 2UL,	        ///< Alternate function 2

	}MES_GPIO_PADFUNC ;

    enum MES_GPIO_NOI { MES_GPIO_NUM_OF_INT = 32UL };
  	enum MES_GPIO_MB  { MES_GPIO_MAX_BIT = 32UL };

//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_GPIO_Initialize( void );
U32     MES_GPIO_SelectModule( U32 ModuleIndex );
U32     MES_GPIO_GetCurrentModule( void );
U32     MES_GPIO_GetNumberOfModule( void );
//@}

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_GPIO_GetPhysicalAddress( void );
U32     MES_GPIO_GetSizeOfRegisterSet( void );
void    MES_GPIO_SetBaseAddress( U32 BaseAddress );
U32     MES_GPIO_GetBaseAddress( void );
CBOOL   MES_GPIO_OpenModule( void );
CBOOL   MES_GPIO_CloseModule( void );
CBOOL   MES_GPIO_CheckBusy( void );
CBOOL   MES_GPIO_CanPowerDown( void );
//@}

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_GPIO_GetInterruptNumber( void );
void    MES_GPIO_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_GPIO_GetInterruptEnable( S32 IntNum );
CBOOL   MES_GPIO_GetInterruptPending( S32 IntNum );
void    MES_GPIO_ClearInterruptPending( S32 IntNum );
void    MES_GPIO_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_GPIO_GetInterruptEnableAll( void );
CBOOL   MES_GPIO_GetInterruptPendingAll( void );
void    MES_GPIO_ClearInterruptPendingAll( void );
S32     MES_GPIO_GetInterruptPendingNumber( void );  // -1 if None
//@}

//------------------------------------------------------------------------------
///  @name   GPIO Operation.
//@{
void	            MES_GPIO_SetInterruptMode( U32 BitNumber, MES_GPIO_INTMODE IntMode );
MES_GPIO_INTMODE	MES_GPIO_GetInterruptMode( U32 BitNumber );
void	            MES_GPIO_SetOutputEnable ( U32 BitNumber, CBOOL OutputEnb );
CBOOL	            MES_GPIO_GetOutputEnable ( U32 BitNumber );
void	            MES_GPIO_SetOutputValue  ( U32 BitNumber, CBOOL Value );
CBOOL	            MES_GPIO_GetOutputValue  ( U32 BitNumber );
CBOOL	            MES_GPIO_GetInputValue 	 ( U32 BitNumber );
void 	            MES_GPIO_SetPullUpEnable ( U32 BitNumber, CBOOL enable);
CBOOL               MES_GPIO_GetPullUpEnable ( U32 BitNumber );

void 	            MES_GPIO_SetPadFunction( U32 BitNumber, MES_GPIO_PADFUNC padfunc );
MES_GPIO_PADFUNC    MES_GPIO_GetPadFunction( U32 BitNumber );

U32                 MES_GPIO_GetValidBit( void );
//@}

//@}

#ifdef  __cplusplus
}
#endif

#endif //__MES_GPIO_H__

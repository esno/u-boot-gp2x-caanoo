//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : CSC
//	File       : mes_csc.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_CSC_H__
#define __MES_CSC_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   CSC CSC
//------------------------------------------------------------------------------
//@{

    /// @brief  Quantization mode
	typedef enum 
	{		
		MES_CSC_QMODE_220 = 0,	///< 220 quantization mode
		MES_CSC_QMODE_256 = 1	///< 256 quantization mode

	} MES_CSC_QMODE ;
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_CSC_Initialize( void );
U32     MES_CSC_SelectModule( U32 ModuleIndex );
U32     MES_CSC_GetCurrentModule( void );
U32     MES_CSC_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_CSC_GetPhysicalAddress( void );
U32     MES_CSC_GetSizeOfRegisterSet( void );
void    MES_CSC_SetBaseAddress( U32 BaseAddress );
U32     MES_CSC_GetBaseAddress( void );
CBOOL   MES_CSC_OpenModule( void );
CBOOL   MES_CSC_CloseModule( void );
CBOOL   MES_CSC_CheckBusy( void );
CBOOL   MES_CSC_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Interrupt Interface
//@{
S32     MES_CSC_GetInterruptNumber( void );
void    MES_CSC_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL   MES_CSC_GetInterruptEnable( S32 IntNum );
CBOOL   MES_CSC_GetInterruptPending( S32 IntNum );
void    MES_CSC_ClearInterruptPending( S32 IntNum );
void    MES_CSC_SetInterruptEnableAll( CBOOL Enable );
CBOOL   MES_CSC_GetInterruptEnableAll( void );
CBOOL   MES_CSC_GetInterruptPendingAll( void );
void    MES_CSC_ClearInterruptPendingAll( void );
S32     MES_CSC_GetInterruptPendingNumber( void );  // -1 if None
//@} 

//------------------------------------------------------------------------------
///  @name   Clock Control Interface
//@{

void            MES_CSC_SetClockPClkMode( MES_PCLKMODE mode );
MES_PCLKMODE    MES_CSC_GetClockPClkMode( void );
void            MES_CSC_SetClockBClkMode( MES_BCLKMODE mode );
MES_BCLKMODE    MES_CSC_GetClockBClkMode( void );

//@}

//------------------------------------------------------------------------------
///  @name  CSC Operation.
//@{
void	        MES_CSC_SetDitherEnable( CBOOL bEnb );
CBOOL	        MES_CSC_GetDitherEnable( void );
void	        MES_CSC_SetQuantizationMode( MES_CSC_QMODE mode );
MES_CSC_QMODE	MES_CSC_GetQuantizationMode( void );
void	        MES_CSC_SetTransparency( U16 ReplaceColor, CBOOL bEnb,
				                		 CBOOL bInvY,  U8 MinY,  U8 MaxY,
				                		 CBOOL bInvCb, U8 MinCb, U8 MaxCb,
				                		 CBOOL bInvCr, U8 MinCr, U8 MaxCr );
void	MES_CSC_SetSize( U32 width, U32 height );
void	MES_CSC_SetSourceAddr( U32 AddrY, U32 AddrCb, U32 AddrCr );
void	MES_CSC_SetTargetAddr( U32 AddrTex );
void	MES_CSC_Run( void );
CBOOL	MES_CSC_IsBusy( void );
//@} 

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_CSC_H__

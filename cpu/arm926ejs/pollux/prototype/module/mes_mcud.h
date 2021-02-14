//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : MCUD
//	File       : mes_mcud.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_MCUD_H__
#define __MES_MCUD_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   MCUD MCUD
//------------------------------------------------------------------------------
//@{
 
	/// @brief	DRAM Type
	typedef enum 
	{
		MES_MCUD_SDRTYPE_DDR = 1	///< Normal DDR-SDRAM
	} MES_MCUD_SDRTYPE ;
	
	/// @brief	Latency
	typedef enum 
	{
		MES_MCUD_LATENCY_2_0 = 1,	///< 2   cycle
		MES_MCUD_LATENCY_2_5 = 2,	///< 2.5 cycle
		MES_MCUD_LATENCY_3_0 = 3 	///< 3   cycle
	} MES_MCUD_LATENCY ;
	
	/// @brief	Output driver impedance control ( Normal EMRS only )
	typedef enum 
	{
		MES_MCUD_DIC_NORMAL = 0,	///< Normal driving
		MES_MCUD_DIC_WEAK   = 1		///< Weak driving
	} MES_MCUD_DIC ; 
	
	/// @brief	Delay in nano seconds
	typedef enum 
	{
		MES_MCUD_DELAY_0_0	= 0,	///< No delay
		MES_MCUD_DELAY_0_5	= 1,	///< 0.5 ns delay
		MES_MCUD_DELAY_1_0	= 2,	///< 1.0 ns delay
		MES_MCUD_DELAY_1_5	= 3,	///< 1.5 ns delay
		MES_MCUD_DELAY_2_0	= 4,	///< 2.0 ns delay
		MES_MCUD_DELAY_2_5	= 5,	///< 2.5 ns delay
		MES_MCUD_DELAY_3_0	= 6,	///< 3.0 ns delay
		MES_MCUD_DELAY_3_5	= 7		///< 3.5 ns delay
	} MES_MCUD_DELAY ;
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_MCUD_Initialize( void );
U32     MES_MCUD_SelectModule( U32 ModuleIndex );
U32     MES_MCUD_GetCurrentModule( void );
U32     MES_MCUD_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_MCUD_GetPhysicalAddress( void );
U32     MES_MCUD_GetSizeOfRegisterSet( void );
void    MES_MCUD_SetBaseAddress( U32 BaseAddress );
U32     MES_MCUD_GetBaseAddress( void );
CBOOL   MES_MCUD_OpenModule( void );
CBOOL   MES_MCUD_CloseModule( void );
CBOOL   MES_MCUD_CheckBusy( void );
CBOOL   MES_MCUD_CanPowerDown( void );
//@} 

//--------------------------------------------------------------------------
///	@name	Memory type settings
//--------------------------------------------------------------------------
//@{
void		            MES_MCUD_SetSDRType( MES_MCUD_SDRTYPE type );           
MES_MCUD_SDRTYPE		MES_MCUD_GetSDRType( void );                            

void		MES_MCUD_SetSDRBusWidth ( U32 buswidth );
U32			MES_MCUD_GetSDRBusWidth ( void );
void		MES_MCUD_SetSDRDataWidth( U32 datawidth ); 
U32			MES_MCUD_GetSDRDataWidth( void );
void		MES_MCUD_SetSDRCapacity ( U32 capacity );
U32			MES_MCUD_GetSDRCapacity ( void );
//@}

//--------------------------------------------------------------------------
///	@name	Mode Register Settings
//--------------------------------------------------------------------------
//@{
void	            MES_MCUD_SetCASLatency ( MES_MCUD_LATENCY cycle );
MES_MCUD_LATENCY	MES_MCUD_GetCASLatency ( void );
void	            MES_MCUD_SetReadLatency( MES_MCUD_LATENCY cycle );
MES_MCUD_LATENCY    MES_MCUD_GetReadLatency( void );

void	            MES_MCUD_SetDIC( MES_MCUD_DIC dic );
MES_MCUD_DIC		MES_MCUD_GetDIC( void );

void	MES_MCUD_SetDLLEnable( CBOOL bEnb );
CBOOL	MES_MCUD_IsDLLEnable ( void );

void	MES_MCUD_ApplyModeSetting ( void );
CBOOL	MES_MCUD_IsBusyModeSetting( void );
//@}

//--------------------------------------------------------------------------
///	@name	Timing settings
//--------------------------------------------------------------------------
//@{
void	MES_MCUD_SetMRD( U32 clocks );
U32		MES_MCUD_GetMRD( void );
void	MES_MCUD_SetRP ( U32 clocks );
U32		MES_MCUD_GetRP ( void );
void	MES_MCUD_SetRCD( U32 clocks );
U32		MES_MCUD_GetRCD( void );
void	MES_MCUD_SetRC ( U32 clocks );
U32		MES_MCUD_GetRC ( void );
void	MES_MCUD_SetRAS( U32 clocks );
U32		MES_MCUD_GetRAS( void );
void	MES_MCUD_SetWR ( U32 clocks );
U32		MES_MCUD_GetWR ( void );
//@}

//--------------------------------------------------------------------------
///	@name	Refresh period settings
//--------------------------------------------------------------------------
//@{
void	MES_MCUD_SetRefreshPeriod( U32 period );
U32		MES_MCUD_GetRefreshPeriod( void );
//@}

//--------------------------------------------------------------------------
///	@name	Block addressing mode settings
//--------------------------------------------------------------------------
//@{
void 	MES_MCUD_SetDisplayBlockMode( CBOOL bEnb );
CBOOL	MES_MCUD_GetDisplayBlockMode( void );
//@}

//--------------------------------------------------------------------------
///	@name	Delay settings
//--------------------------------------------------------------------------
//@{
void	        MES_MCUD_SetClockDelay( MES_MCUD_DELAY delay );
MES_MCUD_DELAY	MES_MCUD_GetClockDelay( void );

void	MES_MCUD_SetDQSDelay( MES_MCUD_DELAY DQS0OUT,   MES_MCUD_DELAY DQS1OUT,   MES_MCUD_DELAY DQS0IN,   MES_MCUD_DELAY DQS1IN );
void	MES_MCUD_GetDQSDelay( MES_MCUD_DELAY *pDQS0OUT, MES_MCUD_DELAY *pDQS1OUT, MES_MCUD_DELAY *pDQS0IN, MES_MCUD_DELAY *pDQS1IN );
//@}

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_MCUD_H__

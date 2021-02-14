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
//	Module     : MCUD
//	File       : mes_mcud.c
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------

#include "mes_mcud.h"

/// @brief  MCUD Module's Register List
struct  MES_MCUD_RegisterSet
{
    volatile U16 MEMCFG      	; ///< 0x00 [RW] Memory Configuration Register
    volatile U16 MEMTIME0    	; ///< 0x02 [RW] Memory Timing #0 Register
    volatile U16 MEMTIME1    	; ///< 0x04 [RW] Memory Timing #1 Register
    volatile U16 MEMACTPWD   	; ///< 0x06 [RW] Memory Active Power Down Control Register
    volatile U16 MEMREFRESH  	; ///< 0x08 [RW] Memory Refresh Control Register
    volatile U16 MEMCONTROL  	; ///< 0x0A [RW] Memory Control Register
    volatile U16 MEMCLKDELAY 	; ///< 0x0C [RW] Memory Clock Output Delay Register
    volatile U16 MEMDQSOUTDELAY ; ///< 0x0E [RW] Memory DQS Output Delay Register
    volatile U16 MEMDQSINDELAY  ; ///< 0x10 [RW] Memory DQS Input Delay Register
};

static  struct
{
    struct MES_MCUD_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_MCUD_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_MCUD_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see    MES_MCUD_SelectModule,
 *          MES_MCUD_GetCurrentModule,   MES_MCUD_GetNumberOfModule
 */
CBOOL   MES_MCUD_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_MCUD_MODULE; i++ )
		{
			__g_ModuleVariables[i].pRegister = CNULL;
		}
		
		bInit = CTRUE;
	}
	
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Select module to control
 *  @param[in]  ModuleIndex     Module index to select.
 *	@return		the index of previously selected module.
 *  @see        MES_MCUD_Initialize,
 *              MES_MCUD_GetCurrentModule,   MES_MCUD_GetNumberOfModule
 */
U32    MES_MCUD_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_MCUD_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_MCUD_Initialize,         MES_MCUD_SelectModule,
 *              MES_MCUD_GetNumberOfModule
 */
U32     MES_MCUD_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_MCUD_Initialize,         MES_MCUD_SelectModule,
 *              MES_MCUD_GetCurrentModule
 */
U32     MES_MCUD_GetNumberOfModule( void )
{
    return NUMBER_OF_MCUD_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see        MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_SetBaseAddress,       MES_MCUD_GetBaseAddress,
 *              MES_MCUD_OpenModule,           MES_MCUD_CloseModule,
 *              MES_MCUD_CheckBusy,            MES_MCUD_CanPowerDown
 */
U32     MES_MCUD_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_MCUD_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_MCUD_MODULE + (OFFSET_OF_MCUD_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_MCUD_GetPhysicalAddress,
 *              MES_MCUD_SetBaseAddress,       MES_MCUD_GetBaseAddress,
 *              MES_MCUD_OpenModule,           MES_MCUD_CloseModule,
 *              MES_MCUD_CheckBusy,            MES_MCUD_CanPowerDown
 */
U32     MES_MCUD_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_MCUD_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_MCUD_GetPhysicalAddress,   MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_GetBaseAddress,
 *              MES_MCUD_OpenModule,           MES_MCUD_CloseModule,
 *              MES_MCUD_CheckBusy,            MES_MCUD_CanPowerDown
 */
void    MES_MCUD_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_MCUD_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_MCUD_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_MCUD_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_MCUD_GetPhysicalAddress,   MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_SetBaseAddress,
 *              MES_MCUD_OpenModule,           MES_MCUD_CloseModule,
 *              MES_MCUD_CheckBusy,            MES_MCUD_CanPowerDown
 */
U32     MES_MCUD_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_MCUD_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_MCUD_GetPhysicalAddress,   MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_SetBaseAddress,       MES_MCUD_GetBaseAddress,
 *              MES_MCUD_CloseModule,
 *              MES_MCUD_CheckBusy,            MES_MCUD_CanPowerDown
 */
CBOOL   MES_MCUD_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_MCUD_GetPhysicalAddress,   MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_SetBaseAddress,       MES_MCUD_GetBaseAddress,
 *              MES_MCUD_OpenModule,
 *              MES_MCUD_CheckBusy,            MES_MCUD_CanPowerDown
 */
CBOOL   MES_MCUD_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_MCUD_GetPhysicalAddress,   MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_SetBaseAddress,       MES_MCUD_GetBaseAddress,
 *              MES_MCUD_OpenModule,           MES_MCUD_CloseModule,
 *              MES_MCUD_CanPowerDown
 */
CBOOL   MES_MCUD_CheckBusy( void )
{
	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_MCUD_GetPhysicalAddress,   MES_MCUD_GetSizeOfRegisterSet,
 *              MES_MCUD_SetBaseAddress,       MES_MCUD_GetBaseAddress,
 *              MES_MCUD_OpenModule,           MES_MCUD_CloseModule,
 *              MES_MCUD_CheckBusy
 */
CBOOL   MES_MCUD_CanPowerDown( void )
{
    return CTRUE;
}

//------------------------------------------------------------------------------
// MCUD Operation.
//------------------------------------------------------------------------------

static U16 MES_MCUD_SetValue( U16 OldValue, U16 BitValue, U32 MSB, U32 LSB )
{
    U16 mask;
	MES_ASSERT( 16 > MSB );
	MES_ASSERT( 16 > LSB );
	MES_ASSERT( MSB >= LSB );
	mask = (U16)(((1<<(MSB-LSB+1)) - 1) << LSB);
	MES_ASSERT( 0 == ((BitValue<<LSB)&(~mask)) );
	return (U16)((OldValue & ~mask) | (BitValue << LSB));		
}

static U16 MES_MCUD_GetValue( U16 Value, U32 MSB, U32 LSB )
{
    U16 mask;
	MES_ASSERT( 16 > MSB );
	MES_ASSERT( 16 > LSB );
	MES_ASSERT( MSB >= LSB );
	mask = (U16)(((1<<(MSB-LSB+1)) - 1) << LSB);
	return (U16)((Value & mask)>>LSB);
}

//--------------------------------------------------------------------------
//	Memory type settings
//--------------------------------------------------------------------------

/**
 *  @brief	    Set DRAM type.
 *  @param[in]  type    DRAM type.
 *  @return	    None.
 *	@see	    MES_MCUD_GetSDRType, MES_MCUD_SetSDRBusWidth, MES_MCUD_GetSDRBusWidth, 
 *              MES_MCUD_SetSDRDataWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_SetSDRCapacity, MES_MCUD_GetSDRCapacity
 */
void    MES_MCUD_SetSDRType( MES_MCUD_SDRTYPE type )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 1 == type );
	__g_pRegister->MEMCFG = MES_MCUD_SetValue( __g_pRegister->MEMCFG, (U16)type, 7, 6 );    
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get DRAM type.
 *  @return	DRAM type.
 *	@see	MES_MCUD_SetSDRType, MES_MCUD_SetSDRBusWidth, MES_MCUD_GetSDRBusWidth, 
 *          MES_MCUD_SetSDRDataWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_SetSDRCapacity, MES_MCUD_GetSDRCapacity
 */
MES_MCUD_SDRTYPE		MES_MCUD_GetSDRType( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (MES_MCUD_SDRTYPE)MES_MCUD_GetValue( __g_pRegister->MEMCFG, 7, 6 );    
}


//------------------------------------------------------------------------------
/**
 *  @brief	    Set memory bus bit-width of MCU.
 *  @param[in]  buswidth    Memory bus bit-width, 16.
 *  @return	    None.
 *	@see	    MES_MCUD_SetSDRType, MES_MCUD_GetSDRType, MES_MCUD_GetSDRBusWidth, 
 *              MES_MCUD_SetSDRDataWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_SetSDRCapacity, MES_MCUD_GetSDRCapacity
 */
void    MES_MCUD_SetSDRBusWidth
( 
	U32 buswidth	
)
{
	MES_ASSERT( (16==buswidth) );
	MES_ASSERT( CNULL != __g_pRegister );
	buswidth >>= 3;		//  divide by 8
	__g_pRegister->MEMCFG = MES_MCUD_SetValue( __g_pRegister->MEMCFG, (U16)buswidth, 5, 4 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get memory bus bit-width of MCU.
 *  @return	memory bus bit-width of MCU.
 *	@see	MES_MCUD_SetSDRType, MES_MCUD_GetSDRType, MES_MCUD_SetSDRBusWidth, 
 *          MES_MCUD_SetSDRDataWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_SetSDRCapacity, MES_MCUD_GetSDRCapacity
 */
U32     MES_MCUD_GetSDRBusWidth ( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return ((U32)MES_MCUD_GetValue( __g_pRegister->MEMCFG, 5, 4 )) << 3;	// * 8
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set data bit-width of a DRAM.
 *  @param[in]  datawidth    Data bit-width, 8 or 16.
 *  @return	    None.
 *	@see	    MES_MCUD_SetSDRType, MES_MCUD_GetSDRType, MES_MCUD_SetSDRBusWidth, 
 *              MES_MCUD_GetSDRBusWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_SetSDRCapacity, MES_MCUD_GetSDRCapacity
 */
void    MES_MCUD_SetSDRDataWidth
( 
	U32 datawidth		
)
{
	MES_ASSERT( (8==datawidth) || (16==datawidth) );
	MES_ASSERT( CNULL != __g_pRegister );
	datawidth >>= 3;	//  divide by 8
	__g_pRegister->MEMCFG = MES_MCUD_SetValue( __g_pRegister->MEMCFG, (U16)datawidth, 3, 2 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get data bit-width of MCU.
 *  @return	data bit-width of MCU.
 *	@see	MES_MCUD_SetSDRType, MES_MCUD_GetSDRType, MES_MCUD_SetSDRBusWidth, 
 *          MES_MCUD_GetSDRBusWidth, MES_MCUD_SetSDRDataWidth, MES_MCUD_SetSDRCapacity, MES_MCUD_GetSDRCapacity
 */
U32     MES_MCUD_GetSDRDataWidth ( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return ((U32)MES_MCUD_GetValue( __g_pRegister->MEMCFG, 3, 2 )) << 3;	// * 8
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set DRAM capacity.
 *  @param[in]  capacity    DRAM capacity in MBits, 64 or 128 or 256 or 512.
 *  @return	    None.
 *	@see	    MES_MCUD_SetSDRType, MES_MCUD_GetSDRType, MES_MCUD_SetSDRBusWidth, 
 *              MES_MCUD_GetSDRBusWidth, MES_MCUD_SetSDRDataWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_GetSDRCapacity
 */
void    MES_MCUD_SetSDRCapacity
( 
	U32 capacity		
)
{
	MES_ASSERT( (64==capacity) || (128==capacity) || (256==capacity) || (512==capacity) );
	MES_ASSERT( CNULL != __g_pRegister );
	capacity >>= 7;		//  divide by 128
	if( 4 <= capacity )		capacity = 3;
	__g_pRegister->MEMCFG = MES_MCUD_SetValue( __g_pRegister->MEMCFG, (U16)capacity, 1, 0 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get DRAM capacity.
 *  @return	DRAM capacity in MBits.
 *	@see	MES_MCUD_SetSDRType, MES_MCUD_GetSDRType, MES_MCUD_SetSDRBusWidth, 
 *          MES_MCUD_GetSDRBusWidth, MES_MCUD_SetSDRDataWidth, MES_MCUD_GetSDRDataWidth, MES_MCUD_SetSDRCapacity
 */
U32     MES_MCUD_GetSDRCapacity ( void )
{
    register U32 temp;
	MES_ASSERT( CNULL != __g_pRegister );
	temp = (U32)MES_MCUD_GetValue( __g_pRegister->MEMCFG, 1, 0 );
	return (64UL<<temp);
}

//------------------------------------------------------------------------------
//	Mode Register Settings
//------------------------------------------------------------------------------
/**
 *  @brief	    Set new CAS latency.
 *  @param[in]  cycle    New CAS latency
 *  @return	    None.
 *	@remark	    The system has affected by this setting after calling the function MES_MCUD_ApplyModeSetting(). 
 *	@see	                                MES_MCUD_GetCASLatency,
 *              MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *              MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *              MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
void    MES_MCUD_SetCASLatency
(
	MES_MCUD_LATENCY cycle	
)
{
	const U16 RWD_MASK = 1<<2;

	MES_ASSERT( (0 < cycle) && (4 > cycle) );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMTIME1 = MES_MCUD_SetValue( __g_pRegister->MEMTIME1, (U16)cycle, 13, 12 );
	__g_pRegister->MEMCONTROL &= (U16)~RWD_MASK;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get current CAS latency.
 *  @return	Currnet CAS latency.
 *	@see	    MES_MCUD_SetCASLatency,     
 *              MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *              MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *              MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
MES_MCUD_LATENCY    MES_MCUD_GetCASLatency( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (MES_MCUD_LATENCY)MES_MCUD_GetValue( __g_pRegister->MEMTIME1, 13, 12 );	
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set new Read latency.
 *  @param[in]  cycle    New Read latency
 *  @return	    None.
 *	@remark	    The system has affected by this setting after calling the function MES_MCUD_ApplyModeSetting(). 
 *	@see	    MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *                                          MES_MCUD_GetReadLatency,
 *              MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *              MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
void    MES_MCUD_SetReadLatency
(
	MES_MCUD_LATENCY cycle	
)
{
	MES_ASSERT( (0 < cycle) && (4 > cycle) );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMCONTROL = MES_MCUD_SetValue( __g_pRegister->MEMCONTROL, (U16)cycle, 7, 6 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get current Read latency.
 *  @return	Currnet Read latency.
 *	@see	    MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *              MES_MCUD_SetReadLatency,    
 *              MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *              MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
MES_MCUD_LATENCY    MES_MCUD_GetReadLatency( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (MES_MCUD_LATENCY)MES_MCUD_GetValue( __g_pRegister->MEMCONTROL, 9, 8 );
}


//------------------------------------------------------------------------------
/**
 *  @brief	    Set output driver impedance control.
 *  @param[in]  dic    output driver impedance control.
 *  @return	    None.
 *	@remark	    The normal DDR-DRAM has only affected by this function.\n
 *			    The system has affected by this setting after calling the function MES_MCUD_ApplyModeSetting(). 
 *	@see	    MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *              MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *                                          MES_MCUD_GetDIC,
 *              MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
void    MES_MCUD_SetDIC
(
	MES_MCUD_DIC dic		
)
{
	MES_ASSERT( 2 > dic );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMCFG = MES_MCUD_SetValue( __g_pRegister->MEMCFG, (U16)dic, 9, 9 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get currnet output driver impedance control.
 *  @return	Currnet output driver impedance control.
 *	@see	    MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *              MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *              MES_MCUD_SetDIC,            
 *              MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
MES_MCUD_DIC    MES_MCUD_GetDIC( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (MES_MCUD_DIC)MES_MCUD_GetValue( __g_pRegister->MEMCFG, 9, 9 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Enable/Disable DLL.
 *  @param[in]  bEnb    Set it to CTRUE to enable DLL.
 *  @return	    None.
 *	@remark	    The normal DDR-DRAM has only affected by this function.\n
 *			    The system has affected by this setting after calling the function MES_MCUD_ApplyModeSetting(). 
 *	@see	    MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *              MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *              MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *                                          MES_MCUD_IsDLLEnable,
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
void    MES_MCUD_SetDLLEnable
(
	CBOOL	bEnb	
)
{
	const U16 bitmask = 1<<8;
	MES_ASSERT( CNULL != __g_pRegister );

	if( bEnb )	__g_pRegister->MEMCFG &= (U16)~bitmask;
	else		__g_pRegister->MEMCFG |= (U16)bitmask;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Informs whether DLL is enabled or not.
 *  @return	\b CTRUE indicates DLL is enabled.\n
 *			\b CFALSE indicates DLL is disabled.
 *	@see	    MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *              MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *              MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *              MES_MCUD_SetDLLEnable,      
 *              MES_MCUD_ApplyModeSetting,  MES_MCUD_IsBusyModeSetting
 */
CBOOL   MES_MCUD_IsDLLEnable( void )
{
	const U16 bitmask = 1<<8;
	MES_ASSERT( CNULL != __g_pRegister );

	return (__g_pRegister->MEMCFG & bitmask) ? CFALSE : CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Apply Mode Register setting on DRAM.
 *	@return	None.
 *  @remark The system has affected by mode register settings after you call this funcion.
 *			After calling this function, you have to wait until a mode setting sequence has completed.
 *			Therefore the mode setting sequence is as follows.
 *	@code
 *		Some functions for MRS or EMRS.
 *		MES_MCUD_ApplyModeSetting();
 *		while( MES_MCUD_IsBusyModeSetting() );
 *	@endcode
 *	@see	 MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *           MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *           MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *           MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *                                       MES_MCUD_IsBusyModeSetting
 */
void    MES_MCUD_ApplyModeSetting( void )
{
	const U16 bitmask = 1<<15;

	MES_ASSERT( CNULL != __g_pRegister );

	__g_pRegister->MEMTIME1 |= (U16)bitmask;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Informs whether a mode setting sequence is busy or not.
 *	@return	\b CTRUE indicates a mode setting sequence is busy.\n
 *			\b CFALSE indicates a mode setting sequence has completed.
 *	@see	MES_MCUD_SetCASLatency,     MES_MCUD_GetCASLatency,
 *          MES_MCUD_SetReadLatency,    MES_MCUD_GetReadLatency,
 *          MES_MCUD_SetDIC,            MES_MCUD_GetDIC,
 *          MES_MCUD_SetDLLEnable,      MES_MCUD_IsDLLEnable,
 *          MES_MCUD_ApplyModeSetting  
 */
CBOOL   MES_MCUD_IsBusyModeSetting( void )
{
	const U16 bitmask = 1<<15;

	MES_ASSERT( CNULL != __g_pRegister );

	return (__g_pRegister->MEMTIME1 & bitmask) ? CTRUE : CFALSE;
}

//------------------------------------------------------------------------------
//	Timing settings
//------------------------------------------------------------------------------
/**
 *  @brief	    Set a tMRD (Mode register set cycle).
 *  @param[in]  clocks    Memory clocks for tMRD, 1 ~ 16.
 *	@return	    None.
 * 	@see	                        MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
void    MES_MCUD_SetMRD
(
	U32	clocks	
)
{
	MES_ASSERT( (1 <= clocks) && (16 >= clocks) );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMTIME0 = MES_MCUD_SetValue( __g_pRegister->MEMTIME0, (U16)clocks-1, 15, 12 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a tMRD (Mode register set cycle).
 *	@return	memory clocks for tMRD.
 * 	@see	    MES_MCUD_SetMRD,    
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
U32  MES_MCUD_GetMRD( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(MES_MCUD_GetValue( __g_pRegister->MEMTIME0, 15, 12 )+1);
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a tRP (Row precharge time).
 *  @param[in]  clocks    Memory clocks for tRP, 1 ~ 16.
 *	@remark		IMPORTANT : The tRP - 1 is used in mode register set
 *	@return	    None.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *                                  MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
void    MES_MCUD_SetRP
(
	U32	clocks	
)
{
	MES_ASSERT( (1 <= clocks) && (16 >= clocks) );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMTIME0 = MES_MCUD_SetValue( __g_pRegister->MEMTIME0, (U16)clocks-1, 7, 4 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a tRP (Row precharge time).
 *	@return	memory clocks for tRP.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
U32  MES_MCUD_GetRP( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(MES_MCUD_GetValue( __g_pRegister->MEMTIME0, 7, 4 )+1);
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a tRCD (RAS to CAS delay).
 *  @param[in]  clocks    Memory clocks for tRCD, 1 ~ 16.
 *	@return	    None.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *                                  MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
void    MES_MCUD_SetRCD
(
	U32	clocks	
)
{
	MES_ASSERT( (1 <= clocks) && (16 >= clocks) );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMTIME0 = MES_MCUD_SetValue( __g_pRegister->MEMTIME0, (U16)clocks-1, 3, 0 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a tRCD (RAS to CAS delay).
 *	@return	memory clocks for tRCD.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
U32  MES_MCUD_GetRCD( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(MES_MCUD_GetValue( __g_pRegister->MEMTIME0, 3, 0 )+1);
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a tRC (Row cycle time, REF to ACT command).
 *  @param[in]  clocks    Memory clocks for tRC, 2 ~ 17.
 *	@return	    None.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *                                  MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
void    MES_MCUD_SetRC
(
	U32	clocks	
)
{
	const U16 modeset = 1<<15;
	register U16 regvalue;
	
	MES_ASSERT( (2 <= clocks) && (17 >= clocks) );
	MES_ASSERT( CNULL != __g_pRegister );
	regvalue = __g_pRegister->MEMTIME1;

	regvalue &= ~modeset;		// Unmask MODESET
	__g_pRegister->MEMTIME1 = MES_MCUD_SetValue( regvalue, (U16)clocks-2, 11, 8 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a tRC (Row cycle time, REF to ACT command).
 *	@return	memory clocks for tRC.
 * 	@see	MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *          MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *          MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *          MES_MCUD_SetRC,     
 *          MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *          MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
U32  MES_MCUD_GetRC( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(MES_MCUD_GetValue( __g_pRegister->MEMTIME1, 11, 8 )+2);
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a tRAS (Row active time, ACT to PRE command).
 *  @param[in]  clocks    Memory clocks for tRAS, 2 ~ 17.
 *	@return	    None.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *                                  MES_MCUD_GetRAS,
 *              MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
void    MES_MCUD_SetRAS
(
	U32	clocks	
)
{
	const U16 modeset = 1<<15;
    register U16 regvalue;
    
	MES_ASSERT( (2 <= clocks) && (17 >= clocks) );
	MES_ASSERT( CNULL != __g_pRegister );
	regvalue = __g_pRegister->MEMTIME1;

	regvalue &= ~modeset;		// Unmask MODESET
	__g_pRegister->MEMTIME1 = MES_MCUD_SetValue( regvalue, (U16)clocks-2, 7, 4 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a tRAS (Row active time).
 *	@return	memory clocks for tRAS.
 * 	@see	MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *          MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *          MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *          MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *          MES_MCUD_SetRAS,    
 *          MES_MCUD_SetWR,     MES_MCUD_GetWR
 */
U32  MES_MCUD_GetRAS( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(MES_MCUD_GetValue( __g_pRegister->MEMTIME1, 7, 4 )+2);
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a tWR (Write recovery time).
 *  @param[in]  clocks    Memory clocks for tWR, 0 ~ 15.
 *	@return	    None.
 * 	@see	    MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *              MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *              MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *              MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *              MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *                                  MES_MCUD_GetWR
 */
void    MES_MCUD_SetWR
(
	U32	clocks	
)
{
	const U16 modeset = 1<<15;
    register U16 regvalue;

	MES_ASSERT( 16 > clocks );
	MES_ASSERT( CNULL != __g_pRegister );
	regvalue = __g_pRegister->MEMTIME1;

	regvalue &= ~modeset;		// Unmask MODESET
	__g_pRegister->MEMTIME1 = MES_MCUD_SetValue( regvalue, (U16)clocks, 3, 0 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a tWR (Write recovery time).
 *	@return	memory clocks for tWR.
 * 	@see	MES_MCUD_SetMRD,    MES_MCUD_GetMRD,
 *          MES_MCUD_SetRP,     MES_MCUD_GetRP,
 *          MES_MCUD_SetRCD,    MES_MCUD_GetRCD,
 *          MES_MCUD_SetRC,     MES_MCUD_GetRC,
 *          MES_MCUD_SetRAS,    MES_MCUD_GetRAS,
 *          MES_MCUD_SetWR     
 */
U32  MES_MCUD_GetWR( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(MES_MCUD_GetValue( __g_pRegister->MEMTIME1, 3, 0 ));
}


//------------------------------------------------------------------------------
//	Refresh period settings
//------------------------------------------------------------------------------
/**
 *  @brief	    Set a refresh period.
 *  @param[in]  period    A refresh period, 0 ~ 65534 and 65535 to disable.
 *	@return	    None.
 *  @remarks    The unit is a BCLK * 2
 * 	@see	    MES_MCUD_GetRefreshPeriod
 */
void    MES_MCUD_SetRefreshPeriod
(
	U32	period	
)
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 65536 > period );
	__g_pRegister->MEMREFRESH = (U16)(period);
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a refresh period.
 *	@return	a refresh period, The unit is a BCLK * 2.
 * 	@see	MES_MCUD_SetRefreshPeriod
 */
U32 MES_MCUD_GetRefreshPeriod( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (__g_pRegister->MEMREFRESH);
}

//------------------------------------------------------------------------------
//	Block addressing mode settings
//------------------------------------------------------------------------------
/**
 *  @brief	    Enable/Disable a display block addressing mode.
 *  @param[in]  bEnb    Set it to CTRUE to enable a display block addressing mode.
 *	@return	    None.
 * 	@see	    MES_MCUD_GetDisplayBlockMode
 */
void    MES_MCUD_SetDisplayBlockMode
(
	CBOOL	bEnb	
)
{
	const U16 bitmask = 1<<0;
	MES_ASSERT( CNULL != __g_pRegister );

	if( bEnb )	__g_pRegister->MEMCONTROL |= (U16)bitmask;
	else		__g_pRegister->MEMCONTROL &= (U16)~bitmask;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Informs whether a display block addressing mode is enabled or not.
 *	@return	\b CTRUE indicates a display block addressing mode is enabled.\n
 *			\b CFALSE indicates a display block addressing mode is disabled.
 * 	@see	MES_MCUD_SetDisplayBlockMode
 */
CBOOL   MES_MCUD_GetDisplayBlockMode( void )
{
	const U16 bitmask = 1<<0;
	MES_ASSERT( CNULL != __g_pRegister );

	return (__g_pRegister->MEMCONTROL & bitmask) ? CTRUE : CFALSE;
}

//------------------------------------------------------------------------------
//	Delay settings
//------------------------------------------------------------------------------
/**
 *  @brief	    Set a delay for a memory clock output.
 *  @param[in]  delay    A delay for a memory clock output.
 *	@return	    None.
 *	@remark		This value will be applied during MCU power-up sequence such
 *				as PLL change.
 * 	@see	                                MES_MCUD_GetClockDelay,
 *              MES_MCUD_SetDQSDelay,       MES_MCUD_GetDQSDelay
 */             
void    MES_MCUD_SetClockDelay
(
	MES_MCUD_DELAY	delay	
)
{
	MES_ASSERT( 8 > delay );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->MEMCLKDELAY = MES_MCUD_SetValue( __g_pRegister->MEMCLKDELAY, (U16)delay, 2, 0 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a delay for a memory clock output.
 *	@return	a delay for a memory clock output.
 *	@remark	The retrun values do not represent last writing value until PLL is changed.
 * 	@see	    MES_MCUD_SetClockDelay,     
 *              MES_MCUD_SetDQSDelay,       MES_MCUD_GetDQSDelay
 */
MES_MCUD_DELAY  MES_MCUD_GetClockDelay( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (MES_MCUD_DELAY)MES_MCUD_GetValue( __g_pRegister->MEMCLKDELAY, 2, 0 );
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set delays for DQS signals.
 *  @param[in]  DQS0OUT	a delay for a DQS0 output signal.
 *  @param[in]  DQS1OUT	a delay for a DQS1 output signal.
 *  @param[in]  DQS0IN	a delay for a DQS0 input signal.
 *  @param[in]  DQS1IN	a delay for a DQS1 input signal.
 *	@return	    None.
 *	@remark		This values will be applied during MCU power-up sequence such
 *				as PLL change.
 * 	@see	    MES_MCUD_SetClockDelay,     MES_MCUD_GetClockDelay,
 *                                          MES_MCUD_GetDQSDelay
 */
void    MES_MCUD_SetDQSDelay
(
	MES_MCUD_DELAY DQS0OUT, MES_MCUD_DELAY DQS1OUT, MES_MCUD_DELAY DQS0IN, MES_MCUD_DELAY DQS1IN
)
{
	MES_ASSERT( 8 > DQS0OUT );
	MES_ASSERT( 8 > DQS1OUT );
	MES_ASSERT( 8 > DQS0IN  );
	MES_ASSERT( 8 > DQS1IN  );
	MES_ASSERT( CNULL != __g_pRegister );

	__g_pRegister->MEMDQSOUTDELAY  = (U16)( (DQS1OUT << 4) | DQS0OUT );
	__g_pRegister->MEMDQSINDELAY   = (U16)( (DQS1IN  << 4) | DQS0IN  );
}

//------------------------------------------------------------------------------
/**
 *  @brief		Get delays for DQS signals.
 *  @param[out]	pDQS0OUT	Get a delay for a DQS0 output signal.
 *  @param[out]	pDQS1OUT	Get a delay for a DQS1 output signal.
 *  @param[out]	pDQS0IN		Get a delay for a DQS0 input signal.
 *  @param[out]	pDQS1IN		Get a delay for a DQS1 input signal.
 *	@return		None.
 *	@remark		Arguments which does not required can be CNULL.\n
 *				The read values do not represent last writing values
 *				until PLL is changed.
 * 	@see	    MES_MCUD_SetClockDelay,     MES_MCUD_GetClockDelay,
 *              MES_MCUD_SetDQSDelay       
 */
void    MES_MCUD_GetDQSDelay
(
	MES_MCUD_DELAY *pDQS0OUT, MES_MCUD_DELAY *pDQS1OUT, MES_MCUD_DELAY *pDQS0IN, MES_MCUD_DELAY *pDQS1IN
)
{
	register U32 temp;
	MES_ASSERT( CNULL != __g_pRegister );
	
	temp = (U32)__g_pRegister->MEMDQSOUTDELAY;
	if( CNULL != pDQS0OUT )	*pDQS0OUT = (MES_MCUD_DELAY)((temp >> 0) & 0x7);
	if( CNULL != pDQS1OUT )	*pDQS1OUT = (MES_MCUD_DELAY)((temp >> 4) & 0x7);
	
	temp = (U32)__g_pRegister->MEMDQSINDELAY;
	if( CNULL != pDQS0IN  )	*pDQS0IN  = (MES_MCUD_DELAY)((temp >> 0) & 0x7);
	if( CNULL != pDQS1IN  )	*pDQS1IN  = (MES_MCUD_DELAY)((temp >> 4) & 0x7);
}




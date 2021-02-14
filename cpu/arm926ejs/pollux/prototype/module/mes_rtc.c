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
//	Module     : RTC
//	File       : mes_rtc.h
//	Description: 
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------

#include "mes_rtc.h"

/// @brief  RTC Module's Register List
struct  MES_RTC_RegisterSet
{
	volatile U32 RTCCNTWRITE;	///< 0x00 : RTC counter register (Write only)
	volatile U32 RTCCNTREAD;	///< 0x04 : RTC counter register (Read only)
	volatile U32 RTCALARM;		///< 0x08 : RTC alarm register
	volatile U32 RTCCTRL;	    ///< 0x0C : RTC control register
	volatile U32 RTCINTENB;		///< 0x10 : RTC interrupt enable register
	volatile U32 RTCINTPND;	    ///< 0x14 : RTC interrupt pending register (Read only)
};

static  struct
{
    struct MES_RTC_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_RTC_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_RTC_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see                                MES_RTC_SelectModule,
 *          MES_RTC_GetCurrentModule,   MES_RTC_GetNumberOfModule
 */
CBOOL   MES_RTC_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_RTC_MODULE; i++ )
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
 *  @see        MES_RTC_Initialize,         
 *              MES_RTC_GetCurrentModule,   MES_RTC_GetNumberOfModule
 */
U32    MES_RTC_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_RTC_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;    

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_RTC_Initialize,         MES_RTC_SelectModule,
 *                                          MES_RTC_GetNumberOfModule
 */
U32     MES_RTC_GetCurrentModule( void )
{
    return __g_CurModuleIndex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @see        MES_RTC_Initialize,         MES_RTC_SelectModule,
 *              MES_RTC_GetCurrentModule   
 */
U32     MES_RTC_GetNumberOfModule( void )
{
    return NUMBER_OF_RTC_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                        MES_RTC_GetSizeOfRegisterSet,
 *              MES_RTC_SetBaseAddress,         MES_RTC_GetBaseAddress,
 *              MES_RTC_OpenModule,             MES_RTC_CloseModule,
 *              MES_RTC_CheckBusy,              MES_RTC_CanPowerDown
 */
U32     MES_RTC_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_RTC_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_RTC_MODULE );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_RTC_GetPhysicalAddress,     
 *              MES_RTC_SetBaseAddress,         MES_RTC_GetBaseAddress,
 *              MES_RTC_OpenModule,             MES_RTC_CloseModule,
 *              MES_RTC_CheckBusy,              MES_RTC_CanPowerDown
 */
U32     MES_RTC_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_RTC_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_RTC_GetPhysicalAddress,     MES_RTC_GetSizeOfRegisterSet,
 *                                              MES_RTC_GetBaseAddress,
 *              MES_RTC_OpenModule,             MES_RTC_CloseModule,
 *              MES_RTC_CheckBusy,              MES_RTC_CanPowerDown
 */
void    MES_RTC_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_RTC_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = ( struct MES_RTC_RegisterSet *)BaseAddress;
    __g_pRegister = ( struct MES_RTC_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_RTC_GetPhysicalAddress,     MES_RTC_GetSizeOfRegisterSet,
 *              MES_RTC_SetBaseAddress,         
 *              MES_RTC_OpenModule,             MES_RTC_CloseModule,
 *              MES_RTC_CheckBusy,              MES_RTC_CanPowerDown
 */
U32     MES_RTC_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_RTC_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_RTC_GetPhysicalAddress,     MES_RTC_GetSizeOfRegisterSet,
 *              MES_RTC_SetBaseAddress,         MES_RTC_GetBaseAddress,
 *                                              MES_RTC_CloseModule,
 *              MES_RTC_CheckBusy,              MES_RTC_CanPowerDown
 */
CBOOL   MES_RTC_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_RTC_GetPhysicalAddress,     MES_RTC_GetSizeOfRegisterSet,
 *              MES_RTC_SetBaseAddress,         MES_RTC_GetBaseAddress,
 *              MES_RTC_OpenModule,             
 *              MES_RTC_CheckBusy,              MES_RTC_CanPowerDown
 */
CBOOL   MES_RTC_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_RTC_GetPhysicalAddress,     MES_RTC_GetSizeOfRegisterSet,
 *              MES_RTC_SetBaseAddress,         MES_RTC_GetBaseAddress,
 *              MES_RTC_OpenModule,             MES_RTC_CloseModule,
 *                                              MES_RTC_CanPowerDown
 */
CBOOL   MES_RTC_CheckBusy( void )
{
    return ( MES_RTC_IsBusyAlarmCounter() || MES_RTC_IsBusyRTCCounter() );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_RTC_GetPhysicalAddress,     MES_RTC_GetSizeOfRegisterSet,
 *              MES_RTC_SetBaseAddress,         MES_RTC_GetBaseAddress,
 *              MES_RTC_OpenModule,             MES_RTC_CloseModule,
 *              MES_RTC_CheckBusy              
 */
CBOOL   MES_RTC_CanPowerDown( void )
{
    if( CTRUE == MES_RTC_CheckBusy() )
    {
        return CFALSE;        
    }

    return CTRUE;
}


//------------------------------------------------------------------------------
// Interrupt Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number for interrupt controller.
 *  @return     Interrupt number
 *  @see                                            MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
S32     MES_RTC_GetInterruptNumber( void )
{
    return  INTNUM_OF_RTC_MODULE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number ( 0 (RTC Counter), 1 (RTC Alarm) ) .
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @remarks    RTC module have two interrupt source that is RTC counter interrupt and RTC alarm interrupt.
 *              So. \e IntNum must have 0(Counter interrupt) or 1(Alarm interrupt).
 *  @see        MES_RTC_GetInterruptNumber,         
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
void    MES_RTC_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    register struct MES_RTC_RegisterSet*   pRegister;
    register U32 regvalue;   

    MES_ASSERT( 2 > IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );
    
    pRegister   = __g_pRegister;
    
    regvalue = pRegister->RTCINTENB;
    
    regvalue &= ~( 1UL      << IntNum);
    regvalue |= (U32)Enable << IntNum;
    
    pRegister->RTCINTENB = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number ( 0 (RTC Counter), 1 (RTC Alarm) ).
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSe indicate that Interrupt is disabled.
 *  @remarks    RTC module have two interrupt source that is RTC counter interrupt and RTC alarm interrupt.
 *              So. \e IntNum must have 0(Counter interrupt) or 1(Alarm interrupt).
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *                                                  MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
CBOOL   MES_RTC_GetInterruptEnable( S32 IntNum )
{
    MES_ASSERT( 2 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
        
    return (CBOOL) (( __g_pRegister->RTCINTENB >> IntNum ) & 0x01);
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number ( 0 (RTC Counter), 1 (RTC Alarm) ).
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @remarks    RTC module have two interrupt source that is RTC counter interrupt and RTC alarm interrupt.
 *              So. \e IntNum must have 0(Counter interrupt) or 1(Alarm interrupt).
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
CBOOL   MES_RTC_GetInterruptPending( S32 IntNum )
{
    MES_ASSERT( 2 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
        
    return (CBOOL) (( __g_pRegister->RTCINTPND >> IntNum ) & 0x01);
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt Number ( 0 (RTC Counter), 1 (RTC Alarm) ).
 *  @return     None.
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *                                                  MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
void    MES_RTC_ClearInterruptPending( S32 IntNum ) 
{
    register struct MES_RTC_RegisterSet*   pRegister;
    register U32 regvalue;   

    MES_ASSERT( 2 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
    
    pRegister   = __g_pRegister;
    
    regvalue = pRegister->RTCINTENB;    // Caution : RTC module's pending bit is cleared by write corresponding interrupt enable bit

    pRegister->RTCINTENB = regvalue & ~( 1UL << IntNum );   // disable interrupt enable bit to pending bit clear
    pRegister->RTCINTENB = regvalue;                        // restore Interrupt enable bit
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
void    MES_RTC_SetInterruptEnableAll( CBOOL Enable )
{
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );
    
    __g_pRegister->RTCINTENB = Enable<<1 | Enable;
    
    if( Enable )
    {
        __g_pRegister->RTCINTENB = 0x03;         
    }
    else
    {
        __g_pRegister->RTCINTENB = 0x00;                 
    }
    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *                                                  MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
CBOOL   MES_RTC_GetInterruptEnableAll( void )  
{
    MES_ASSERT( CNULL != __g_pRegister );
    
    if( __g_pRegister->RTCINTENB )
    {
        return CTRUE;   
    }    
    
    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are pended or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) pending is seted. \n
 *              \b CFALSE indicate that All pending is NOT seted.
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      
 *              MES_RTC_ClearInterruptPendingAll,   MES_RTC_GetInterruptPendingNumber
 */
CBOOL   MES_RTC_GetInterruptPendingAll( void )      
{
    MES_ASSERT( CNULL != __g_pRegister );
    
    if( __g_pRegister->RTCINTPND )
    {
        return CTRUE;   
    }    
    
    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *                                                  MES_RTC_GetInterruptPendingNumber
 */
void    MES_RTC_ClearInterruptPendingAll( void )
{
    register struct MES_RTC_RegisterSet*   pRegister;
    register U32 regvalue;   

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;
    
    regvalue    = pRegister->RTCINTENB;

    pRegister->RTCINTENB = 0x0;                 // All pending clear
    pRegister->RTCINTENB = regvalue;            // restore Interrupt enable bit

}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_RTC_GetInterruptNumber,         MES_RTC_SetInterruptEnable,
 *              MES_RTC_GetInterruptEnable,         MES_RTC_GetInterruptPending,
 *              MES_RTC_ClearInterruptPending,      MES_RTC_SetInterruptEnableAll,
 *              MES_RTC_GetInterruptEnableAll,      MES_RTC_GetInterruptPendingAll,
 *              MES_RTC_ClearInterruptPendingAll   
 */
S32     MES_RTC_GetInterruptPendingNumber( void )  // -1 if None    
{
    register        U32 intpend;
    static const    S32 IntNum[4] = { -1, 0, 1, 0 };

    MES_ASSERT( CNULL != __g_pRegister );
    
    intpend = __g_pRegister->RTCINTPND & 0x03;

    return IntNum[ intpend ];
}

//------------------------------------------------------------------------------
// RTC Operation.
//------------------------------------------------------------------------------
/**
 *  @brief	    Set new alarm value.
 *  @param[in]  AlarmCounter    new alarm value.
 *  @return	    None.
 *  @remark     Before you call this function, you have to check a return value of 
 *			    MES_RTC_IsBusyAlarmCounter function to ensure the alarm counter is ready
 *			    to write a new value.\n
 *  		    Therefore the sequence for changing a alarm value is as follows.
 *  @code
 * 		while( MES_RTC_IsBusyAlarmCounter() );	// wait until a last writing operation is completed.
 *		MES_RTC_SetAlarmCounter( newvalue );	// update a alarm value.
 *  @endcode
 *	@see	                                    MES_RTC_GetAlarmCounter,
 *              MES_RTC_IsBusyAlarmCounter,     MES_RTC_SetRTCCounter,
 *              MES_RTC_GetRTCCounter,          MES_RTC_IsBusyRTCCounter,
 *              MES_RTC_SetRTCCounterWriteEnable
 */
void  	MES_RTC_SetAlarmCounter( U32 AlarmCounter )
{
	MES_ASSERT( CFALSE == MES_RTC_IsBusyAlarmCounter() );
	MES_ASSERT( CNULL != __g_pRegister );

    __g_pRegister->RTCALARM = AlarmCounter;    
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a alarm value.
 *  @return	current alarm value.
 *	@see	    MES_RTC_SetAlarmCounter,        
 *              MES_RTC_IsBusyAlarmCounter,     MES_RTC_SetRTCCounter,
 *              MES_RTC_GetRTCCounter,          MES_RTC_IsBusyRTCCounter,
 *              MES_RTC_SetRTCCounterWriteEnable
 */
U32   	MES_RTC_GetAlarmCounter( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
    return __g_pRegister->RTCALARM;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Informs whether a last writing operation on alarm register is completed or not.
 *  @return	\b CTRUE indicates a writing operation on alarm counter is not completed.\n
 *			\b CFALSE indicates a writing operation on alarm counter is completed.
 *	@see	    MES_RTC_SetAlarmCounter,        MES_RTC_GetAlarmCounter,
 *                                              MES_RTC_SetRTCCounter,
 *              MES_RTC_GetRTCCounter,          MES_RTC_IsBusyRTCCounter,
 *              MES_RTC_SetRTCCounterWriteEnable
 */
CBOOL 	MES_RTC_IsBusyAlarmCounter( void )
{
	const U32 ALARMCNTWAIT	= (1UL<<3);
	MES_ASSERT( CNULL != __g_pRegister );

	return (__g_pRegister->RTCCTRL & ALARMCNTWAIT) ? CTRUE : CFALSE;    
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set new RTC counter value.
 *  @param[in]  RTCCounter    new RTC counter value.
 *  @return	    None.
 *	@remark     Before you call this function, you have to ensure CPU can access a 
 *			    RTC counter block. After you call this function, you have to wait
 *			    until a writing operation is completed before you isolate a RTC 
 *			    counter block.\n
 *  		    Therefore the sequence for changing a alarm value is as follows.
 *  @code
 *		MES_RTC_SetRTCCounterWriteEnable( CTRUE );	// Deisolate a RTC counter block.
 *		MES_RTC_SetRTCCounter( newvalue );          // update a RTC counter value.
 * 		while( MES_RTC_IsBusyRTCCounter() );		// wait until a writing operation is completed.
 *		MES_RTC_SetRTCCounterWriteEnable( CFALSE );	// Isolate a RTC counter block.
 *  @endcode
 *	@see	    MES_RTC_SetAlarmCounter,        MES_RTC_GetAlarmCounter,
 *              MES_RTC_IsBusyAlarmCounter,     
 *              MES_RTC_GetRTCCounter,          MES_RTC_IsBusyRTCCounter,
 *              MES_RTC_SetRTCCounterWriteEnable
 */
void 	MES_RTC_SetRTCCounter( U32 RTCCounter )
{
	MES_ASSERT( CFALSE == MES_RTC_IsBusyRTCCounter() );
	MES_ASSERT( CNULL != __g_pRegister );
    __g_pRegister->RTCCNTWRITE = RTCCounter;    
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get current RTC counter value.
 *  @return	current RTC counter value.
 *	@see	    MES_RTC_SetAlarmCounter,        MES_RTC_GetAlarmCounter,
 *              MES_RTC_IsBusyAlarmCounter,     MES_RTC_SetRTCCounter,
 *                                              MES_RTC_IsBusyRTCCounter,
 *              MES_RTC_SetRTCCounterWriteEnable
 */
U32  	MES_RTC_GetRTCCounter( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
    return __g_pRegister->RTCCNTREAD;    
}

//------------------------------------------------------------------------------
/**
 *  @brief	Informs whether a last writing operation on RTC counter register is completed or not.
 *  @return	\b CTRUE indicates a writing operation on RTC counter is not completed.\n
 *			\b CFALSE indicates a writing operation on RTC counter is completed.
 *	@see	    MES_RTC_SetAlarmCounter,        MES_RTC_GetAlarmCounter,
 *              MES_RTC_IsBusyAlarmCounter,     MES_RTC_SetRTCCounter,
 *              MES_RTC_GetRTCCounter,          
 *              MES_RTC_SetRTCCounterWriteEnable
 */
CBOOL 	MES_RTC_IsBusyRTCCounter( void )
{
	const U32 RTCCNTWAIT	= (1UL<<4);
	MES_ASSERT( CNULL != __g_pRegister );

	return (__g_pRegister->RTCCTRL & RTCCNTWAIT) ? CTRUE : CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Enable/Disable to write data on RTC registers.
 *  @param[in]  enable    Set it to CTRUE to enable a writing on RTC counter register.
 *  @return	    None.
 *	@remark	    RTC counter module is always powered on regardless the system power
 *		 	    modes. RTC has a isolation function to prevent unpredictable writing
 * 			    on RTC counter register	by a power-off sequence of other modules.
 *			    A calling this function with CFALSE isolates RTC counter block and 
 *			    prevents a writing on RTC counter by CPU. A calling this function
 *			    with CTURE deisolates RTC counter block and allows a writing on RTC
 *			    counter by CPU. Therefore you have to call this function with CFALSE
 * 			    before entering	a power-down sequence. A reading on RTC counter has
 * 			    no affect by this function.
 *	@see	    MES_RTC_SetAlarmCounter,        MES_RTC_GetAlarmCounter,
 *              MES_RTC_IsBusyAlarmCounter,     MES_RTC_SetRTCCounter,
 *              MES_RTC_GetRTCCounter,          MES_RTC_IsBusyRTCCounter
 */
void    MES_RTC_SetRTCCounterWriteEnable( CBOOL enable )
{
	const U32 WRITEENB_POS	= 0;
	const U32 WRITEENB	    = 1UL << 0;
    register struct MES_RTC_RegisterSet    *pRegister;
    register U32 regvalue;

	MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( (0==enable) || (1==enable) );

    pRegister = __g_pRegister;
    regvalue = pRegister->RTCCTRL;
    
    regvalue &= ~(WRITEENB);
    regvalue |= (enable) << WRITEENB_POS;
    
    pRegister->RTCCTRL = regvalue;
}


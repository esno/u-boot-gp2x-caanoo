//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : TIMER
//	File       : mes_timer.c
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------

#include "mes_timer.h"

/// @brief  TIMER Module's Register List
struct  MES_TIMER_RegisterSet
{
    volatile U32 TMRCOUNT;		    ///< 0x00 : Timer counter register
    volatile U32 TMRMATCH;		    ///< 0x04 : Timer match register
    volatile U32 TMRCONTROL;	    ///< 0x08 : Timer control register
    volatile U32 _RESERVED[0x0D];   ///< 0x0C ~ 0x3C : Reserved region
    volatile U32 TMRCLKENB;         ///< 0x40 : Timer clock generation enable register
    volatile U32 TMRCLKGEN;         ///< 0x44 : Timer clock generation control register
};

static  struct
{
    struct MES_TIMER_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_TIMER_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_TIMER_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see                               MES_TIMER_SelectModule,
 *          MES_TIMER_GetCurrentModule,MES_TIMER_GetNumberOfModule
 */
CBOOL   MES_TIMER_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_TIMER_MODULE; i++ )
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
 *  @see        MES_TIMER_Initialize,      
 *              MES_TIMER_GetCurrentModule,MES_TIMER_GetNumberOfModule
 */
U32    MES_TIMER_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_TIMER_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;
    
	return oldindex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_TIMER_Initialize,      MES_TIMER_SelectModule,
 *                                         MES_TIMER_GetNumberOfModule
 */
U32     MES_TIMER_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_TIMER_Initialize,      MES_TIMER_SelectModule,
 *              MES_TIMER_GetCurrentModule
 */
U32     MES_TIMER_GetNumberOfModule( void )
{
    return NUMBER_OF_TIMER_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                        MES_TIMER_GetSizeOfRegisterSet,
 *              MES_TIMER_SetBaseAddress,       MES_TIMER_GetBaseAddress,
 *              MES_TIMER_OpenModule,           MES_TIMER_CloseModule,
 *              MES_TIMER_CheckBusy,            MES_TIMER_CanPowerDown
 */
U32     MES_TIMER_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_TIMER_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_TIMER_MODULE + (OFFSET_OF_TIMER_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_TIMER_GetPhysicalAddress,   
 *              MES_TIMER_SetBaseAddress,       MES_TIMER_GetBaseAddress,
 *              MES_TIMER_OpenModule,           MES_TIMER_CloseModule,
 *              MES_TIMER_CheckBusy,            MES_TIMER_CanPowerDown
 */
U32     MES_TIMER_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_TIMER_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_TIMER_GetPhysicalAddress,   MES_TIMER_GetSizeOfRegisterSet,
 *                                              MES_TIMER_GetBaseAddress,
 *              MES_TIMER_OpenModule,           MES_TIMER_CloseModule,
 *              MES_TIMER_CheckBusy,            MES_TIMER_CanPowerDown
 */
void    MES_TIMER_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_TIMER_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_TIMER_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_TIMER_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_TIMER_GetPhysicalAddress,   MES_TIMER_GetSizeOfRegisterSet,
 *              MES_TIMER_SetBaseAddress,       
 *              MES_TIMER_OpenModule,           MES_TIMER_CloseModule,
 *              MES_TIMER_CheckBusy,            MES_TIMER_CanPowerDown
 */
U32     MES_TIMER_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_TIMER_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_TIMER_GetPhysicalAddress,   MES_TIMER_GetSizeOfRegisterSet,
 *              MES_TIMER_SetBaseAddress,       MES_TIMER_GetBaseAddress,
 *                                              MES_TIMER_CloseModule,
 *              MES_TIMER_CheckBusy,            MES_TIMER_CanPowerDown
 */
CBOOL   MES_TIMER_OpenModule( void )
{
	return CTRUE;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_TIMER_GetPhysicalAddress,   MES_TIMER_GetSizeOfRegisterSet,
 *              MES_TIMER_SetBaseAddress,       MES_TIMER_GetBaseAddress,
 *              MES_TIMER_OpenModule,           
 *              MES_TIMER_CheckBusy,            MES_TIMER_CanPowerDown
 */
CBOOL   MES_TIMER_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_TIMER_GetPhysicalAddress,   MES_TIMER_GetSizeOfRegisterSet,
 *              MES_TIMER_SetBaseAddress,       MES_TIMER_GetBaseAddress,
 *              MES_TIMER_OpenModule,           MES_TIMER_CloseModule,
 *                                              MES_TIMER_CanPowerDown
 */
CBOOL   MES_TIMER_CheckBusy( void )
{
	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_TIMER_GetPhysicalAddress,   MES_TIMER_GetSizeOfRegisterSet,
 *              MES_TIMER_SetBaseAddress,       MES_TIMER_GetBaseAddress,
 *              MES_TIMER_OpenModule,           MES_TIMER_CloseModule,
 *              MES_TIMER_CheckBusy            
 */
CBOOL   MES_TIMER_CanPowerDown( void )
{
    return CTRUE;
}


//------------------------------------------------------------------------------
// Interrupt Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number for interrupt controller.
 *  @return     Interrupt number
 *  @see                                            MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
S32     MES_TIMER_GetInterruptNumber( void )
{
    static  const S32 MES_TIMER_IntNumber[] = { INTNUM_OF_TIMER0_MODULE, INTNUM_OF_TIMER1_MODULE,
                                                INTNUM_OF_TIMER2_MODULE, INTNUM_OF_TIMER3_MODULE,
                                                INTNUM_OF_TIMER4_MODULE };

    MES_ASSERT( 5 > __g_CurModuleIndex );

    return  MES_TIMER_IntNumber[__g_CurModuleIndex];
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number .
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @see        MES_TIMER_GetInterruptNumber,       
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
void    MES_TIMER_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    register struct MES_TIMER_RegisterSet* pRegister;
    register U32    regvalue;

    const   U32 INTPEND_POS = 5;
    const   U32 INTENB_POS  = 4;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );
  	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;

    regvalue = pRegister->TMRCONTROL;

    regvalue &=  ~(( 1UL << INTPEND_POS ) | ( 1UL << INTENB_POS ) );
    regvalue |= (U32)Enable << INTENB_POS;

    pRegister->TMRCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSe indicate that Interrupt is disabled.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *                                                  MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
CBOOL   MES_TIMER_GetInterruptEnable( S32 IntNum )
{
    const   U32 INTENB_POS  = 4;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( ( __g_pRegister->TMRCONTROL >> INTENB_POS ) & 0x01 );
}


//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
CBOOL   MES_TIMER_GetInterruptPending( S32 IntNum )
{
    const   U32 INTPEND_POS  = 5;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( ( __g_pRegister->TMRCONTROL >> INTPEND_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt number.
 *  @return     None.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *                                                  MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
void    MES_TIMER_ClearInterruptPending( S32 IntNum )
{
    const   U32 INTPEND_POS  = 5;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
  	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    __g_pRegister->TMRCONTROL |= 1UL << INTPEND_POS;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
void    MES_TIMER_SetInterruptEnableAll( CBOOL Enable )
{
    register struct MES_TIMER_RegisterSet* pRegister;
    register U32    regvalue;

    const   U32 INTPEND_POS = 5;
    const   U32 INTENB_POS  = 4;

    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );
  	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;

    regvalue = pRegister->TMRCONTROL;

    regvalue &=  ~(( 1UL << INTPEND_POS ) | ( 1UL << INTENB_POS ));
    regvalue |= (U32)Enable << INTENB_POS ;

    pRegister->TMRCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *                                                  MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
CBOOL   MES_TIMER_GetInterruptEnableAll( void )
{
    const   U32 INTENB_POS  = 4;

    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( (__g_pRegister->TMRCONTROL >> INTENB_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are pended or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) pending is seted. \n
 *              \b CFALSE indicate that All pending is NOT seted.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    
 *              MES_TIMER_ClearInterruptPendingAll, MES_TIMER_GetInterruptPendingNumber
 */
CBOOL   MES_TIMER_GetInterruptPendingAll( void )
{
    const   U32 INTPEND_POS = 5;

    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( ( __g_pRegister->TMRCONTROL >> INTPEND_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *                                                  MES_TIMER_GetInterruptPendingNumber
 */
void    MES_TIMER_ClearInterruptPendingAll( void )
{
    const   U32 INTPEND_POS  = 5;

    MES_ASSERT( CNULL != __g_pRegister );
  	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    __g_pRegister->TMRCONTROL |= 1UL << INTPEND_POS;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_TIMER_GetInterruptNumber,       MES_TIMER_SetInterruptEnable,
 *              MES_TIMER_GetInterruptEnable,       MES_TIMER_GetInterruptPending,
 *              MES_TIMER_ClearInterruptPending,    MES_TIMER_SetInterruptEnableAll,
 *              MES_TIMER_GetInterruptEnableAll,    MES_TIMER_GetInterruptPendingAll,
 *              MES_TIMER_ClearInterruptPendingAll 
 */
S32     MES_TIMER_GetInterruptPendingNumber( void )  // -1 if None
{
    const   U32 INTPEND_POS  = 5;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->TMRCONTROL &  (1UL << INTPEND_POS) )
    {
        return 0;       // Timer module have only ONE pending bit. so return 0.
    }

    return -1;
}

//------------------------------------------------------------------------------
// Clock Control Interface

//------------------------------------------------------------------------------
/**
 *  @brief      Set a PCLK mode
 *  @param[in]  mode    PCLK mode
 *  @return     None.
 *  @see                                            MES_TIMER_GetClockPClkMode,
 *              MES_TIMER_SetClockDivisorEnable,    MES_TIMER_GetClockDivisorEnable,
 *              MES_TIMER_SetClockSource,           MES_TIMER_GetClockSource,
 *              MES_TIMER_SetClockDivisor,          MES_TIMER_GetClockDivisor
 */
void            MES_TIMER_SetClockPClkMode( MES_PCLKMODE mode )
{
	U32 clkmode=0;
    const U32 TCLKMODE_POS  =   1UL << 3;
    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	switch(mode)
	{
    	case MES_PCLKMODE_DYNAMIC:  clkmode = 0;		break;
    	case MES_PCLKMODE_ALWAYS:  	clkmode = 1;		break;
    	default: MES_ASSERT( CFALSE );
	}
	
    regvalue = pRegister->TMRCLKENB;

    regvalue &= ~(1UL<<TCLKMODE_POS);
    regvalue |= ( clkmode & 0x01 ) << TCLKMODE_POS;

    pRegister->TMRCLKENB = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get current PCLK mode
 *  @return     Current PCLK mode
 *  @see        MES_TIMER_SetClockPClkMode,         
 *              MES_TIMER_SetClockDivisorEnable,    MES_TIMER_GetClockDivisorEnable,
 *              MES_TIMER_SetClockSource,           MES_TIMER_GetClockSource,
 *              MES_TIMER_SetClockDivisor,          MES_TIMER_GetClockDivisor
 */
MES_PCLKMODE    MES_TIMER_GetClockPClkMode( void )
{
    const U32 TCLKMODE_POS  = 3;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->TMRCLKENB & ( 1UL << TCLKMODE_POS ) )
    {
        return MES_PCLKMODE_ALWAYS;
    }
    
    return  MES_PCLKMODE_DYNAMIC;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a clock generator to be enabled or disabled.
 *  @param[in]  Enable  \b CTRUE indicate that Clock generator enable. \n
 *                      \b CFALSE indicate that Clock generator disable. 
 *  @return     None.
 *  @see        MES_TIMER_SetClockPClkMode,         MES_TIMER_GetClockPClkMode,
 *                                                  MES_TIMER_GetClockDivisorEnable,
 *              MES_TIMER_SetClockSource,           MES_TIMER_GetClockSource,
 *              MES_TIMER_SetClockDivisor,          MES_TIMER_GetClockDivisor
 */
void            MES_TIMER_SetClockDivisorEnable( CBOOL Enable )
{
    const U32 CLKGENENB_POS  = 2;
    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    regvalue = pRegister->TMRCLKENB;

    regvalue &= ~(1UL        << CLKGENENB_POS);
    regvalue |=  (U32)Enable << CLKGENENB_POS;

    pRegister->TMRCLKENB = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a clock generator is enabled or not.
 *  @return     \b CTRUE indicate that Clock generator is enabled. \n
 *              \b CFALSE indicate that Clock generator is disabled. 
 *  @see        MES_TIMER_SetClockPClkMode,         MES_TIMER_GetClockPClkMode,
 *              MES_TIMER_SetClockDivisorEnable,    
 *              MES_TIMER_SetClockSource,           MES_TIMER_GetClockSource,
 *              MES_TIMER_SetClockDivisor,          MES_TIMER_GetClockDivisor
 */
CBOOL           MES_TIMER_GetClockDivisorEnable( void )
{
    const U32 CLKGENENB_POS  = 2;
    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( ( __g_pRegister->TMRCLKENB >> CLKGENENB_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a clock source for clock generator.
 *  @param[in]  Index       Select clock generator
 *  @param[in]  ClkSrc      Select clock source ( 0 (PLL0), 1 (PLL1) )
 *  @return     None.
 *  @remarks    Timer module have only ONE clock generator. so Index is always setting to 0.
 *  @see        MES_TIMER_SetClockPClkMode,         MES_TIMER_GetClockPClkMode,
 *              MES_TIMER_SetClockDivisorEnable,    MES_TIMER_GetClockDivisorEnable,
 *                                                  MES_TIMER_GetClockSource,
 *              MES_TIMER_SetClockDivisor,          MES_TIMER_GetClockDivisor
 */
void            MES_TIMER_SetClockSource( U32 Index, U32 ClkSrc )
{
    const U32   CLKSRCSEL_POS = 1;
    const U32   CLKSRCSEL_MASK = 0x7;
    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( 1 >= ClkSrc );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    regvalue = pRegister->TMRCLKGEN;

    regvalue &= ~(CLKSRCSEL_MASK << CLKSRCSEL_POS);
    regvalue |= ( ClkSrc << CLKSRCSEL_POS );

    pRegister->TMRCLKGEN = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get current clock source for clock generator.
 *  @param[in]  Index       Select clock generator .
 *  @return     Clock source of clock generator( 0 (PLL0), 1 (PLL1) ).
 *  @remarks    Timer module have only ONE clock generator. so Index is always setting to 0.
 *  @see        MES_TIMER_SetClockPClkMode,         MES_TIMER_GetClockPClkMode,
 *              MES_TIMER_SetClockDivisorEnable,    MES_TIMER_GetClockDivisorEnable,
 *              MES_TIMER_SetClockSource,           
 *              MES_TIMER_SetClockDivisor,          MES_TIMER_GetClockDivisor
 */
U32             MES_TIMER_GetClockSource( U32 Index )
{
    const U32   CLKSRCSEL_POS  = 1;
    const U32   CLKSRCSEL_MASK = 0x7;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( CNULL != __g_pRegister );

    return (U32)(( __g_pRegister->TMRCLKGEN >> CLKSRCSEL_POS ) & CLKSRCSEL_MASK );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a clock divider for clock generator.
 *  @param[in]  Index       Select clock generator.
 *  @param[in]  Divisor     Divide value of clock generator( 1 ~ 256 ).
 *  @return     None.
 *  @remarks    Timer module have only ONE clock generator. so Index is always setting to 0.
 *  @see        MES_TIMER_SetClockPClkMode,         MES_TIMER_GetClockPClkMode,
 *              MES_TIMER_SetClockDivisorEnable,    MES_TIMER_GetClockDivisorEnable,
 *              MES_TIMER_SetClockSource,           MES_TIMER_GetClockSource,
 *                                                  MES_TIMER_GetClockDivisor
 */
void            MES_TIMER_SetClockDivisor( U32 Index, U32 Divisor )
{
    const U32 CLKDIV_POS   = 4;
    const U32 CLKDIV_MASK  = 0xFF;

    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( (1 <= Divisor) && ( 256 >= Divisor ) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;
    regvalue = pRegister->TMRCLKGEN;

    regvalue &= ~(CLKDIV_MASK << CLKDIV_POS);
    regvalue |= (Divisor-1) << CLKDIV_POS;

    pRegister->TMRCLKGEN = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get current clock divider of clock generator
 *  @param[in]  Index       Select clock generator.
 *  @return     Clock divider value of current clock generator.
 *  @remarks    Timer module have only ONE clock generator. so Index is always setting to 0.
 *  @see        MES_TIMER_SetClockPClkMode,         MES_TIMER_GetClockPClkMode,
 *              MES_TIMER_SetClockDivisorEnable,    MES_TIMER_GetClockDivisorEnable,
 *              MES_TIMER_SetClockSource,           MES_TIMER_GetClockSource,
 *              MES_TIMER_SetClockDivisor          
 */
U32             MES_TIMER_GetClockDivisor( U32 Index )
{
    const U32 CLKDIV_POS   = 4;
    const U32 CLKDIV_MASK  = 0xFF;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( CNULL != __g_pRegister );

    return (U32)(((__g_pRegister->TMRCLKGEN >> CLKDIV_POS ) & CLKDIV_MASK ) +1 ) ;
}

//------------------------------------------------------------------------------
// TIMER Operation.

//------------------------------------------------------------------------------
/**
 *  @brief	Run the timer operation.
 *  @return	None.
 *	@see	                                MES_TIMER_Stop,
 *          MES_TIMER_SetWatchDogEnable,    MES_TIMER_SetTClkDivider,
 *          MES_TIMER_SetMatchCounter,      MES_TIMER_GetMatchCounter,
 *          MES_TIMER_SetTimerCounter,      MES_TIMER_GetTimerCounter 
 */
void MES_TIMER_Run( void )
{
	const U32 INTPEND	= (1UL<<5);
	const U32 RUN		= (1UL<<3);

    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;

	regvalue = pRegister->TMRCONTROL;

	regvalue &= ~INTPEND;
	regvalue |=  RUN;

	pRegister->TMRCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Stop the timer operation.
 *  @return	None.
 *	@see	MES_TIMER_Run,                  
 *          MES_TIMER_SetWatchDogEnable,    MES_TIMER_SetTClkDivider,
 *          MES_TIMER_SetMatchCounter,      MES_TIMER_GetMatchCounter,
 *          MES_TIMER_SetTimerCounter,      MES_TIMER_GetTimerCounter 
 */
void MES_TIMER_Stop( void )
{
	const U32 INTPEND	= (1UL<<5);
	const U32 RUN		= (1UL<<3);

    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;

	regvalue = pRegister->TMRCONTROL;

	regvalue &= ~(INTPEND | RUN);

	pRegister->TMRCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Enable/Disable the watchdog function.
 *  @param[in]  Enable    Determines whether the timer is used for watchdog function.
 *	@see	    MES_TIMER_Run,                  MES_TIMER_Stop,
 *                                              MES_TIMER_SetTClkDivider,
 *              MES_TIMER_SetMatchCounter,      MES_TIMER_GetMatchCounter,
 *              MES_TIMER_SetTimerCounter,      MES_TIMER_GetTimerCounter 
 */
void MES_TIMER_SetWatchDogEnable( CBOOL Enable )
{
	const U32 INTPEND	= (1UL<<5);
	const U32 WDENB		= (1UL<<2);

    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

	MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( (0==Enable) || (1==Enable) );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;

	regvalue = pRegister->TMRCONTROL;

	regvalue &= ~(INTPEND|WDENB);

	if( Enable )	regvalue |= WDENB;

	pRegister->TMRCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Selects a internal clock divider for a timer clock.
 *  @param[in]  Clock    selects a TCLK to be used.
 *	@see	    MES_TIMER_Run,                  MES_TIMER_Stop,
 *              MES_TIMER_SetWatchDogEnable,    
 *              MES_TIMER_SetMatchCounter,      MES_TIMER_GetMatchCounter,
 *              MES_TIMER_SetTimerCounter,      MES_TIMER_GetTimerCounter 
 */
void MES_TIMER_SetTClkDivider( MES_TIMER_CLOCK Clock )
{

	const U32 INTPEND	= (1UL<<5);
	const U32 SELTCLK	= (3UL<<0);

    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

	MES_ASSERT( 4 > Clock );
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;
	regvalue = pRegister->TMRCONTROL;

	regvalue &= ~(INTPEND | SELTCLK);
	regvalue |=  (U32)Clock;

	pRegister->TMRCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set new match counter value.
 *  @param[in]  MatchCounter    New match counter value.
 *  @return	    None.
 *	@see	    MES_TIMER_Run,                  MES_TIMER_Stop,
 *              MES_TIMER_SetWatchDogEnable,    MES_TIMER_SetTClkDivider,
 *                                              MES_TIMER_GetMatchCounter,
 *              MES_TIMER_SetTimerCounter,      MES_TIMER_GetTimerCounter 
 */
void MES_TIMER_SetMatchCounter( U32 MatchCounter )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

	__g_pRegister->TMRMATCH = MatchCounter;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a match counter value.
 *  @return	current match counter value.
 *	@see	MES_TIMER_Run,                  MES_TIMER_Stop,
 *          MES_TIMER_SetWatchDogEnable,    MES_TIMER_SetTClkDivider,
 *          MES_TIMER_SetMatchCounter,      
 *          MES_TIMER_SetTimerCounter,      MES_TIMER_GetTimerCounter 
 */
U32  MES_TIMER_GetMatchCounter( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return __g_pRegister->TMRMATCH;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set new timer counter value.
 *  @param[in]  TimerCounter    New timer counter value.
 *  @return	    None.
 *	@see	    MES_TIMER_Run,                  MES_TIMER_Stop,
 *              MES_TIMER_SetWatchDogEnable,    MES_TIMER_SetTClkDivider,
 *              MES_TIMER_SetMatchCounter,      MES_TIMER_GetMatchCounter,
 *                                              MES_TIMER_GetTimerCounter 
 */
void MES_TIMER_SetTimerCounter( U32 TimerCounter )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );
	__g_pRegister->TMRCOUNT = TimerCounter;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Get a timer counter value.
 *  @return	Current timer counter value.
 *	@see	MES_TIMER_Run,                  MES_TIMER_Stop,
 *          MES_TIMER_SetWatchDogEnable,    MES_TIMER_SetTClkDivider,
 *          MES_TIMER_SetMatchCounter,      MES_TIMER_GetMatchCounter,
 *          MES_TIMER_SetTimerCounter      
 */
U32  MES_TIMER_GetTimerCounter( void )
{
	const U32 INTPEND	= (1UL<<5);
	const U32 LDCNT		= (1UL<<6);

    register struct MES_TIMER_RegisterSet* pRegister;
    register U32 regvalue;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_TIMER_GetClockDivisorEnable() );

    pRegister = __g_pRegister;
	regvalue = pRegister->TMRCONTROL;

	regvalue &= ~INTPEND;
	regvalue |=  LDCNT;

	pRegister->TMRCONTROL = regvalue;

	return pRegister->TMRCOUNT;
}


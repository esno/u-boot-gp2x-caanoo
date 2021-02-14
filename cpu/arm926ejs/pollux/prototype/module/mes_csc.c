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
//	Module     : CSC
//	File       : mes_csc.c
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------

#include "mes_csc.h"

enum
{
	MES_CSC_ALIGN_WIDTH 	= 16UL,
	MES_CSC_MIN_WIDTH 		= 16UL,
	MES_CSC_MAX_WIDTH		= 1024UL,
	MES_CSC_ALIGN_HEIGHT 	= 16UL,
	MES_CSC_MIN_HEIGHT		= 16UL,
	MES_CSC_MAX_HEIGHT		= 1024UL
};

MES_DEBUG_CODE
(
    // Store widht and height to verify paramters of the function
    // SetSourceAddr and SetTargetAddr in debug build environment.
    U32		MES_CSC_dbg_dwWidth;
    U32		MES_CSC_dbg_dwHeight;
)

/// @brief  CSC Module's Register List
struct  MES_CSC_RegisterSet
{
	volatile U32 CONTROL;	        ///< 0x00 [RW]      : Control Register
	volatile U32 SIZE   ;	        ///< 0x04 [ W]      : Size Register
	volatile U32 ADDRY  ;	        ///< 0x08 [ W]      : Address Y Channel Register
	volatile U32 ADDRCB ;	        ///< 0x0C [ W]      : Address Cb Channel Register
	volatile U32 ADDRCR ;	        ///< 0x10 [ W]      : Address Cr Channel Register
	volatile U32 ADDRTEX;	        ///< 0x14 [ W]      : Address Texture Register
	volatile U32 TPCTRL ;	        ///< 0x18 [ W]      : Transparency Control Register
	volatile U32 TPY    ;	        ///< 0x1C [ W]      : Transparency Y Range Register
	volatile U32 TPCB   ;	        ///< 0x20 [ W]      : Transparency Cb Range Register
	volatile U32 TPCR   ;	        ///< 0x24 [ W]      : Transparency Cr Range Register
	volatile U32 RESERVED[0x1E6];   ///< 0x28 ~ 0x7BC   : Reserved Region
    volatile U32 CSCCLKENB;         ///< 0x7C0          : Clock Enable Register
};

static  struct
{
    struct MES_CSC_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_CSC_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_CSC_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see                                MES_CSC_SelectModule,
 *          MES_CSC_GetCurrentModule,   MES_CSC_GetNumberOfModule
 */
CBOOL   MES_CSC_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_CSC_MODULE; i++ )
		{
			__g_ModuleVariables[i].pRegister = CNULL;
		}
		
		bInit = CTRUE;
	}

	MES_DEBUG_CODE
	(
		MES_CSC_dbg_dwWidth = 0;
		MES_CSC_dbg_dwHeight = 0;
	)

    return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Select module to control
 *  @param[in]  ModuleIndex     Module index to select.
 *	@return		the index of previously selected module.
 *  @see        MES_CSC_Initialize,
 *              MES_CSC_GetCurrentModule,   MES_CSC_GetNumberOfModule
 */
U32    MES_CSC_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_CSC_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;

	return oldindex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_CSC_Initialize,         MES_CSC_SelectModule,
 *                                          MES_CSC_GetNumberOfModule
 */
U32     MES_CSC_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_CSC_Initialize,         MES_CSC_SelectModule,
 *              MES_CSC_GetCurrentModule
 */
U32     MES_CSC_GetNumberOfModule( void )
{
    return NUMBER_OF_CSC_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                        MES_CSC_GetSizeOfRegisterSet,
 *              MES_CSC_SetBaseAddress,         MES_CSC_GetBaseAddress,
 *              MES_CSC_OpenModule,             MES_CSC_CloseModule,
 *              MES_CSC_CheckBusy,              MES_CSC_CanPowerDown
 */
U32     MES_CSC_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_CSC_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_CSC_MODULE + (OFFSET_OF_CSC_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_CSC_GetPhysicalAddress,
 *              MES_CSC_SetBaseAddress,         MES_CSC_GetBaseAddress,
 *              MES_CSC_OpenModule,             MES_CSC_CloseModule,
 *              MES_CSC_CheckBusy,              MES_CSC_CanPowerDown
 */
U32     MES_CSC_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_CSC_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_CSC_GetPhysicalAddress,     MES_CSC_GetSizeOfRegisterSet,
 *                                              MES_CSC_GetBaseAddress,
 *              MES_CSC_OpenModule,             MES_CSC_CloseModule,
 *              MES_CSC_CheckBusy,              MES_CSC_CanPowerDown
 */
void    MES_CSC_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_CSC_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_CSC_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_CSC_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_CSC_GetPhysicalAddress,     MES_CSC_GetSizeOfRegisterSet,
 *              MES_CSC_SetBaseAddress,
 *              MES_CSC_OpenModule,             MES_CSC_CloseModule,
 *              MES_CSC_CheckBusy,              MES_CSC_CanPowerDown
 */
U32     MES_CSC_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_CSC_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_CSC_GetPhysicalAddress,     MES_CSC_GetSizeOfRegisterSet,
 *              MES_CSC_SetBaseAddress,         MES_CSC_GetBaseAddress,
 *                                              MES_CSC_CloseModule,
 *              MES_CSC_CheckBusy,              MES_CSC_CanPowerDown
 */
CBOOL   MES_CSC_OpenModule( void )
{
	MES_ASSERT(	!MES_CSC_CheckBusy() );
	MES_CSC_SetInterruptEnableAll( CFALSE );
	MES_CSC_ClearInterruptPendingAll();
	MES_CSC_SetTransparency( 0, CFALSE,
        					 CFALSE, 0, 255,
        					 CFALSE, 0, 255,
        					 CFALSE, 0, 255 );
	MES_CSC_SetDitherEnable( CTRUE );
	MES_CSC_SetQuantizationMode( MES_CSC_QMODE_256 );
	MES_ASSERT(	!MES_CSC_CheckBusy() );

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_CSC_GetPhysicalAddress,     MES_CSC_GetSizeOfRegisterSet,
 *              MES_CSC_SetBaseAddress,         MES_CSC_GetBaseAddress,
 *              MES_CSC_OpenModule,
 *              MES_CSC_CheckBusy,              MES_CSC_CanPowerDown
 */
CBOOL   MES_CSC_CloseModule( void )
{
	MES_ASSERT(	!MES_CSC_CheckBusy() );
	MES_CSC_SetInterruptEnableAll( CFALSE );
	MES_CSC_ClearInterruptPendingAll();
	MES_CSC_SetTransparency( 0, CFALSE,
        					 CFALSE, 0, 255,
        					 CFALSE, 0, 255,
        					 CFALSE, 0, 255 );
	MES_CSC_SetDitherEnable( CTRUE );
	MES_CSC_SetQuantizationMode( MES_CSC_QMODE_256 );
	MES_ASSERT(	!MES_CSC_CheckBusy() );

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_CSC_GetPhysicalAddress,     MES_CSC_GetSizeOfRegisterSet,
 *              MES_CSC_SetBaseAddress,         MES_CSC_GetBaseAddress,
 *              MES_CSC_OpenModule,             MES_CSC_CloseModule,
 *                                              MES_CSC_CanPowerDown
 */
CBOOL   MES_CSC_CheckBusy( void )
{
  	return MES_CSC_IsBusy();
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_CSC_GetPhysicalAddress,     MES_CSC_GetSizeOfRegisterSet,
 *              MES_CSC_SetBaseAddress,         MES_CSC_GetBaseAddress,
 *              MES_CSC_OpenModule,             MES_CSC_CloseModule,
 *              MES_CSC_CheckBusy
 */
CBOOL   MES_CSC_CanPowerDown( void )
{
    if( MES_CSC_IsBusy() )
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
 *  @see                                            MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
S32     MES_CSC_GetInterruptNumber( void )
{
    return  INTNUM_OF_CSC_MODULE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number ( 0 ).
 *  @param[in]  Enable  \b CTRUE   indicate that Interrupt Enable. \n
 *                      \b CFALSE  indicate that Interrupt Disable.
 *  @return     None.
 *  @remarks    CSC Module Only have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_CSC_GetInterruptNumber,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
void    MES_CSC_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    const U32 INTENB_POS    = 1;
	const U32 DITHER 		= 1UL<<5;
	const U32 QMODE_MASK 	= 1UL<<4;


    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   = __g_pRegister;
    regvalue    = pRegister->CONTROL;

    regvalue    &= ( DITHER | QMODE_MASK );
    regvalue    |= ( (U32)Enable << INTENB_POS );

    pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number ( 0 ).
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSE indicate that Interrupt is disabled.
 *  @remarks    CSC Module Only have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *                                                  MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
CBOOL   MES_CSC_GetInterruptEnable( S32 IntNum )
{
    const U32 INTENB_MASK   = (1UL << 1);
    const U32 INTENB_POS    = 1;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( (__g_pRegister->CONTROL & INTENB_MASK) >> INTENB_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number ( 0 ).
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @remarks    CSC Module Only have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
CBOOL   MES_CSC_GetInterruptPending( S32 IntNum )
{
    const U32 INTPEND_MASK  = (1UL << 2);
    const U32 INTPEND_POS   = 2;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( (__g_pRegister->CONTROL & INTPEND_MASK) >> INTPEND_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt number ( 0 ).
 *  @return     None.
 *  @remarks    CSC Module Only have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *                                                  MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
void    MES_CSC_ClearInterruptPending( S32 IntNum )
{
	const U32 DITHER 		= 1UL<<5;
	const U32 QMODE_MASK 	= 1UL<<4;
	const U32 INTENB 		= 1UL<<1;
    const U32 INTPEND_POS   = 2;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

    regvalue    = pRegister->CONTROL;

    regvalue    &= ( DITHER | QMODE_MASK | INTENB );

    regvalue    |= ( 1UL << INTPEND_POS );

    pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable.  \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
void    MES_CSC_SetInterruptEnableAll( CBOOL Enable )
{
    const U32 INTENB_POS    = 1;
	const U32 DITHER 		= 1UL<<5;
	const U32 QMODE_MASK 	= 1UL<<4;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   = __g_pRegister;

    regvalue    = pRegister->CONTROL;

    regvalue    &= ( DITHER | QMODE_MASK );
    regvalue    |= ( (U32)Enable << INTENB_POS );

    pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *                                                  MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
CBOOL   MES_CSC_GetInterruptEnableAll( void )
{
    const U32 INTENB_MASK   = (1UL << 1);
    const U32 INTENB_POS    = 1;

    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( (__g_pRegister->CONTROL & INTENB_MASK) >> INTENB_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are pended or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) pending is seted. \n
 *              \b CFALSE indicate that All pending is NOT seted.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,
 *              MES_CSC_ClearInterruptPendingAll,   MES_CSC_GetInterruptPendingNumber
 */
CBOOL   MES_CSC_GetInterruptPendingAll( void )
{
    const U32 INTPEND_MASK  = (1UL << 2);
    const U32 INTPEND_POS   = 2;

    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( (__g_pRegister->CONTROL & INTPEND_MASK) >> INTPEND_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *                                                  MES_CSC_GetInterruptPendingNumber
 */
void    MES_CSC_ClearInterruptPendingAll( void )
{

	const U32 DITHER 		= 1UL<<5;
	const U32 QMODE_MASK 	= 1UL<<4;
	const U32 INTENB 		= 1UL<<1;

    const U32 INTPEND_POS   = 2;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

    regvalue    = pRegister->CONTROL;

    regvalue    &= ( DITHER | QMODE_MASK | INTENB );

    regvalue    |= ( 1UL << INTPEND_POS );

    pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_CSC_GetInterruptNumber,         MES_CSC_SetInterruptEnable,
 *              MES_CSC_GetInterruptEnable,         MES_CSC_GetInterruptPending,
 *              MES_CSC_ClearInterruptPending,      MES_CSC_SetInterruptEnableAll,
 *              MES_CSC_GetInterruptEnableAll,      MES_CSC_GetInterruptPendingAll,
 *              MES_CSC_ClearInterruptPendingAll
 */
S32     MES_CSC_GetInterruptPendingNumber( void )  // -1 if None
{
    const U32 INTPEND_MASK  = (1UL << 2);

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->CONTROL & INTPEND_MASK )
    {
        return 0;
    }

    return -1;
}

//------------------------------------------------------------------------------
// Clock Control Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Set a PCLK mode
 *  @param[in]  mode    PCLK mode
 *  @return     None.
 *  @see                                        MES_CSC_GetClockPClkMode,
 *              MES_CSC_SetClockBClkMode,       MES_CSC_GetClockBClkMode
 */
void            MES_CSC_SetClockPClkMode( MES_PCLKMODE mode )
{
    const U32 PCLKMODE_POS  =   3;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

	U32 clkmode=0;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	switch(mode)
	{
    	case MES_PCLKMODE_DYNAMIC:  clkmode = 0;		break;
    	case MES_PCLKMODE_ALWAYS:  	clkmode = 1;		break;
    	default: MES_ASSERT( CFALSE );
	}

    regvalue = pRegister->CSCCLKENB;

    regvalue &= ~(1UL<<PCLKMODE_POS);
    regvalue |= ( clkmode & 0x01 ) << PCLKMODE_POS;

    pRegister->CSCCLKENB = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get current PCLK mode
 *  @return     Current PCLK mode
 *  @see        MES_CSC_SetClockPClkMode,
 *              MES_CSC_SetClockBClkMode,       MES_CSC_GetClockBClkMode
 */
MES_PCLKMODE    MES_CSC_GetClockPClkMode( void )
{
    const U32 PCLKMODE_POS  = 3;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->CSCCLKENB & ( 1UL << PCLKMODE_POS ) )
    {
        return MES_PCLKMODE_ALWAYS;
    }

    return  MES_PCLKMODE_DYNAMIC;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set System Bus Clock's operation Mode
 *  @param[in]  mode     BCLK Mode
 *  @return     None.
 *  @see        MES_CSC_SetClockPClkMode,       MES_CSC_GetClockPClkMode,
 *                                              MES_CSC_GetClockBClkMode
 */
void            MES_CSC_SetClockBClkMode( MES_BCLKMODE mode )
{
    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;
	U32 clkmode=0;

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	switch(mode)
	{
    	case MES_BCLKMODE_DISABLE: 	clkmode = 0;		break;
    	case MES_BCLKMODE_DYNAMIC:	clkmode = 2;		break;
    	case MES_BCLKMODE_ALWAYS:	clkmode = 3;		break;
    	default: MES_ASSERT( CFALSE );
	}

	regvalue = pRegister->CSCCLKENB;
	regvalue &= ~(0x3);
	regvalue |= clkmode & 0x3;
	pRegister->CSCCLKENB = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get System Bus Clock's operation Mode
 *  @return     BCLK Mode
 *  @see        MES_CSC_SetClockPClkMode,       MES_CSC_GetClockPClkMode,
 *              MES_CSC_SetClockBClkMode
 */
MES_BCLKMODE    MES_CSC_GetClockBClkMode( void )
{
	const U32 BCLKMODE	= 3UL<<0;

	MES_ASSERT( CNULL != __g_pRegister );

	switch( __g_pRegister->CSCCLKENB & BCLKMODE )
	{
    	case 0 :	return MES_BCLKMODE_DISABLE;
    	case 2 :	return MES_BCLKMODE_DYNAMIC;
    	case 3 :	return MES_BCLKMODE_ALWAYS;
	}

	MES_ASSERT( CFALSE );
	return MES_BCLKMODE_DISABLE;
}

//------------------------------------------------------------------------------
// CSC Operation.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *	@brief	    Specifies whether to apply the dithering.
 *	@param[in]	bEnb	Set it to CTRUE to apply the dithering.
 *	@return	    None.
 *	@remarks    The dithering is useful method for case which is that the color
 *			    depth of destination is less than one of source. It's recommeded
 *			    to apply the dithering because the texture has RGB565 format.
 *	@see	                                    MES_CSC_GetDitherEnable,
 *              MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *              MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *              MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *              MES_CSC_Run,                    MES_CSC_IsBusy
 */
void	        MES_CSC_SetDitherEnable( CBOOL bEnb )
{
	const U32 DITHER_POS	= 5;
	const U32 QMODE_MASK 	= 1UL<<4;
	const U32 INTENB 		= 1UL<<1;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );	// You have to ensure the CSC is idle.

    pRegister = __g_pRegister;

	regvalue = pRegister->CONTROL;

	regvalue &= (QMODE_MASK | INTENB);

    regvalue |= ( (U32)bEnb << DITHER_POS );

	pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *	@brief	Informs whether to enable the ditheing.
 *	@return \b CTRUE indicates the dithering is applied.\n
 *			\b CFALSE indicates the dithering is not applied.
 *	@see	MES_CSC_SetDitherEnable,        
 *          MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *          MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *          MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *          MES_CSC_Run,                    MES_CSC_IsBusy
 */
CBOOL	        MES_CSC_GetDitherEnable( void )
{
	const U32 DITHER 	= 1UL<<5;
    const U32 DITHER_POS= 5;

	MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( (__g_pRegister->CONTROL & DITHER) >> DITHER_POS );
}

//------------------------------------------------------------------------------
/**
 *	@brief	Set the quantization mode.
 *	@param[in]	mode	specifies the type of quantization mode.
 *	@return None.
 *	@remark	The following fomula is used to convert YCbCr to RGB.\n
 *			for 220 quantization mode,
 *			- R = Y + 1.375 * Cr
 *			- G = Y - 0.3359375 * Cb - 0.6953125 * Cr
 *			- B = Y + 1.734375 * Cb
 *			.
 *			for 256 quantization mode,
 *     		- R = Y + 1.4020 * Cr
 *			- G = Y - 0.34414 * Cb - 0.71414 * Cr
 *			- B = Y + 1.7720 * Cb
 *	@see	MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *                                          MES_CSC_GetQuantizationMode,
 *          MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *          MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *          MES_CSC_Run,                    MES_CSC_IsBusy
 */
void	        MES_CSC_SetQuantizationMode( MES_CSC_QMODE mode )
{
	const U32 DITHER 		= 1UL<<5;
	const U32 INTENB 		= 1UL<<1;
	const U32 QMODE_POS		= 4;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 2 > mode );
	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );	// You have to ensure the CSC is idle.

    pRegister = __g_pRegister;

	regvalue = pRegister->CONTROL;
	regvalue &= (DITHER | INTENB);
	regvalue |= ((U32)mode)<<QMODE_POS;
	pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *	@brief	Get the quantization mode.
 *	@return the current quantization mode.
 *	@see	MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *          MES_CSC_SetQuantizationMode,    
 *          MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *          MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *          MES_CSC_Run,                    MES_CSC_IsBusy
 */
MES_CSC_QMODE	MES_CSC_GetQuantizationMode( void )
{
	const U32 QMODE_POS		= 4;
	const U32 QMODE_MASK 	= 1UL<<QMODE_POS;

	MES_ASSERT( CNULL != __g_pRegister );

	return (MES_CSC_QMODE)((__g_pRegister->CONTROL & QMODE_MASK)>>QMODE_POS);
}

//------------------------------------------------------------------------------
/**
 *	@brief	Set configutaion for transparency function.
 *	@param[in]	ReplaceColor	the color which has RGB565 format is used to replace
 *								the result color of CSC in case satisfying the
 *								condition.
 *	@param[in]	bEnb			Specifies whether to test Y/Cb/Cr channel for transparency
 *	@param[in]	bInvY			Specifies whether to invert color test range for Y channel.
 *	@param[in]	MinY			the minimum value of Y channel.
 *	@param[in]	MaxY			the maximum value of Y channel.
 *	@param[in]	bInvCb			Specifies whether to invert color test range for Cb channel.
 *	@param[in]	MinCb			the minimum value of Cb channel.
 *	@param[in]	MaxCb			the maximum value of Cb channel.
 *	@param[in]	bInvCr			Specifies whether to invert color test range for Cr channel.
 *	@param[in]	MinCr			the minimum value of Cr channel.
 *	@param[in]	MaxCr			the maximum value of Cr channel.
 *	@return	None.
 *	@remark	The result color of CSC can be replaced by specified color when
 * 			the each component is in individually specified range. The condition
 * 			is satisfied when the component is in between minimum value and
 * 			maximum value. and you can also invert this	condition.
 *	@see	MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *          MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *                                          MES_CSC_SetSize,
 *          MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *          MES_CSC_Run,                    MES_CSC_IsBusy
 */
void	        MES_CSC_SetTransparency( U16 ReplaceColor, CBOOL bEnb,
        		                		 CBOOL bInvY,  U8 MinY,  U8 MaxY,
        		                		 CBOOL bInvCb, U8 MinCb, U8 MaxCb,
        		                		 CBOOL bInvCr, U8 MinCr, U8 MaxCr )
{
    const U32 TPENB_POS   = 19;
    const U32 TPINVCR_POS = 18;
    const U32 TPINVCB_POS = 17;
    const U32 TPINVY_POS  = 16;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

    MES_ASSERT( CNULL != __g_pRegister );
   	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );

    pRegister   =   __g_pRegister;

    regvalue    = 0;
    regvalue    = (U32)ReplaceColor;
    regvalue    |= ( ((bEnb<<(TPENB_POS+2)) | (bEnb<<(TPENB_POS+1)) | (bEnb<<(TPENB_POS+0)))
                | (bInvCr<<TPINVCR_POS) | (bInvCb<<TPINVCB_POS) | (bInvY<<TPINVY_POS) );

    pRegister->TPCTRL = regvalue;
	pRegister->TPY    = ((U32)MaxY  << 8) | ((U32)MinY);
	pRegister->TPCB   = ((U32)MaxCb << 8) | ((U32)MinCb);
	pRegister->TPCR   = ((U32)MaxCr << 8) | ((U32)MinCr);

}

//------------------------------------------------------------------------------
/**
 *	@brief	Specifes the size of the image to convert.
 *	@param[in] 	width	the image width in pixels, 16 ~ 1024.
 *	@param[in] 	height	the image height in pixels, 16 ~ 1024.
 *	@return	None.
 *	@remark	The result of this function is used to verify parameters of the
 *			function MES_CSC_SetSourceAddr() and MES_CSC_SetTargetAddr() in debug build
 * 			environment. Therefore you have to set relevant image size once
 *			at least before	calling the function MES_CSC_SetSourceAddr() or
 *			MES_CSC_SetTargetAddr().
 *	@see	    MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *              MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *              MES_CSC_SetTransparency,        
 *              MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *              MES_CSC_Run,                    MES_CSC_IsBusy
 */
void	MES_CSC_SetSize( U32 width, U32 height )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 0 == (width % MES_CSC_ALIGN_WIDTH) );
	MES_ASSERT( 0 == (height % MES_CSC_ALIGN_HEIGHT) );
	MES_ASSERT( MES_CSC_MIN_WIDTH <= width && width <= MES_CSC_MAX_WIDTH );
	MES_ASSERT( MES_CSC_MIN_HEIGHT <= height && height <= MES_CSC_MAX_HEIGHT );
	// You have to ensure the CSC is idle.
	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );

	MES_DEBUG_CODE
	(
		MES_CSC_dbg_dwWidth = width;
		MES_CSC_dbg_dwHeight = height;
	)


	__g_pRegister->SIZE = ((height - 1) << 16) | (width - 1);
}

//------------------------------------------------------------------------------
/**
 *	@brief	Specifies addresses of the source iamge to convert.
 *	@param[in] 	AddrY		the Y channel address which has 2D block addressing format.
 *	@param[in] 	AddrCb		the Cb channel address which has 2D block addressing format.
 *	@param[in]	AddrCr		the Cr channel address which has 2D block addressing format.
 *	@return	None.
 *	@remark	The 2D block addressing format is as follwings.
 *			- Addr[31:24] specifies the index of segment.
 *			- Addr[23:12] specifies the y-coordinate of upper-left corner of the
 *			  image in segment.
 *			- Addr[11: 0] specifies the x-coordinate of upper-left corner of the
 * 			  image in segment.
 *			.
 *			There's some restriction for the address of CSC and you should set
 *			the address carefully. The address for source image has following
 *			restrictions.
 *			- The x-coordinate of the image must be mulitple of 2^n, where
 *			  2^n is greater than or equal to the image width in pixels.
 *			  The image width for Cb, Cr has half of the image width for Y.
 *			- The y-coordinate of the image must be multiple of 2^n, where
 *			  2^n is greater than or equal to the image height in pixels.
 *			  The image height for Cb, Cr has half of the image height for Y.
 *			- The whole image must be in a segment which has 2048x1024 region.
 *			.
 *			If the image size is modified or you did not set the relevant image
 *			size yet, you have to call the function MES_CSC_SetSize() before calling
 * 			this function to verify paramters of this function in debug build
 * 			environment.
 *	@see	    MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *              MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *              MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *                                              MES_CSC_SetTargetAddr,
 *              MES_CSC_Run,                    MES_CSC_IsBusy
 */
void	MES_CSC_SetSourceAddr( U32 AddrY, U32 AddrCb, U32 AddrCr )
{
	MES_DEBUG_CODE
	(
		U32 x;
		U32 y;
		U32 width2n;
		U32 height2n;
    )

	// You have to set the size of image before this function is called.
	MES_ASSERT( MES_CSC_MIN_WIDTH  <= MES_CSC_dbg_dwWidth );
	MES_ASSERT( MES_CSC_MIN_HEIGHT <= MES_CSC_dbg_dwHeight );

	MES_DEBUG_CODE
	(
		// Find minimum 2^n which is greater than or equal to the image width / height
		for( width2n = 16; width2n < MES_CSC_dbg_dwWidth; width2n <<= 1 ){  }
		for( height2n = 16; height2n < MES_CSC_dbg_dwHeight; height2n <<= 1 ){  }
	)
	//--------------------------------------------------------------------------
	// Verify Y address
	MES_DEBUG_CODE
	(
		x = AddrY & 0x000007FFUL;
		y = (AddrY >> 12) & 0x3FFUL;
	)
	// x & y must be multiple of 2^n
	MES_ASSERT( 0 == (x % width2n) );
	MES_ASSERT( 0 == (y % height2n) );
	// The whole image must be in a segment.
	MES_ASSERT( (MES_CSC_MAX_WIDTH*2) >= (x + MES_CSC_dbg_dwWidth) );
	MES_ASSERT( MES_CSC_MAX_HEIGHT    >= (y + MES_CSC_dbg_dwHeight) );
	//--------------------------------------------------------------------------
	// Verfify Cb address
	MES_DEBUG_CODE
	(
		x = AddrCb & 0x000007FFUL;
		y = (AddrCb >> 12) & 0x3FFUL;
	)
	// x & y must be multiple of 2^n
	MES_ASSERT( 0 == (x % (width2n /2)) );
	MES_ASSERT( 0 == (y % (height2n/2)) );
	// The whole image must be in a segment.
	MES_ASSERT( (MES_CSC_MAX_WIDTH*2) >= (x + (MES_CSC_dbg_dwWidth /2)) );
	MES_ASSERT( MES_CSC_MAX_HEIGHT    >= (y + (MES_CSC_dbg_dwHeight/2)) );
	//--------------------------------------------------------------------------
	// Verify Cr address
	MES_DEBUG_CODE
	(
		x = AddrCr & 0x000007FFUL;
		y = (AddrCr >> 12) & 0x3FFUL;
	)
	// x & y must be multiple of 2^n
	MES_ASSERT( 0 == (x % (width2n /2)) );
	MES_ASSERT( 0 == (y % (height2n/2)) );
	// the whole image must be in a segment.
	MES_ASSERT( (MES_CSC_MAX_WIDTH*2) >= (x + (MES_CSC_dbg_dwWidth /2)) );
	MES_ASSERT( MES_CSC_MAX_HEIGHT    >= (y + (MES_CSC_dbg_dwHeight/2)) );

	// You have to ensure the CSC is idle.
	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->ADDRY	= (AddrY & 0xFFC007FF) 			// Segment[10:1] & X[10:0]
    						| ((AddrY & 0x00000800)<<10)	// Segment[0]
    						| ((AddrY & 0x003FF000)>>1);	// Y[9:0]
	__g_pRegister->ADDRCB	= (AddrCb & 0xFFC007FF) 		// Segment[10:1] & X[10:0]
    						| ((AddrCb & 0x00000800)<<10)	// Segment[0]
    						| ((AddrCb & 0x003FF000)>>1);	// Y[9:0]
	__g_pRegister->ADDRCR	= (AddrCr & 0xFFC007FF) 		// Segment[10:1] & X[10:0]
    						| ((AddrCr & 0x00000800)<<10)	// Segment[0]
    						| ((AddrCr & 0x003FF000)>>1);	// Y[9:0]
}

//------------------------------------------------------------------------------
/**
 *	@brief	Specifies the texture address to be written.
 *	@param[in] 	AddrTex		the texture address which has 2D block addressing format.
 *	@return	None.
 *	@remark	The 2D block addressing format is as follwings.
 *			- Addr[31:24] specifies the index of segment.
 *			- Addr[23:12] specifies the y-coordinate of upper-left corner of the
 *			  image in segment.
 *			- Addr[11: 0] specifies the x-coordinate of upper-left corner of the
 * 			  image in segment.
 *			.
 *			There's some restriction for the address of CSC and you should set
 *			the address carefully. The address for texture image has following
 *			restrictions.
 *			- The x-coordinate of texture image must be mulitple of 2^n, where
 *			  2^n is greater than or equal to double of the image width in
 *			  pixels.
 *			- The y-coordinate of texture image must be multiple of 2^n, where
 *			  2^n is greater than or equal to the image height in pixels.
 *			- The segment of texture image must be in the normal
 *			  addressing region.
 *			- The whole image must be in a segment which has 2048x1024 region.
 *			.
 *			If the image size is modified or you did not set the relevant image
 *			size yet, you have to call the function MES_CSC_SetSize() before calling
 * 			this function to verify paramters of this function in debug build
 * 			environment.
 *	@see	    MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *              MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *              MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *              MES_CSC_SetSourceAddr,          
 *              MES_CSC_Run,                    MES_CSC_IsBusy
 */
void	MES_CSC_SetTargetAddr( U32 AddrTex )
{
    MES_DEBUG_CODE
    (
		U32 x;
		U32 y;
		U32 width2n;
		U32 height2n;
    )

	// You have to set the size of image before this function is called.
	MES_ASSERT( MES_CSC_MIN_WIDTH  <= MES_CSC_dbg_dwWidth );
	MES_ASSERT( MES_CSC_MIN_HEIGHT <= MES_CSC_dbg_dwHeight );
	MES_DEBUG_CODE
	(
		// Find minimum 2^n which is greater than or equal to the image width / height
		for( width2n = 16; width2n < MES_CSC_dbg_dwWidth; width2n <<= 1 ){  }
		for( height2n = 16; height2n < MES_CSC_dbg_dwHeight; height2n <<= 1 ){  }
	)
	MES_DEBUG_CODE
	(
		x = AddrTex & 0x000007FFUL;
		y = (AddrTex >> 12) & 0x3FFUL;
	)
	// x & y must be multiple of 2^n
	MES_ASSERT( 0 == (x % (width2n*2)) );
	MES_ASSERT( 0 == (y % height2n) );
	// The whole image must be in a segment.
	MES_ASSERT( (MES_CSC_MAX_WIDTH*2) >= (x + (MES_CSC_dbg_dwWidth*2)) );
	MES_ASSERT( MES_CSC_MAX_HEIGHT    >= (y + MES_CSC_dbg_dwHeight) );
	// The segment must be in the normal addressing region.
	MES_ASSERT( 0 == (AddrTex & 0x30000000) );

	// You have to ensure the CSC is idle.
	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->ADDRTEX	= (AddrTex & 0xFFC007FF) 		// Segment[10:1] & X[10:0]
							| ((AddrTex & 0x00000800)<<10)	// Segment[0]
							| ((AddrTex & 0x003FF000)>>1);	// Y[9:0]
}

//------------------------------------------------------------------------------
/**
 *	@brief	Run CSC to perform a color space conversion.
 *	@return	None.
 *	@see	    MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *              MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *              MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *              MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *                                              MES_CSC_IsBusy
 */
void	MES_CSC_Run( void )
{
	const U32 DITHER 		= 1UL<<5;
	const U32 QMODE_MASK 	= 1UL<<4;
	const U32 INTENB 		= 1UL<<1;
	const U32 RUN			= 1UL<<0;

    register U32 regvalue;
    register struct MES_CSC_RegisterSet* pRegister;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CFALSE == MES_CSC_IsBusy() );

    pRegister = __g_pRegister;

	regvalue = pRegister->CONTROL;
	regvalue &= (DITHER | QMODE_MASK | INTENB);
	regvalue |= RUN;
	pRegister->CONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *	@brief	Informs whether the CSC is busy or not.
 *	@return	\b CTRUE indicates the CSC is busy.\n
 *			\b CFALSE indicates the CSC is idle.
 *	@see	MES_CSC_SetDitherEnable,        MES_CSC_GetDitherEnable,
 *          MES_CSC_SetQuantizationMode,    MES_CSC_GetQuantizationMode,
 *          MES_CSC_SetTransparency,        MES_CSC_SetSize,
 *          MES_CSC_SetSourceAddr,          MES_CSC_SetTargetAddr,
 *          MES_CSC_Run                    
 */
CBOOL	MES_CSC_IsBusy( void )
{

	const U32 RUN		= 1UL<<0;

	MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)( __g_pRegister->CONTROL & RUN );
}



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
//	Module     : MCUS
//	File       : mes_mcus.c
//	Description:
//	Author     : Firmware Team
//	History    :
//------------------------------------------------------------------------------

#include "mes_mcus.h"

/// @brief  MCUS Module's Register List
struct  MES_MCUS_RegisterSet
{
	volatile U32 MEMBW			;	///< 00h : Memory Bus Width Register
	volatile U32 MEMTIMEACS		;	///< 04h : Memory Timing for tACS Register
	volatile U32 MEMTIMECOS		;	///< 08h : Memory Timing for tCOS Register
	volatile U32 MEMTIMEACC[2]	;	///< 0Ch, 10h : Memory Timing for tACC Register
	volatile U32 MEMTIMESACC[2]	;	///< 14h, 18h : Memory Timing for tSACC Register
	volatile U32 MEMTIMESACCW[2];	///< 1Ch, 20h : Memory Timing for tSACCW Low Register
	volatile U32 MEMTIMECOH		;	///< 24h : Memory Timing for tCOH Register
	volatile U32 MEMTIMECAH		;	///< 28h : Memory Timing for tCAH Register
	volatile U32 MEMBURST[2]	;	///< 2Ch, 30h : Memory Burst Control Register
	volatile U32 MEMWAIT		;	///< 34h : Memory Wait Control Register
	const    U32 __Reserved0[15];	///< 38h ~ 70h : Reserved for future use.
	volatile U32 NFCONTROL		;	///< 74h : Nand Flash Control Register
	volatile U32 NFECCL			;	///< 78h : Nand Flash ECC Low Register
    volatile U32 NFECCH			;   ///< 7Ch : Nand Flash ECC High Register
    volatile U32 NFORGECCL		;   ///< 80h : Nand Flash Origin ECC Low Register
    volatile U32 NFORGECCH		;   ///< 84h : Nand Flash Origin ECC High Register
    volatile U32 NFCNT  		;   ///< 88h : Nand Flash Data Count Register
    volatile U32 NFECCSTATUS	;   ///< 8Ch : Nand Flash ECC Status Register
    volatile U32 NFSYNDRONE31	;   ///< 90h : Nand Flash ECC Syndrome Value31 Register
    volatile U32 NFSYNDRONE75	;   ///< 94h : Nand Flash ECC Syndrome Value75 Register

};

static  struct
{
    struct MES_MCUS_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_MCUS_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_MCUS_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see    MES_MCUS_SelectModule,
 *          MES_MCUS_GetCurrentModule,   MES_MCUS_GetNumberOfModule
 */
CBOOL   MES_MCUS_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;

	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;

		for( i=0; i < NUMBER_OF_MCUS_MODULE; i++ )
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
 *  @see        MES_MCUS_Initialize,
 *              MES_MCUS_GetCurrentModule,   MES_MCUS_GetNumberOfModule
 */
U32    MES_MCUS_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_MCUS_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_MCUS_Initialize,         MES_MCUS_SelectModule,
 *              MES_MCUS_GetNumberOfModule
 */
U32     MES_MCUS_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_MCUS_Initialize,         MES_MCUS_SelectModule,
 *              MES_MCUS_GetCurrentModule
 */
U32     MES_MCUS_GetNumberOfModule( void )
{
    return NUMBER_OF_MCUS_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see        MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_SetBaseAddress,       MES_MCUS_GetBaseAddress,
 *              MES_MCUS_OpenModule,           MES_MCUS_CloseModule,
 *              MES_MCUS_CheckBusy,            MES_MCUS_CanPowerDown
 */
U32     MES_MCUS_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_MCUS_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_MCUS_MODULE + (OFFSET_OF_MCUS_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_MCUS_GetPhysicalAddress,
 *              MES_MCUS_SetBaseAddress,       MES_MCUS_GetBaseAddress,
 *              MES_MCUS_OpenModule,           MES_MCUS_CloseModule,
 *              MES_MCUS_CheckBusy,            MES_MCUS_CanPowerDown
 */
U32     MES_MCUS_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_MCUS_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_MCUS_GetPhysicalAddress,   MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_GetBaseAddress,
 *              MES_MCUS_OpenModule,           MES_MCUS_CloseModule,
 *              MES_MCUS_CheckBusy,            MES_MCUS_CanPowerDown
 */
void    MES_MCUS_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_MCUS_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_MCUS_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_MCUS_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_MCUS_GetPhysicalAddress,   MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_SetBaseAddress,
 *              MES_MCUS_OpenModule,           MES_MCUS_CloseModule,
 *              MES_MCUS_CheckBusy,            MES_MCUS_CanPowerDown
 */
U32     MES_MCUS_GetBaseAddress( void )
{

	MES_ASSERT( NUMBER_OF_MCUS_MODULE > __g_CurModuleIndex );
	
    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_MCUS_GetPhysicalAddress,   MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_SetBaseAddress,       MES_MCUS_GetBaseAddress,
 *              MES_MCUS_CloseModule,
 *              MES_MCUS_CheckBusy,            MES_MCUS_CanPowerDown
 */
CBOOL   MES_MCUS_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_MCUS_GetPhysicalAddress,   MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_SetBaseAddress,       MES_MCUS_GetBaseAddress,
 *              MES_MCUS_OpenModule,
 *              MES_MCUS_CheckBusy,            MES_MCUS_CanPowerDown
 */
CBOOL   MES_MCUS_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_MCUS_GetPhysicalAddress,   MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_SetBaseAddress,       MES_MCUS_GetBaseAddress,
 *              MES_MCUS_OpenModule,           MES_MCUS_CloseModule,
 *              MES_MCUS_CanPowerDown
 */
CBOOL   MES_MCUS_CheckBusy( void )
{
	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_MCUS_GetPhysicalAddress,   MES_MCUS_GetSizeOfRegisterSet,
 *              MES_MCUS_SetBaseAddress,       MES_MCUS_GetBaseAddress,
 *              MES_MCUS_OpenModule,           MES_MCUS_CloseModule,
 *              MES_MCUS_CheckBusy
 */
CBOOL   MES_MCUS_CanPowerDown( void )
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
 *  @see        MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
S32     MES_MCUS_GetInterruptNumber( void )
{
    return  INTNUM_OF_MCUS_MODULE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number .
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @remarks    MCUS have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_MCUS_GetInterruptNumber,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
void    MES_MCUS_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
	const U32 IRQPEND_POS	= 15;
    const U32 ECCRST_POS    = 11;
	const U32 IRQENB_POS	= 8;
	const U32 IRQPEND		= (1UL<<IRQPEND_POS);
    const U32 ECCRST        = (1UL<<ECCRST_POS);
	const U32 IRQENB		= (1UL<<IRQENB_POS);

    register U32 regvalue;
    register struct MES_MCUS_RegisterSet*   pRegister;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	regvalue = pRegister->NFCONTROL;

	regvalue &= ~(IRQPEND | ECCRST | IRQENB);
	regvalue |= (U32)Enable<<IRQENB_POS;

	pRegister->NFCONTROL = regvalue;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSE indicate that Interrupt is disabled.
 *  @remarks    MCUS have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
CBOOL   MES_MCUS_GetInterruptEnable( S32 IntNum )
{
	const U32 IRQENB_POS	= 8;
	const U32 IRQENB_MASK	= (1UL<<IRQENB_POS);

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->NFCONTROL & IRQENB_MASK) >> IRQENB_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @remarks    MCUS have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
CBOOL   MES_MCUS_GetInterruptPending( S32 IntNum )
{
	const U32 IRQPEND_POS	= 15;
	const U32 IRQPEND_MASK	= (1UL<<IRQPEND_POS);

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->NFCONTROL & IRQPEND_MASK) >> IRQPEND_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt number.
 *  @return     None.
 *  @remarks    MCUS have one interrupt. So always \e IntNum set to 0.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
void    MES_MCUS_ClearInterruptPending( S32 IntNum )
{
	const U32 IRQPEND_POS	= 15;
    const U32 ECCRST_POS    = 11;
	const U32 IRQPEND		= (1UL<<IRQPEND_POS);
    const U32 ECCRST        = (1UL<<ECCRST_POS);

    register U32 regvalue;
    register struct MES_MCUS_RegisterSet*   pRegister;

    MES_ASSERT( 0 == IntNum );
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	regvalue = pRegister->NFCONTROL;

	regvalue &= ~(IRQPEND | ECCRST );
	regvalue |= (1UL)<<IRQPEND_POS;

	pRegister->NFCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
void    MES_MCUS_SetInterruptEnableAll( CBOOL Enable )
{
	const U32 IRQPEND_POS	= 15;
    const U32 ECCRST_POS    = 11;
	const U32 IRQENB_POS	= 8;
	const U32 IRQPEND		= (1UL<<IRQPEND_POS);
    const U32 ECCRST        = (1UL<<ECCRST_POS);
	const U32 IRQENB		= (1UL<<IRQENB_POS);

    register U32 regvalue;
    register struct MES_MCUS_RegisterSet*   pRegister;

    MES_ASSERT( (0==Enable) || (1==Enable) );
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	regvalue = pRegister->NFCONTROL;

	regvalue &= ~(IRQPEND | ECCRST | IRQENB);
	regvalue |= ( (U32)Enable<<IRQENB_POS );

	pRegister->NFCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
CBOOL   MES_MCUS_GetInterruptEnableAll( void )
{
	const U32 IRQENB_POS	= 8;
	const U32 IRQENB_MASK	= (1UL<<IRQENB_POS);

    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->NFCONTROL & IRQENB_MASK) >> IRQENB_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are pended or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) pending is seted. \n
 *              \b CFALSE indicate that All pending is NOT seted.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,
 *              MES_MCUS_ClearInterruptPendingAll, MES_MCUS_GetInterruptPendingNumber
 */
CBOOL   MES_MCUS_GetInterruptPendingAll( void )
{
	const U32 IRQPEND_POS	= 15;
	const U32 IRQPEND_MASK	= (1UL<<IRQPEND_POS);

    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->NFCONTROL & IRQPEND_MASK) >> IRQPEND_POS );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_GetInterruptPendingNumber
 */
void    MES_MCUS_ClearInterruptPendingAll( void )
{
	const U32 IRQPEND_POS	= 15;
    const U32 ECCRST_POS    = 11;
	const U32 IRQPEND		= (1UL<<IRQPEND_POS);
    const U32 ECCRST        = (1UL<<ECCRST_POS);

    register U32 regvalue;
    register struct MES_MCUS_RegisterSet*   pRegister;

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	regvalue = pRegister->NFCONTROL;

	regvalue &= ~(IRQPEND | ECCRST );
	regvalue |= (1UL)<<IRQPEND_POS;

	pRegister->NFCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_MCUS_GetInterruptNumber,       MES_MCUS_SetInterruptEnable,
 *              MES_MCUS_GetInterruptEnable,       MES_MCUS_GetInterruptPending,
 *              MES_MCUS_ClearInterruptPending,    MES_MCUS_SetInterruptEnableAll,
 *              MES_MCUS_GetInterruptEnableAll,    MES_MCUS_GetInterruptPendingAll,
 *              MES_MCUS_ClearInterruptPendingAll
 */
S32     MES_MCUS_GetInterruptPendingNumber( void )  // -1 if None
{
	const U32 IRQPEND_POS	= 15;
	const U32 IRQPEND_MASK	= (1UL<<IRQPEND_POS);

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->NFCONTROL & IRQPEND_MASK )
    {
        return 0;
    }

    return -1;
}


//------------------------------------------------------------------------------
// DMA Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief  Get DMA peripheral index of MCUS
 *  @return DMA peripheral index of MCUS
 *  @see    MES_MCUS_GetDMABusWidth
 */
U32     MES_MCUS_GetDMAIndex( void )
{
    return DMAINDEX_OF_MCUS_MODULE;
}

/**
 *  @brief      Get bus width of MCUS
 *  @return     DMA bus width of MCUS
 *  @see        MES_MCUS_GetDMAIndex
 */
U32     MES_MCUS_GetDMABusWidth( void )
{
    return 16;
}

//--------------------------------------------------------------------------
static U32 MES_MCUS_SetValue( U32 OldValue, U32 BitValue, U32 MSB, U32 LSB )
{
    U32 mask;
	MES_ASSERT( 32 > MSB );
	MES_ASSERT( 32 > LSB );
	MES_ASSERT( MSB >= LSB );
	mask = (((1UL<<(MSB-LSB+1)) - 1) << LSB);
	MES_ASSERT( 0 == ((BitValue<<LSB)&(~mask)) );
	return ((OldValue & ~mask) | (BitValue << LSB));
}

static U32 MES_MCUS_GetValue( U32 Value, U32 MSB, U32 LSB )
{
    U32 mask;
	MES_ASSERT( 32 > MSB );
	MES_ASSERT( 32 > LSB );
	MES_ASSERT( MSB >= LSB );
	mask = (((1UL<<(MSB-LSB+1)) - 1) << LSB);
	return ((Value & mask)>>LSB);
}

static U32 MES_MCUS_SetBit( U32 OldValue, U32 BitValue, U32 BitPos )
{
    U32 mask;
	MES_ASSERT( 32 > BitPos );
	MES_ASSERT( 2 > BitValue );
	mask = (1UL<<BitPos);
	return ((OldValue & ~mask) | (BitValue<<BitPos));
}

static U32 MES_MCUS_GetBit( U32 Value, U32 BitPos )
{
	MES_ASSERT( 32 > BitPos );
	return ((Value >> BitPos) & 1UL);
}

//--------------------------------------------------------------------------
//	Static BUS Configuration.
//--------------------------------------------------------------------------
/**
 * 	@brief	    set static BUS configration.
 *  @param[in]  Id          Static BUS ID
 *  @param[in]  BitWidth	BUS bit width. ( it must be 8 or 16 ). \n This argument is only valid in case of Static #0 ~ #9.
 *  @param[in]  tACS		address setup time before nCS, 0 ~ 3.
 *  @param[in]  tCAH		address hold time after nCS, 0 ~ 3.
 *  @param[in]  tCOS		chip select setup time before nOE, 0 ~ 3.
 *  @param[in]  tCOH		chip select hold time after nOE, 0 ~ 3.
 *  @param[in]  tACC		data access time, 1 ~ 16.
 *  @param[in]  tSACC		data access time in burst access, 1 ~ 16.\n This argument is only valid in case of Static #0 ~ #9.
 *  @param[in]  WaitMode	Wait control \n This argument is only valid in case of Static #0 ~ #9.
 *  @param[in]  BurstRead   Burst mode control in reading.\n This argument is only valid in case of Static #0 ~ #9.
 *  @param[in]  BurstWrite  Burst mode control in writing.\n This argument is only valid in case of Static #0 ~ #9.
 *	@return     None.
 *	@see	    MES_MCUS_GetStaticBUSConfig
 */
void MES_MCUS_SetStaticBUSConfig	( MES_MCUS_SBUSID Id, U32 BitWidth,
					  U32 tACS, U32 tCAH, U32 tCOS, U32 tCOH,
					  U32 tACC, U32 tSACC,
					  MES_MCUS_WAITMODE WaitMode,
					  MES_MCUS_BURSTMODE BurstRead,
					  MES_MCUS_BURSTMODE BurstWrite  )
{
	register U32 BitPos;
	register U32 regIndex;
    register struct MES_MCUS_RegisterSet*   pRegister;

	MES_ASSERT( MES_MCUS_SBUSID_NAND >= Id );
	MES_ASSERT( (8 == BitWidth || 16 == BitWidth) || (9 < Id) );
	MES_ASSERT( 4 > tACS );
	MES_ASSERT( 4 > tCAH );
	MES_ASSERT( 4 > tCOS );
	MES_ASSERT( 4 > tCOH );
	MES_ASSERT( 1 <=  tACC &&  tACC <= 16 );
	MES_ASSERT( (1 <= tSACC && tSACC <= 16) || (9 < Id) );
	MES_ASSERT( 4 > WaitMode );
	MES_ASSERT( (4 > BurstRead) || (9 < Id) );
	MES_ASSERT( (4 > BurstWrite) || (9 < Id) );
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

	BitPos = ((U32)Id) * 2;
	// Set timing for tACS, tCAH, tCOS, tOCH.
	// 0 : 1 cycle, 1 : 2 cycle, 2 : 3 cycle, 3 : 0 cycle
	tACS = (tACS + 3) % 4;
	tCAH = (tCAH + 3) % 4;
	tCOS = (tCOS + 3) % 4;
	tCOH = (tCOH + 3) % 4;
	
	pRegister->MEMTIMEACS = MES_MCUS_SetValue( pRegister->MEMTIMEACS, tACS, BitPos + 1, BitPos );
	pRegister->MEMTIMECAH = MES_MCUS_SetValue( pRegister->MEMTIMECAH, tCAH, BitPos + 1, BitPos );
	pRegister->MEMTIMECOS = MES_MCUS_SetValue( pRegister->MEMTIMECOS, tCOS, BitPos + 1, BitPos );
	pRegister->MEMTIMECOH = MES_MCUS_SetValue( pRegister->MEMTIMECOH, tCOH, BitPos + 1, BitPos );

	// Wait control
	pRegister->MEMWAIT = MES_MCUS_SetValue( pRegister->MEMWAIT, (U32)WaitMode, BitPos + 1, BitPos );

	BitPos      = ((U32)Id) * 4;
    regIndex    = BitPos / 32;
    BitPos      = BitPos % 32;

	tACC  -= 1;	// tACC  = n+1 cycle
	tSACC -= 1;	// tSACC = n+1 cycle

	if( MES_MCUS_SBUSID_STATIC9 >= Id  )	// For static bus #0 ~ #9
	{
		pRegister->MEMTIMEACC[regIndex]  = MES_MCUS_SetValue( pRegister->MEMTIMEACC[regIndex] , tACC , BitPos + 3, BitPos );
		pRegister->MEMTIMESACC[regIndex] = MES_MCUS_SetValue( pRegister->MEMTIMESACC[regIndex], tSACC, BitPos + 3, BitPos );

    	pRegister->MEMBW     = MES_MCUS_SetBit( pRegister->MEMBW, (BitWidth>>4), Id );
		pRegister->MEMBURST[regIndex] = MES_MCUS_SetValue( pRegister->MEMBURST[regIndex], (U32)BurstRead , BitPos + 1, BitPos );
		pRegister->MEMBURST[regIndex] = MES_MCUS_SetValue( pRegister->MEMBURST[regIndex], (U32)BurstWrite, BitPos + 3, BitPos + 2 );
	}
	else if( MES_MCUS_SBUSID_NAND == Id )   // NAND
	{
		pRegister->MEMTIMEACC[regIndex]  = MES_MCUS_SetValue( pRegister->MEMTIMEACC[regIndex] , tACC , BitPos + 3, BitPos );
	}
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get static BUS configration.
 *  @param[in]  Id    static BUS ID
 *  @param[out] pBitWidth	BUS bit width.\n This argument is only valid in case of Static #0 ~ #9.
 *  @param[out] ptACS		address setup time before nCS
 *  @param[out] ptCAH		address hold time after nCS
 *  @param[out] ptCOS		chip select setup time before nOE
 *  @param[out] ptCOH		chip select hold time after nOE
 *  @param[out] ptACC		data access time
 *  @param[out] ptSACC		data access time in burst access.\n This argument is only valid in case of Static #0 ~ #9.
 *  @param[out] pWaitMode	wait control. \n This argument is only valid in case of Static #0 ~ #9.
 *  @param[out] pBurstRead   Burst mode control in reading.\n This argument is only valid in case of Static #0 ~ #9.
 *  @param[out] pBurstWrite  Burst mode control in writing.\n This argument is only valid in case of Static #0 ~ #9.
 *	@return     None.
 *	@remark	    Arguments which does not required can be CNULL.
 *	@see	    MES_MCUS_SetStaticBUSConfig
 */
void MES_MCUS_GetStaticBUSConfig	( MES_MCUS_SBUSID Id, U32* pBitWidth,
					  U32* ptACS, U32* ptCAH, U32* ptCOS, U32* ptCOH,
					  U32* ptACC, U32* ptSACC,
					  MES_MCUS_WAITMODE* pWaitMode,
					  MES_MCUS_BURSTMODE* pBurstRead,
					  MES_MCUS_BURSTMODE* pBurstWrite )
{
	register U32 BitPos;
	register U32 regIndex;
    register struct MES_MCUS_RegisterSet*   pRegister;

	MES_ASSERT( MES_MCUS_SBUSID_NAND >= Id );
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	BitPos = ((U32)Id) * 2;

	// Get timing for tACS, tCAH, tCOS, tOCH.

	if( CNULL != ptACS )
	{
    	*ptACS = MES_MCUS_GetValue( pRegister->MEMTIMEACS, BitPos + 1, BitPos );
    	// 0 : 1 cycle, 1 : 2 cycle, 2 : 3 cycle, 3 : 0 cycle
    	*ptACS = (*ptACS + 1) % 4;
    }

	if( CNULL != ptCAH )
	{
    	*ptCAH = MES_MCUS_GetValue( pRegister->MEMTIMECAH, BitPos + 1, BitPos );
    	*ptCAH = (*ptCAH + 1) % 4;
    }

	if( CNULL != ptCOS )
	{
    	*ptCOS = MES_MCUS_GetValue( pRegister->MEMTIMECOS, BitPos + 1, BitPos );
    	*ptCOS = (*ptCOS + 1) % 4;
    }

	if( CNULL != ptCOH )
	{
    	*ptCOH = MES_MCUS_GetValue( pRegister->MEMTIMECOH, BitPos + 1, BitPos );
    	*ptCOH = (*ptCOH + 1) % 4;
    }

	// Wait control
    if( CNULL != pWaitMode ){ *pWaitMode = (MES_MCUS_WAITMODE)MES_MCUS_GetValue( pRegister->MEMWAIT, BitPos + 1, BitPos ); }

	BitPos      = ((U32)Id) * 4;
    regIndex    = BitPos / 32;
    BitPos      = BitPos % 32;

	if( MES_MCUS_SBUSID_STATIC9 >= Id  )	// For static bus #0 ~ #9
	{
		if( CNULL != ptACC )
		{
    		*ptACC = MES_MCUS_GetValue( pRegister->MEMTIMEACC[regIndex] , BitPos + 3, BitPos );
            *ptACC  += 1;	// tACC  = n+1 cycle
    	}

    	if( CNULL != ptSACC )
    	{
    		*ptSACC = MES_MCUS_GetValue( pRegister->MEMTIMESACC[regIndex], BitPos + 3, BitPos );
	        *ptSACC += 1;	// tSACC = n+1 cycle
    	}

		if( CNULL != pBitWidth )    { *pBitWidth   = (MES_MCUS_WAITMODE)MES_MCUS_GetBit( pRegister->MEMBW, (U32)Id ) ? 16 : 8;                       }
		if( CNULL != pBurstRead )   { *pBurstRead  = (MES_MCUS_BURSTMODE)MES_MCUS_GetValue( pRegister->MEMBURST[regIndex], BitPos + 1, BitPos );     }
		if( CNULL != pBurstWrite )  { *pBurstWrite = (MES_MCUS_BURSTMODE)MES_MCUS_GetValue( pRegister->MEMBURST[regIndex], BitPos + 3, BitPos + 2 ); }
	}
	else if( MES_MCUS_SBUSID_NAND == Id )   // NAND
	{
		if( CNULL != ptACC )
		{
    		*ptACC = MES_MCUS_GetValue( pRegister->MEMTIMEACC[regIndex] , BitPos + 3, BitPos );
    		*ptACC  += 1;	// tACC  = n+1 cycle
        }
	}
}

//--------------------------------------------------------------------------
//	NAND Flash Memory operations
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * 	@brief	    Set NAND Flash Address Type.
 *  @param[in]  type    NAND Flash address type
 *	@return	    None.
 * 	@remark	    It can also be set NFTYPE by external pin configuration(CfgNFType)
 * 			    for NAND Booting.
 * 	@see	                                    MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void    MES_MCUS_SetNFType ( MES_MCUS_NFTYPE type )
{
    U32 nandctrl;

	MES_ASSERT( 4 > type );
	MES_ASSERT( CNULL != __g_pRegister );

	nandctrl = __g_pRegister->NFCONTROL;
	nandctrl = MES_MCUS_SetBit( nandctrl, 0, 15 );	// Unmask IRQPEND bit.
	nandctrl = MES_MCUS_SetValue( nandctrl, type, 4, 3 );
	__g_pRegister->NFCONTROL = nandctrl;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get current setting for NAND Flash Address Type.
 *	@return	    current setting for NAND Flash Address Type.
 * 	@remark	    It can also be set NFTYPE by external pin configuration(CfgNFType)
 * 			    for NAND Booting.
 * 	@see	    MES_MCUS_SetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
MES_MCUS_NFTYPE	MES_MCUS_GetNFType ( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (MES_MCUS_NFTYPE)MES_MCUS_GetValue( __g_pRegister->NFCONTROL, 4, 3 );
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Set NAND Flash Boot Enable.
 *  @param[in]  bEnb    Set it to CTRUE to enable NAND Booting.
 *	@return	    None.
 *	@remark	    It can also be enabled by external pin configuration(CfgNFBoot).
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *                                              MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void	MES_MCUS_SetNFBootEnable ( CBOOL bEnb )
{
    U32 nandctrl;

    MES_ASSERT( (0==bEnb) || (1==bEnb) );
	MES_ASSERT( CNULL != __g_pRegister );

	nandctrl = __g_pRegister->NFCONTROL;
	nandctrl = MES_MCUS_SetBit( nandctrl, 0, 15 );	// Unmask IRQPEND bit.
	nandctrl = MES_MCUS_SetBit( nandctrl, bEnb, 5 );
	__g_pRegister->NFCONTROL = nandctrl;
}


//------------------------------------------------------------------------------
/**
 * 	@brief	Informs whether NAND Flash Booting is enabled or not.
 *	@return	\b CTRUE indicates NAND Flash Booting is enabled.\n
 *			\b CFALSE indicates NAND Flash Booting is disabled.
 *	@remark	It can also be enabled by external pin configuration(CfgNFBoot).
 * 	@see	MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *          MES_MCUS_SetNFBootEnable,
 *          MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *          MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *          MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *          MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *          MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *          MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *          MES_MCUS_GetNFECCDecoderResult
 */
CBOOL   MES_MCUS_IsNFBootEnable ( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return MES_MCUS_GetBit( __g_pRegister->NFCONTROL, 5 ) ? CTRUE : CFALSE;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Set NAND Flash bank number to access.
 *  @param[in]  Bank    NAND Flash bank number to access, 0 ~ 2.
 *	@return	    None.
 *	@remark		You can select bank as 2 to de-assert NAND flash chip select 0 and 1.
 *				To de-assert NAND flash chip select,  you have to access NFDATA, 
 *				NFCMD or NFADDR once at least after selecting bank as 2.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *                                              MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void    MES_MCUS_SetNFBank( U32 Bank )
{
    U32 nandctrl;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 8 > Bank );

	nandctrl = __g_pRegister->NFCONTROL;
	nandctrl = MES_MCUS_SetBit( nandctrl, 0, 15 );	// Unmask IRQPEND bit.
	nandctrl = MES_MCUS_SetValue( nandctrl, Bank, 2, 0 );
	__g_pRegister->NFCONTROL = nandctrl;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get current NAND Flash bank number.
 *	@return	    current NAND Flash bank number.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
U32     MES_MCUS_GetNFBank ( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return MES_MCUS_GetValue( __g_pRegister->NFCONTROL, 2, 0 );
}

//------------------------------------------------------------------------------
/**
 * 	@brief	Informs whether NAND Flash is ready or not.
 *	@return	\b CTRUE indicates NAND Flash is ready.\n
 *			\b CFALSE indicates NAND Flash is busy.
 *	@remark	There's a external pin, RnB, indicates the status of NAND Flash
 *			operation. When low, it indicates that a program, erase or random
 *			read operation is in progress and returns to high state upon
 *			completion. This function returns the state of RnB pin.
 * 	@see	MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *          MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *          MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *                                          MES_MCUS_ResetNFECCBlock,
 *          MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *          MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *          MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *          MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *          MES_MCUS_GetNFECCDecoderResult
 */
CBOOL   MES_MCUS_IsNFReady ( void )
{
    const U32 RNB_POS = 9;
    const U32 RNB_MASK = 1UL << RNB_POS;

	MES_ASSERT( CNULL != __g_pRegister );

	return (CBOOL) ((__g_pRegister->NFCONTROL & RNB_MASK) >> RNB_POS);
}


//------------------------------------------------------------------------------
/**
 * 	@brief	    Hardware ECC reset
 *	@return	    None.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void    MES_MCUS_ResetNFECCBlock( void )
{
    const U32   ECCRST_MASK = 1UL << 11;
    const U32   IRQPEND_MASK = 1UL << 15;

    register struct MES_MCUS_RegisterSet*   pRegister;
    register U32 regvalue;

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    regvalue = pRegister->NFCONTROL;

    regvalue &= ~(IRQPEND_MASK | ECCRST_MASK );
    regvalue |= ECCRST_MASK;

    pRegister->NFCONTROL = regvalue;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get ECC parity code during \b WRITE operation.
 *  @param[out] pECCH   ECC parity code high value ( 24bit ).
 *  @param[out] pECCL   ECC parity code low value ( 32bit ).
 *	@return     None.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *                                              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void    MES_MCUS_GetNFECC( U32* pECCH, U32* pECCL )
{
    const U32   ECCH_POS = 0;
    const U32   ECCH_MASK = ((1UL<<24)-1) << ECCH_POS;

    MES_ASSERT( CNULL != pECCH );
    MES_ASSERT( CNULL != pECCL );
	MES_ASSERT( CNULL != __g_pRegister );

	*pECCH  =   __g_pRegister->NFECCH & ECCH_MASK;
    *pECCL  =   __g_pRegister->NFECCL;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Set ECC parity code for reading operation
 *  @param[in]  ECCH    ECC parity code high value ( 24bit ).
 *  @param[in]  ECCL    ECC parity code low value ( 32bit ).
 *	@return	    None.
 *	@remark	    When NAND Read operation, first read Original ECC data already saved in
 *              the NAND spare area and then setting this function.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void    MES_MCUS_SetNFOriginECC( U32 ECCH, U32 ECCL )
{
	MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( (1UL<<24) > ECCH );

    __g_pRegister->NFORGECCH = ECCH;
    __g_pRegister->NFORGECCL = ECCL;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get setting value of NFORGECC register.
 *  @param[out] pECCH   ECC parity code high value ( 24bit ).
 *  @param[out] pECCL   ECC parity code low value ( 32bit ).
 *	@return	    None.
 *	@remark	    Just return setting value of NFORGECC register.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *                                              MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
void    MES_MCUS_GetNFOriginECC( U32* pECCH, U32* pECCL )
{
    const U32   ORGECCH_POS = 0;
    const U32   ORGECCH_MASK = ((1UL<<24)-1) << ORGECCH_POS;

    MES_ASSERT( CNULL != pECCH );
    MES_ASSERT( CNULL != pECCL );
	MES_ASSERT( CNULL != __g_pRegister );

	*pECCH  =   __g_pRegister->NFORGECCH & ORGECCH_MASK;
    *pECCL  =   __g_pRegister->NFORGECCL;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get write data count
 *	@return	    write data count
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
U32     MES_MCUS_GetNFWriteDataCount( void )
{
    const U32   NFWRCNT_POS = 16;
    const U32   NFWRCNT_MASK = ((1UL<<10)-1) << NFWRCNT_POS;

	MES_ASSERT( CNULL != __g_pRegister );

    return (__g_pRegister->NFCNT & NFWRCNT_MASK) >> NFWRCNT_POS;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get read data count
 *	@return	    read data count
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *                                              MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
U32     MES_MCUS_GetNFReadDataCount( void )
{
    const U32   NFRDCNT_POS = 0;
    const U32   NFRDCNT_MASK = ((1UL<<10)-1) << NFRDCNT_POS;

	MES_ASSERT( CNULL != __g_pRegister );

    return (__g_pRegister->NFCNT & NFRDCNT_MASK) >> NFRDCNT_POS;
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Error check on read data
 *	@return	    \b CTRUE indicate NAND read operation have some error. \n
 *              \b CFALSE indicate NAND read operation is successed.  \n
 *	@remark	    When completing NAND read operating, error check on read data using this function.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
CBOOL   MES_MCUS_IsNFError( void )
{
    const U32 NFCHECKERROR_POS = 2;
    const U32 NFCHECKERROR_MASK = 1UL << NFCHECKERROR_POS;

	MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)((__g_pRegister->NFECCSTATUS & NFCHECKERROR_MASK) >> NFCHECKERROR_POS);
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Check read operation is completed or Not.
 *	@return	    \b CTRUE indicate read operation is completed. \n
 *              \b CFALSE indicate read operation is running or idle.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *                                              MES_MCUS_IsNFWriteEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
CBOOL   MES_MCUS_IsNFReadEnd( void )
{
    const U32 NFECCDECDONE_POS = 1;
    const U32 NFECCDECDONE_MASK = 1UL << NFECCDECDONE_POS;

	MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)((__g_pRegister->NFECCSTATUS & NFECCDECDONE_MASK) >> NFECCDECDONE_POS);
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Check write operation is completed or Not.
 *	@return	    \b CTRUE indicate write operation is completed. \n
 *              \b CFALSE indicate write operation is running or idle.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,
 *              MES_MCUS_GetNFECCDecoderResult
 */
CBOOL   MES_MCUS_IsNFWriteEnd( void )
{
    const U32 NFECCENCDONE_POS = 0;
    const U32 NFECCENCDONE_MASK = 1UL << NFECCENCDONE_POS;

	MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)((__g_pRegister->NFECCSTATUS & NFECCENCDONE_MASK) >> NFECCENCDONE_POS);
}

//------------------------------------------------------------------------------
/**
 * 	@brief	    Get ECC decoder result
 *  @param[out] pSyndrome7  Odd Syndrome Data7
 *  @param[out] pSyndrome5  Odd Syndrome Data5
 *  @param[out] pSyndrome3  Odd Syndrome Data3
 *  @param[out] pSyndrome1  Odd Syndrome Data1
 *	@return	    None.
 * 	@see	    MES_MCUS_SetNFType,             MES_MCUS_GetNFType,
 *              MES_MCUS_SetNFBootEnable,       MES_MCUS_IsNFBootEnable,
 *              MES_MCUS_SetNFBank,             MES_MCUS_GetNFBank,
 *              MES_MCUS_IsNFReady,             MES_MCUS_ResetNFECCBlock,
 *              MES_MCUS_GetNFECC,              MES_MCUS_SetNFOriginECC,
 *              MES_MCUS_GetNFOriginECC,        MES_MCUS_GetNFWriteDataCount,
 *              MES_MCUS_GetNFReadDataCount,    MES_MCUS_IsNFError,
 *              MES_MCUS_IsNFReadEnd,           MES_MCUS_IsNFWriteEnd
 */
void    MES_MCUS_GetNFECCDecoderResult( U32* pSyndrome7, U32* pSyndrome5, U32* pSyndrome3, U32* pSyndrome1 )
{
    const U32   SYNDROM1_POS = 0;
    const U32   SYNDROM1_MASK = ((1UL<<13)-1) << SYNDROM1_POS;

    const U32   SYNDROM3_POS = 13;
    const U32   SYNDROM3_MASK = ((1UL<<13)-1) << SYNDROM3_POS;

    const U32   SYNDROM5_POS = 0;
    const U32   SYNDROM5_MASK = ((1UL<<13)-1) << SYNDROM5_POS;

    const U32   SYNDROM7_POS = 13;
    const U32   SYNDROM7_MASK = ((1UL<<13)-1) << SYNDROM7_POS;

    register struct MES_MCUS_RegisterSet*   pRegister;

    MES_ASSERT( CNULL != pSyndrome7 );
    MES_ASSERT( CNULL != pSyndrome5 );
    MES_ASSERT( CNULL != pSyndrome3 );
    MES_ASSERT( CNULL != pSyndrome1 );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    *pSyndrome7 = ((pRegister->NFSYNDRONE75 & SYNDROM7_MASK) >> SYNDROM7_POS );
    *pSyndrome5 =  (pRegister->NFSYNDRONE75 & SYNDROM5_MASK);
    *pSyndrome3 = ((pRegister->NFSYNDRONE31 & SYNDROM3_MASK) >> SYNDROM3_POS );
    *pSyndrome1 =  (pRegister->NFSYNDRONE31 & SYNDROM1_MASK);
}


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
//	Module     : SSPSPI
//	File       : mes_sspspi.c
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------

#include "mes_sspspi.h"

/// @brief  SSPSPI Module's Register List
struct  MES_SSPSPI_RegisterSet
{
    volatile U16    CONT0;              ///< 0x00 : SSP/SPI Control Register 0
    volatile U16    CONT1;              ///< 0x02 : SSP/SPI Control Register 1
    volatile U16    DATA;               ///< 0x04 : SSP/SPI Data Register
    volatile U16    STATE;              ///< 0x06 : SSP/SPI Status Register
    volatile U16    RXBUSTSIZE;         ///< 0x08 : SSP/SPI Burst Receive Size Register
    const    U16    __Reserved[0x1B];   ///< 0x0A ~ 0x3E : Reserved Region
    volatile U32    CLKENB;             ///< 0x40 : SSP/SPI Clock Enable Register
    volatile U32    CLKGEN;             ///< 0x44 : SSP/SPI Clock Generator Register
};

static  struct
{
    struct MES_SSPSPI_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_SSPSPI_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_SSPSPI_RegisterSet *__g_pRegister = CNULL;


//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see    MES_SSPSPI_SelectModule,
 *          MES_SSPSPI_GetCurrentModule,   MES_SSPSPI_GetNumberOfModule
 */
CBOOL   MES_SSPSPI_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_SSPSPI_MODULE; i++ )
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
 *  @see        MES_SSPSPI_Initialize,
 *              MES_SSPSPI_GetCurrentModule,   MES_SSPSPI_GetNumberOfModule
 */
U32    MES_SSPSPI_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;
    
	return oldindex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_SSPSPI_Initialize,         MES_SSPSPI_SelectModule,
 *              MES_SSPSPI_GetNumberOfModule
 */
U32     MES_SSPSPI_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_SSPSPI_Initialize,         MES_SSPSPI_SelectModule,
 *              MES_SSPSPI_GetCurrentModule
 */
U32     MES_SSPSPI_GetNumberOfModule( void )
{
    return NUMBER_OF_SSPSPI_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see        MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_SetBaseAddress,       MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_OpenModule,           MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CheckBusy,            MES_SSPSPI_CanPowerDown
 */
U32     MES_SSPSPI_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_SSPSPI_MODULE + (OFFSET_OF_SSPSPI_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_SSPSPI_GetPhysicalAddress,
 *              MES_SSPSPI_SetBaseAddress,       MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_OpenModule,           MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CheckBusy,            MES_SSPSPI_CanPowerDown
 */
U32     MES_SSPSPI_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_SSPSPI_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_SSPSPI_GetPhysicalAddress,   MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_OpenModule,           MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CheckBusy,            MES_SSPSPI_CanPowerDown
 */
void    MES_SSPSPI_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_SSPSPI_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_SSPSPI_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_SSPSPI_GetPhysicalAddress,   MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_SetBaseAddress,
 *              MES_SSPSPI_OpenModule,           MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CheckBusy,            MES_SSPSPI_CanPowerDown
 */
U32     MES_SSPSPI_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_SSPSPI_GetPhysicalAddress,   MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_SetBaseAddress,       MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CheckBusy,            MES_SSPSPI_CanPowerDown
 */
CBOOL   MES_SSPSPI_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_SSPSPI_GetPhysicalAddress,   MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_SetBaseAddress,       MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_OpenModule,
 *              MES_SSPSPI_CheckBusy,            MES_SSPSPI_CanPowerDown
 */
CBOOL   MES_SSPSPI_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_SSPSPI_GetPhysicalAddress,   MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_SetBaseAddress,       MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_OpenModule,           MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CanPowerDown
 */
CBOOL   MES_SSPSPI_CheckBusy( void )
{

    if( CTRUE == MES_SSPSPI_GetEnable() )
    {
        return CTRUE;
    }

	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_SSPSPI_GetPhysicalAddress,   MES_SSPSPI_GetSizeOfRegisterSet,
 *              MES_SSPSPI_SetBaseAddress,       MES_SSPSPI_GetBaseAddress,
 *              MES_SSPSPI_OpenModule,           MES_SSPSPI_CloseModule,
 *              MES_SSPSPI_CheckBusy
 */
CBOOL   MES_SSPSPI_CanPowerDown( void )
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
 *  @see        MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
S32     MES_SSPSPI_GetInterruptNumber( void )
{
    const U32   SSPSPIInterruptNumber[NUMBER_OF_SSPSPI_MODULE] = {

                INTNUM_OF_SSPSPI0_MODULE, INTNUM_OF_SSPSPI0_MODULE  };

    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE > __g_CurModuleIndex );


    return  SSPSPIInterruptNumber[__g_CurModuleIndex];
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number ( 0:Rx Buffer Full, 1:Tx Buffer Empty, 2:Tx/Rx Completed, 3:Error  ).
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @see        MES_SSPSPI_GetInterruptNumber,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
void    MES_SSPSPI_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    const U32   PEND_MASK = 0x0F << 4;

    register struct MES_SSPSPI_RegisterSet*    pRegister;
    register U32    shift;
    register U32    ReadValue;

    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;


    if( 3 == IntNum )
    {   
        shift = 12; 
    }
    else 
    {
        shift = IntNum + 13;        
    }

    ReadValue = pRegister->STATE & ~PEND_MASK;

    ReadValue &= ~(1UL << shift);
    ReadValue |= (U32)Enable << shift;

    pRegister->STATE = (U16)ReadValue;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number ( 0:Rx Buffer Full, 1:Tx Buffer Empty, 2:Tx/Rx Completed, 3:Error  ).
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSE indicate that Interrupt is disabled.
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
CBOOL   MES_SSPSPI_GetInterruptEnable( S32 IntNum )
{
    register U32    shift;

    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    if( 3 == IntNum )
    { 
        shift = 12; 
    }
    else
    {
        shift = IntNum + 13;        
    }

    return (CBOOL)((__g_pRegister->STATE >> shift) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number ( 0:Rx Buffer Full, 1:Tx Buffer Empty, 2:Tx/Rx Completed, 3:Error  ).
 *  @return     \b CTRUE indicate that  Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
CBOOL   MES_SSPSPI_GetInterruptPending( S32 IntNum )
{
    register U32    shift;

    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    shift = IntNum + 4;

     return (CBOOL)((__g_pRegister->STATE >> shift) & 0x01 );

}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt number ( 0:Rx Buffer Full, 1:Tx Buffer Empty, 2:Tx/Rx Completed, 3:Error  ).
 *  @return     None.
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
void    MES_SSPSPI_ClearInterruptPending( S32 IntNum )
{
    const U32   PEND_MASK = 0x0F << 4;

    register struct MES_SSPSPI_RegisterSet*    pRegister;
    register U32    shift;
    register U32    ReadValue;

    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    shift = IntNum + 4;

    ReadValue = pRegister->STATE & ~PEND_MASK;

    ReadValue |= 1UL << shift;

    pRegister->STATE = (U16)ReadValue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
void    MES_SSPSPI_SetInterruptEnableAll( CBOOL Enable )
{
    const U32 IRQ_MASK  =   0x0F << 12;

    MES_ASSERT( (0==Enable) ||(1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    if( Enable )
    {
        __g_pRegister->STATE =   IRQ_MASK;
    }
    else
    {
        __g_pRegister->STATE = 0;
    }
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
CBOOL   MES_SSPSPI_GetInterruptEnableAll( void )
{
    const U32 IRQ_MASK  =   0x0F << 12;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->STATE & IRQ_MASK )
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
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,
 *              MES_SSPSPI_ClearInterruptPendingAll, MES_SSPSPI_GetInterruptPendingNumber
 */
CBOOL   MES_SSPSPI_GetInterruptPendingAll( void )
{
    const U32 PEND_MASK  =   0x0F << 4;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->STATE & PEND_MASK )
    {
        return CTRUE;
    }

    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_GetInterruptPendingNumber
 */
void    MES_SSPSPI_ClearInterruptPendingAll( void )
{
    const U32   PEND_MASK = 0x0F << 4;

    register struct MES_SSPSPI_RegisterSet*    pRegister;
    register U32    ReadValue;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    ReadValue = pRegister->STATE;

    ReadValue |= PEND_MASK;

    pRegister->STATE = (U16)ReadValue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_SSPSPI_GetInterruptNumber,       MES_SSPSPI_SetInterruptEnable,
 *              MES_SSPSPI_GetInterruptEnable,       MES_SSPSPI_GetInterruptPending,
 *              MES_SSPSPI_ClearInterruptPending,    MES_SSPSPI_SetInterruptEnableAll,
 *              MES_SSPSPI_GetInterruptEnableAll,    MES_SSPSPI_GetInterruptPendingAll,
 *              MES_SSPSPI_ClearInterruptPendingAll
 */
S32     MES_SSPSPI_GetInterruptPendingNumber( void )  // -1 if None
{
    const U32 IRQR_MASK     = 1UL << 4;
    const U32 IRQW_MASK     = 1UL << 5;
    const U32 IRQE_MASK     = 1UL << 6;
    const U32 IRQERR_MASK   = 1UL << 7;

    register U32 Pend;

    MES_ASSERT( CNULL != __g_pRegister );

    Pend    =   __g_pRegister->STATE;

    if( Pend & IRQR_MASK )
    {
        return 0;
    }
    else if( Pend & IRQW_MASK )
    {
        return 1;
    }
    else if( Pend & IRQE_MASK )
    {
        return 2;
    }
    else if( Pend & IRQERR_MASK )
    {
        return 3;
    }

    return -1;
}


//------------------------------------------------------------------------------
// DMA Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief  Get DMA peripheral index of Tx
 *  @return DMA peripheral index of Tx
 *  @see    MES_SSPSPI_GetDMAIndex_Rx, MES_SSPSPI_GetDMABusWidth
 */
U32     MES_SSPSPI_GetDMAIndex_Tx( void )
{
    const U32   SspspiDmaIndexTx[NUMBER_OF_SSPSPI_MODULE] = {

         DMAINDEX_OF_SSPSPI0_MODULE_TX, DMAINDEX_OF_SSPSPI1_MODULE_TX };

    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE >> __g_CurModuleIndex );

    return SspspiDmaIndexTx[__g_CurModuleIndex];
}

/**
 *  @brief  Get DMA peripheral index of Rx
 *  @return DMA peripheral index of Rx
 *  @see    MES_SSPSPI_GetDMAIndex_Tx, MES_SSPSPI_GetDMABusWidth
 */
U32     MES_SSPSPI_GetDMAIndex_Rx( void )
{
    const U32   SspspiDmaIndexRx[NUMBER_OF_SSPSPI_MODULE] = {

         DMAINDEX_OF_SSPSPI0_MODULE_RX, DMAINDEX_OF_SSPSPI1_MODULE_RX };

    MES_ASSERT( NUMBER_OF_SSPSPI_MODULE >> __g_CurModuleIndex );

    return SspspiDmaIndexRx[__g_CurModuleIndex];
}

/**
 *  @brief      Get bus width of SSP/SPI
 *  @return     DMA bus width of SSP/SPI \n.
 *              8 ( 1 <= Data bit width <= 8 ), 16 ( 9 <= Data bit width <= 16 )\n.
 *  @see        MES_SSPSPI_GetDMAIndex_Tx, MES_SSPSPI_GetDMAIndex_Rx, MES_SSPSPI_GetDMAIndexPCMOut
 */
U32     MES_SSPSPI_GetDMABusWidth( void )
{
    if( 9 <= MES_SSPSPI_GetBitWidth() )
    {
        MES_ASSERT(MES_SSPSPI_GetBitWidth()<=16);
        return 16;
    }
    else
    {
        return 8;
    }
}


//------------------------------------------------------------------------------
// Clock Control Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Set a PCLK mode
 *  @param[in]  mode    PCLK mode
 *  @return     None.
 *  @see        MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,         MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_SetClockDivisor,        MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable,  MES_SSPSPI_GetClockDivisorEnable
 */
void            MES_SSPSPI_SetClockPClkMode( MES_PCLKMODE mode )
{
    const U32 PCLKMODE_POS  =   3;

    register U32 regvalue;
    register struct MES_SSPSPI_RegisterSet* pRegister;

	U32 clkmode=0;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	switch(mode)
	{
    	case MES_PCLKMODE_DYNAMIC:  clkmode = 0;		break;
    	case MES_PCLKMODE_ALWAYS:  	clkmode = 1;		break;
    	default: MES_ASSERT( CFALSE );
	}

    regvalue = pRegister->CLKENB;

    regvalue &= ~(1UL<<PCLKMODE_POS);
    regvalue |= ( clkmode & 0x01 ) << PCLKMODE_POS;

    pRegister->CLKENB = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get current PCLK mode
 *  @return     Current PCLK mode
 *  @see        MES_SSPSPI_SetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,         MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_SetClockDivisor,        MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable,  MES_SSPSPI_GetClockDivisorEnable
 */
MES_PCLKMODE    MES_SSPSPI_GetClockPClkMode( void )
{
    const U32 PCLKMODE_POS  = 3;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->CLKENB & ( 1UL << PCLKMODE_POS ) )
    {
        return MES_PCLKMODE_ALWAYS;
    }

    return  MES_PCLKMODE_DYNAMIC;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set clock source of clock generator
 *  @param[in]  Index   Select clock generator( 0 : clock generator 0  );
 *  @param[in]  ClkSrc  Select clock source of clock generator ( 0:PLL0, 1:PLL1 ).\n
 *  @remarks    SSP/SPI have one clock generator. so \e Index must set to 0.
 *  @return     None.
 *  @see        MES_SSPSPI_SetClockPClkMode,       MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_SetClockDivisor,        MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable,  MES_SSPSPI_GetClockDivisorEnable
 */
void    MES_SSPSPI_SetClockSource( U32 Index, U32 ClkSrc )
{
    const U32 CLKSRCSEL_POS    = 1;
    const U32 CLKSRCSEL_MASK   = 0x07 << CLKSRCSEL_POS;

    register U32 ReadValue;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( 2 > ClkSrc );
    MES_ASSERT( CNULL != __g_pRegister );

    ReadValue = __g_pRegister->CLKGEN;

    ReadValue &= ~CLKSRCSEL_MASK;
    ReadValue |= ClkSrc << CLKSRCSEL_POS;

    __g_pRegister->CLKGEN = ReadValue;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Get clock source of specified clock generator.
 *  @param[in]  Index   Select clock generator( 0 : clock generator 0 );
 *  @return     Clock source of clock generator ( 0:PLL0, 1:PLL1 ) \n
 *  @remarks    SSP/SPI have one clock generator. so \e Index must set to 0.
 *  @see        MES_SSPSPI_SetClockPClkMode,       MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,
 *              MES_SSPSPI_SetClockDivisor,        MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable,  MES_SSPSPI_GetClockDivisorEnable
 */
U32             MES_SSPSPI_GetClockSource( U32 Index )
{
    const U32 CLKSRCSEL_POS    = 1;
    const U32 CLKSRCSEL_MASK   = 0x07 << CLKSRCSEL_POS;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( CNULL != __g_pRegister );

    return ( __g_pRegister->CLKGEN & CLKSRCSEL_MASK ) >> CLKSRCSEL_POS;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set clock divisor of specified clock generator.
 *  @param[in]  Index       Select clock generator( 0 : clock generator 0 );
 *  @param[in]  Divisor     Clock divisor ( 1 ~ 64 )
 *  @return     None.
 *  @remarks    SSP/SPI have one clock generator. so \e Index must set to 0.
 *  @see        MES_SSPSPI_SetClockPClkMode,       MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,         MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable,  MES_SSPSPI_GetClockDivisorEnable
 */
void            MES_SSPSPI_SetClockDivisor( U32 Index, U32 Divisor )
{
    const U32 CLKDIV_POS   =   4;
    const U32 CLKDIV_MASK  =   0x3F << CLKDIV_POS;

    register U32 ReadValue;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( 1 <= Divisor && Divisor <= 64 );
    MES_ASSERT( CNULL != __g_pRegister );

    ReadValue   =   __g_pRegister->CLKGEN;

    ReadValue   &= ~CLKDIV_MASK;
    ReadValue   |= (Divisor-1) << CLKDIV_POS;

    __g_pRegister->CLKGEN = ReadValue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get clock divisor of specified clock generator.
 *  @param[in]  Index       Select clock generator( 0 : clock generator 0 );
 *  @return     Clock divisor ( 1 ~ 64 )
 *  @remarks    SSP/SPI have one clock generator. so \e Index must set to 0.
 *  @see        MES_SSPSPI_SetClockPClkMode,       MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,         MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_SetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable,  MES_SSPSPI_GetClockDivisorEnable
 */
U32             MES_SSPSPI_GetClockDivisor( U32 Index )
{
    const U32 CLKDIV_POS   =   4;
    const U32 CLKDIV_MASK  =   0x3F << CLKDIV_POS;

    MES_ASSERT( 0 == Index );
    MES_ASSERT( CNULL != __g_pRegister );

    return ((__g_pRegister->CLKGEN & CLKDIV_MASK) >> CLKDIV_POS) + 1;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Set clock generator's operation
 *  @param[in]  Enable  \b CTRUE indicate that Enable of clock generator. \n
 *                      \b CFALSE indicate that Disable of clock generator.
 *  @return     None.
 *  @see        MES_SSPSPI_SetClockPClkMode,       MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,         MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_SetClockDivisor,        MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_GetClockDivisorEnable
 */
void            MES_SSPSPI_SetClockDivisorEnable( CBOOL Enable )
{
    const U32   CLKGENENB_POS   =   2;
    const U32   CLKGENENB_MASK  =   1UL << CLKGENENB_POS;

    register U32 ReadValue;

    MES_ASSERT( (0==Enable) ||(1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    ReadValue   =   __g_pRegister->CLKENB;

    ReadValue   &=  ~CLKGENENB_MASK;
    ReadValue   |= (U32)Enable << CLKGENENB_POS;

    __g_pRegister->CLKENB   =   ReadValue;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Get status of clock generator's operation
 *  @return     \b CTRUE indicate that Clock generator is enabled.  \n
 *              \b CFALSE indicate that Clock generator is disabled.
 *  @see        MES_SSPSPI_SetClockPClkMode,       MES_SSPSPI_GetClockPClkMode,
 *              MES_SSPSPI_SetClockSource,         MES_SSPSPI_GetClockSource,
 *              MES_SSPSPI_SetClockDivisor,        MES_SSPSPI_GetClockDivisor,
 *              MES_SSPSPI_SetClockDivisorEnable
 */
CBOOL           MES_SSPSPI_GetClockDivisorEnable( void )
{
    const U32   CLKGENENB_POS   =   2;
    const U32   CLKGENENB_MASK  =   1UL << CLKGENENB_POS;

    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->CLKENB & CLKGENENB_MASK) >> CLKGENENB_POS );
}


//--------------------------------------------------------------------------
// Configuration Function
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief       Set TXD Pin's state
 * @param[in]   bTXD    \b CTRUE indicate that TXD pin as Hi-Z when transmission is not performed. \n
 *                      \b CFALSE indicate that TXD pin is always output pin.
 * @return      None.
 * @see                                             MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetHiZTXD( CBOOL bTXD )
{
    const U32    ZENB_MASK     = ( 0x01 << 14 );
    register U32    temp;

    MES_ASSERT( CNULL != __g_pRegister );

    temp = __g_pRegister->CONT0;

    if( CTRUE == bTXD )
    {
        temp |= ZENB_MASK;
    }
    else
    {
        temp &= ~ZENB_MASK;
    }

    __g_pRegister->CONT0 = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get TXD Pin's state
 * @return      \b CTRUE indicate that TXD pin as Hi-Z when transmission is not performed. \n
 *              \b CFALSE indicate that TXD pin is always output pin.
 * @see         MES_SSPSPI_SetHiZTXD,               
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
CBOOL   MES_SSPSPI_GetHiZTXD( void )
{
    const U32    ZENB_MASK     = ( 0x01 << 14 );

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->CONT0 & ZENB_MASK ){ return CTRUE; }
    else                                  { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Transfer Mode ( DMA mode or PIO mode )
 * @param[in]   bDMA    \b CTRUE indicate that DMA mode. \n
 *                      \b CFALSE indicate that PIO mode.
 * @return      None.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *                                                  MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetDMATransferMode( CBOOL bDMA )
{
    const U32    DMAENB_POS   = 12;
    const U32    DMAENB_MASK  = 1UL << DMAENB_POS;

    register U32    temp;

    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( (0==bDMA) || (1==bDMA) );

    temp = __g_pRegister->CONT0;

    temp &= ~DMAENB_MASK;
    temp |= (U32)bDMA << DMAENB_POS;

    __g_pRegister->CONT0 = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Transfer Mode ( DMA mode or PIO mode )
 * @return      \b CTRUE indicate that DMA mode. \n
 *              \b CFALSE indicate that PIO mode.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
CBOOL   MES_SSPSPI_GetDMATransferMode( void )
{
    const U32    DMAENB_POS   = 12;
    const U32    DMAENB_MASK  = 1UL << DMAENB_POS;

    MES_ASSERT( CNULL != __g_pRegister );

    return(CBOOL)( __g_pRegister->CONT0 & DMAENB_MASK ) >> DMAENB_POS;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Clock Source of SSP/SPI
 * @param[in]   bExt    \b CTRUE indicate that External clock. \n
 *                      \b CFALSE indicate that Internal clock. \n
 * @return      None.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *                                                  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetExternalClockSource( CBOOL bExt )
{
    const U32    EXTCLKSEL_BITPOS   = 9;
    const U32    EXTCLKSEL_MASK     = ( 0x01 << EXTCLKSEL_BITPOS );

    register U32    temp;

    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( (0==bExt) || (1==bExt) );

    temp = __g_pRegister->CONT0;
    
    if( bExt )
    {
        temp |= EXTCLKSEL_MASK;
    }
    else
    {
        temp &= ~EXTCLKSEL_MASK;        
    }

    __g_pRegister->CONT0 = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Clock Source of SSP/SPI
 * @return      \b CTRUE indicate that External clock. \n
 *              \b CFALSE indicate that Internal clock. 
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
CBOOL   MES_SSPSPI_GetExternalClockSource( void )
{
    const U32    EXTCLKSEL_BITPOS   = 9;
    const U32    EXTCLKSEL_MASK     = ( 0x01 << EXTCLKSEL_BITPOS );

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->CONT0 & EXTCLKSEL_MASK ) 
    {
        return CTRUE;
    }

    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Data bit width
 * @param[in]   bitWidth    Value of Data Bit Width( 1 ~ 16 )
 * @return      None.
 * @remarks     MES_SSPSPI_SetBitWidth() must invoke before MES_SSPSPI_GetDMABusWidth()
 * @code
 *              MES_SSPSPI_GetBitWidth();
 *              MES_SSPSPI_GetDMABusWidth();
 * @endcode
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *                                                  MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetBitWidth( U32 bitWidth )
{
    const U32    NUMBIT_POS     = 5;
    const U32    NUMBIT_MASK    = ( 0x0F << NUMBIT_POS );

    register U32    temp;

    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( 1 <= bitWidth && bitWidth <= 16 );

    temp = __g_pRegister->CONT0;

    temp &= ~NUMBIT_MASK;
    temp |=  (bitWidth-1) << NUMBIT_POS ;

    __g_pRegister->CONT0 = (U16)temp;
}


//------------------------------------------------------------------------------
/**
 * @brief       Get Data Bit Width
 * @return      Data Bit Width( 1 ~ 16 )
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
U32     MES_SSPSPI_GetBitWidth( void )
{
    const U32    NUMBIT_POS     = 5;
    const U32    NUMBIT_MASK    = ( 0x0F << NUMBIT_POS );

    MES_ASSERT( CNULL != __g_pRegister );

    return(U32) (((__g_pRegister->CONT0 & NUMBIT_MASK ) >> NUMBIT_POS) + 1 );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Divide Value
 * @param[in]   dividerCNT  Divide Value ( 4 ~ 32 )
 * @return      None.
 * @remarks     Recommand Even Value ( 4, 6 ~ ).\n
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *                                                  MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetDividerCount( U32 dividerCNT )
{
    const U32    DIVCNT_MASK     = 0x1F;
    register U32    temp;

    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( ( 32 >= dividerCNT ) && ( 2 <= dividerCNT ) );
    
    temp = __g_pRegister->CONT0;
    
    temp &= ~DIVCNT_MASK;
    temp |= (dividerCNT-1);
    
    __g_pRegister->CONT0 = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Divide Value
 * @return  Divide Value 
 * @see     MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *          MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *          MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *          MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *          MES_SSPSPI_SetDividerCount,         
 *          MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *          MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *          MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *          MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *          MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
U32     MES_SSPSPI_GetDividerCount( void )
{
    const U32    DIVCNT_MASK     = 0x1F;

    MES_ASSERT( CNULL != __g_pRegister );
    
    return(U32)( __g_pRegister->CONT0 & DIVCNT_MASK )+1;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set location of data byte in 16 bit FIFO. 
 * @param[in]   bSwap   \b CTRUE indicate that Byte swap Enable.\n
 *                      \b CFALSE indicate that Byte swap Disable.
 * @return      None.
 * @remarks     It allows using bandwidth more efficiently in 8bit reception device.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *                                                  MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetByteSwap( CBOOL bSwap )
{
    const U32   BYTE_SWAP_POS   = 5;
    const U32   BYTE_SWAP_MASK  = 1UL << BYTE_SWAP_POS;
    register U32   ReadValue;
    
    MES_ASSERT( (0==bSwap) || (1==bSwap) );
    MES_ASSERT( CNULL != __g_pRegister );

    ReadValue   =   __g_pRegister->CONT1;
    
    ReadValue   &= ~BYTE_SWAP_MASK;
    ReadValue   |= (U32)bSwap << BYTE_SWAP_POS;
    
    __g_pRegister->CONT1    =   (U16)ReadValue;
}

//------------------------------------------------------------------------------
/**
 * @brief      Get status of byte swap is enabled or disabled.
 * @return     \b CTRU indicate that Byte swap is enabled.\n
 *             \b CFALSE indicate that Byte swap is disabled. 
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
CBOOL   MES_SSPSPI_GetByteSwap( void )
{
    const U32   BYTE_SWAP_POS   = 5;
    const U32   BYTE_SWAP_MASK  = 1UL << BYTE_SWAP_POS;

    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)((__g_pRegister->CONT1 & BYTE_SWAP_MASK) >> BYTE_SWAP_POS );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Operation Mode ( Master or Slave )
 * @param[in]   bSlave    \b CTRIE indicate that Slave mode enable. \n
 *                        \b CFALSE indicate that Master mode enable.
 * @return      None.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *                                                  MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetSlaveMode( CBOOL bSlave )
{
    const U32    SLAVE_SEL_POS   = 4;
    const U32    SLAVE_SEL_MASK  = ( 0x01 << SLAVE_SEL_POS );
    register U32    temp;

    MES_ASSERT( (0==bSlave) || (1==bSlave) );
    MES_ASSERT( CNULL != __g_pRegister );

    temp = __g_pRegister->CONT1;
    
    if( bSlave )
    {
        temp    |= SLAVE_SEL_MASK;
    }
    else
    {
        temp    &= ~SLAVE_SEL_MASK;        
    }
    
    __g_pRegister->CONT1 = (U16)temp;    
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Operation Mode ( Master or Slave )
 * @return      \b CTRIE indicate that Slave mode enable. \n
 *              \b CFALSE indicate that Master mode enable.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
CBOOL   MES_SSPSPI_GetSlaveMode( void )
{
    const U32    SLAVE_SEL_POS   = 4;
    const U32    SLAVE_SEL_MASK  = ( 0x01 << SLAVE_SEL_POS );

    MES_ASSERT( CNULL != __g_pRegister );
    
    if( __g_pRegister->CONT1 & SLAVE_SEL_MASK )
    {
        return CTRUE;   
    }

    return CFALSE;       

}

//------------------------------------------------------------------------------
/**
 * @brief       Set Polarity of SSPCLK
 * @param[in]   bInvert    \b CTRUE indicate that SSPCLK Invert enable. \n
 *                         \b CFALSE indicate that SSPCLK Invert disable. \n 
 * @return      None.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *                                                  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void    MES_SSPSPI_SetClockPolarityInvert( CBOOL bInvert )
{
    const U32    SCLKPOL_POS   = 3;
    const U32    SCLKPOL_MASK  = ( 0x01 << SCLKPOL_POS );
    register U32    temp;

    MES_ASSERT( (0==bInvert) || (1==bInvert) );
    MES_ASSERT( CNULL != __g_pRegister );
    
    temp = __g_pRegister->CONT1;
    
    if( bInvert )
    {
        temp    &= ~SCLKPOL_MASK;
    }
    else
    {
        temp    |= SCLKPOL_MASK;        
    }
    
    __g_pRegister->CONT1 = (U16)temp;    
}

//------------------------------------------------------------------------------
/**
 * @brief       Get status of SSPCLK's polarity is invert or normal.
 * @return      \b CTRUE indicate that SSPCLK's polarity is invert. \n
 *              \b CFALSE indicate that SSPCLK's polarity is normal. 
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
CBOOL   MES_SSPSPI_GetClockPolarityInvert( void )
{
    const U32    SCLKPOL_POS   = 3;
    const U32    SCLKPOL_MASK  = ( 0x01 << SCLKPOL_POS );
    
    MES_ASSERT( CNULL != __g_pRegister );    
    
    if( __g_pRegister->CONT1 & SCLKPOL_MASK )
    {
        return CFALSE;   
    }

    return CTRUE;           
}

//------------------------------------------------------------------------------
/**
 * @brief       Set data size,in bytes, for burst receive mode.
 * @param[in]   size    Byte size ( 1 ~ 65536 )   
 * @return      None.
 * @remarks     This setting use to Rx Burst Receive in master mode of SPI's Format B protocol.
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *                                           MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */             
void    MES_SSPSPI_SetBurstRxSize( U32 size )
{               
    MES_ASSERT( 1 <= size && size <= 65536 );
    MES_ASSERT( CNULL != __g_pRegister );    
    
    __g_pRegister->RXBUSTSIZE = (U16)size-1;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get data size,in bytes, for burst receive mode.
 * @return      Byte size ( 1 ~ 65536 )   
 * @remarks     This setting use to Rx Burst Receive in master mode of SPI's Format B protocol.
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
U32     MES_SSPSPI_GetBurstRxSize( void )
{
    MES_ASSERT( CNULL != __g_pRegister );    
    
    return (U32)__g_pRegister->RXBUSTSIZE + 1;
}

//------------------------------------------------------------------------------
/**
 * @brief      Set SPI format
 * @param[in]  format     SPI format ( MES_SSPSPI_FORMAT_A or MES_SSPSPI_FORMAT_B )
 * @return     None.
 * @see        MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *             MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *             MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *             MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *             MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *             MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *             MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *             MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *                                                 MES_SSPSPI_GetSPIFormat,
 *             MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
void                MES_SSPSPI_SetSPIFormat( MES_SSPSPI_FORMAT format)
{
    const U32    SCLKSH_POS  = 2;
    const U32    SCLKSH_MASK = ( 0x01 << SCLKSH_POS );
    register U32    temp;

    MES_ASSERT( 2 > format );
    MES_ASSERT( CNULL != __g_pRegister );
    
    temp = __g_pRegister->CONT1;
    
    if( MES_SSPSPI_FORMAT_A == format )
    {
        temp &= ~SCLKSH_MASK;
    }
    else
    {
        temp |= SCLKSH_MASK;        
    }
    
    __g_pRegister->CONT1 = (U16)temp;    
}

//------------------------------------------------------------------------------
/**
 * @brief      Get SPI format
 * @return     SPI format ( MES_SSPSPI_FORMAT_A or MES_SSPSPI_FORMAT_B )
 * @see        MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *             MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *             MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *             MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *             MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *             MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *             MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *             MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *             MES_SSPSPI_SetSPIFormat,            
 *             MES_SSPSPI_SetProtocol,             MES_SSPSPI_GetProtocol
 */
MES_SSPSPI_FORMAT   MES_SSPSPI_GetSPIFormat( void )
{
    const U32    SCLKSH_POS  = 2;
    const U32    SCLKSH_MASK = ( 0x01 << SCLKSH_POS );

    MES_ASSERT( CNULL != __g_pRegister );
    
    if(__g_pRegister->CONT1 & SCLKSH_MASK ) 
    {
        return MES_SSPSPI_FORMAT_B;
    }

    return MES_SSPSPI_FORMAT_A;    
}

//------------------------------------------------------------------------------
/**
 * @brief       Set SSP/SPI Mode
 * @param[in]   protocol    Mode ( MES_SSPSPI_PROTOCOL_SSP or MES_SSPSPI_PROTOCOL_SPI )
 * @return      None.
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *                                                  MES_SSPSPI_GetProtocol
 */
void                MES_SSPSPI_SetProtocol( MES_SSPSPI_PROTOCOL protocol )
{
    const U32    TYPE_POS       = 0;
    const U32    TYPE_MASK      = ( 0x03 << TYPE_POS );
    register U32    temp;

    MES_ASSERT( 2 > protocol );
    MES_ASSERT( CNULL != __g_pRegister );
    
    temp = __g_pRegister->CONT1;
    
    if( MES_SSPSPI_PROTOCOL_SSP == protocol )
    {
        temp &= ~TYPE_MASK;
    }
    else
    {
        temp |= TYPE_MASK;        
    }
    
    __g_pRegister->CONT1 = (U16)temp;    
}

//------------------------------------------------------------------------------
/**
 * @brief       Get SSP/SPI Mode
 * @return      Mode ( MES_SSPSPI_PROTOCOL_SSP or MES_SSPSPI_PROTOCOL_SPI )
 * @see         MES_SSPSPI_SetHiZTXD,               MES_SSPSPI_GetHiZTXD,
 *              MES_SSPSPI_SetDMATransferMode,      MES_SSPSPI_GetDMATransferMode,
 *              MES_SSPSPI_SetExternalClockSource,  MES_SSPSPI_GetExternalClockSource,
 *              MES_SSPSPI_SetBitWidth,             MES_SSPSPI_GetBitWidth,
 *              MES_SSPSPI_SetDividerCount,         MES_SSPSPI_GetDividerCount,
 *              MES_SSPSPI_SetByteSwap,             MES_SSPSPI_GetByteSwap,
 *              MES_SSPSPI_SetSlaveMode,            MES_SSPSPI_GetSlaveMode,
 *              MES_SSPSPI_SetClockPolarityInvert,  MES_SSPSPI_GetClockPolarityInvert,
 *              MES_SSPSPI_SetSPIFormat,            MES_SSPSPI_GetSPIFormat,
 *              MES_SSPSPI_SetProtocol             
 */
MES_SSPSPI_PROTOCOL MES_SSPSPI_GetProtocol( void )
{
    const U32    TYPE_POS       = 0;
    const U32    TYPE_MASK      = ( 0x03 << TYPE_POS );

    MES_ASSERT( CNULL != __g_pRegister );

    if(__g_pRegister->CONT1 & TYPE_MASK )
    {
        return MES_SSPSPI_PROTOCOL_SPI;
    }

    return MES_SSPSPI_PROTOCOL_SSP;
}



//--------------------------------------------------------------------------
// Operation Function
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief       FIFO Reset
 * @return      None.
 * @remarks     Reset TX & RX FIFO.
 * @see                                      MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
void      MES_SSPSPI_ResetFIFO( void )
{
    const U32    FFCLR_MASK     = ( 0x01 << 10 );

    register struct MES_SSPSPI_RegisterSet* pRegister;
    register U32    temp;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;
    
    temp = pRegister->CONT0;
    // IMPORTANT : You have to clear this bit after writing '1'.
    pRegister->CONT0 = (U16)(temp | FFCLR_MASK);
    pRegister->CONT0 = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Start RX Burst receive.
 * @return      None.
 * @code
 *         ...
 *         MES_SSPSPI_SetSlaveMode(CFALSE);                 // Master Mode
 *         MES_SSPSPI_SetProtocol(MES_SSPSPI_PROTOCOL_SPI); // SPI Protocol
 *         MES_SSPSPI_SetSPIFormat( MES_SSPSPI_FORMAT_B );  // SPI Format B
 *         ...
 *         MES_SSPSPI_SetBurstRxSize( n );                  // Set N byte
 *         ...
 *         MES_SSPSPI_SetEnable( CTRUE );                  // SPI Enable
 *         MES_SSPSPI_SetRxBurstEnable( CTRUE );           // Burst Rx Enable
 *         MES_SSPSPI_StartRxBurst( );                     // Burst Rx Start
 *         ...
 * @endcode
 * @remarks     Start Rx Burst Receiving in master mode of SPI's Format B protocol.
 * @see         MES_SSPSPI_ResetFIFO,        
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */             
void      MES_SSPSPI_StartRxBurst( void )
{
    const    U32    ST_RXBRECEIVE_MASK  =   1UL << 15;
    register U32    ReadValue;
    register struct MES_SSPSPI_RegisterSet* pRegister;
    
    MES_ASSERT( CNULL != __g_pRegister );
    
    pRegister   =   __g_pRegister;

    ReadValue   =   pRegister->CONT0;
    
    ReadValue   |= ST_RXBRECEIVE_MASK;
    
    pRegister->CONT0 = ReadValue;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Rx burst enable.
 * @param[in]   bEnable  \b CTRUE indicate that Burst Receive Enable. \n
 *                       \b CFALSE indicate that Burst Receive Disable.
 * @return      None.
 * @remarks     It's meanful when master mode. It maintains low status until SSPFRM
 *              signal receive BURST_SZ all. It only supports Format B (SPI).
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *                                           MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
void      MES_SSPSPI_SetRxBurstEnable( CBOOL bEnable )
{
    const U32 BRECEIVE_POS   = 13;
    const U32 BRECEIVE_MASK  = 1UL << BRECEIVE_POS;
    
    register struct MES_SSPSPI_RegisterSet* pRegister;
    register U32 ReadValue;
    
    MES_ASSERT( (0==bEnable) ||(1==bEnable) );
    MES_ASSERT( CNULL != __g_pRegister );    
    
    pRegister   =   __g_pRegister;
    
    ReadValue   =   pRegister->CONT0;
    
    ReadValue   &=  ~BRECEIVE_MASK;
    ReadValue   |=  (U32)bEnable << BRECEIVE_POS;    
    
    pRegister->CONT0  =   ReadValue;        
}

//------------------------------------------------------------------------------
/**
 * @brief       Get status of Rx burst enabled or disabled.
 * @return      \b CTRUE indicate that Burst Receive is enabled.\n
 *              \b CFALSE indicate that Burst Receive is disabled.
 * @remarks     It's meanful when master mode. It maintains low status until SSPFRM
 *              signal receive BURST_SZ all. It only supports Format B (SPI).
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, 
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
CBOOL     MES_SSPSPI_GetRxBurstEnable( void )
{
    const U32 BRECEIVE_POS   = 13;
    const U32 BRECEIVE_MASK  = 1UL << BRECEIVE_POS;

    MES_ASSERT( CNULL != __g_pRegister );    
    
    return  (CBOOL)( (__g_pRegister->CONT0 & BRECEIVE_MASK) >> BRECEIVE_POS );        
}

//------------------------------------------------------------------------------
/**
 * @brief       Set SSP/SPI enable or disable.
 * @param[in]   bEnable     \b CTRUE indicate that SSP/SPI Enable. \n
 *                          \b CFALSE indicate that SSP/SPI Disable. 
 * @return      None.
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *                                           MES_SSPSPI_GetEnable,
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
void      MES_SSPSPI_SetEnable( CBOOL bEnable )
{
    const U32 ENB_POS   = 11;
    const U32 ENB_MASK  = 1UL << ENB_POS;
    
    register struct MES_SSPSPI_RegisterSet* pRegister;
    register U32 ReadValue;
    
    MES_ASSERT( (0==bEnable) ||(1==bEnable) );
    MES_ASSERT( CNULL != __g_pRegister );    
    
    pRegister   =   __g_pRegister;
    
    ReadValue   =   pRegister->CONT0;
    
    ReadValue   &=  ~ENB_MASK;
    ReadValue   |=  (U32)bEnable << ENB_POS;    
    
    pRegister->CONT0  =   ReadValue;        
}

//------------------------------------------------------------------------------
/**
 * @brief       Get status of SSP/SPI is enabled or disabled.
 * @return      \b CTRUE indicate that SSP/SPI is enabled. \n
 *              \b CFALSE indicate that SSP/SPI is disabled.
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        
 *              MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
CBOOL     MES_SSPSPI_GetEnable( void )
{
    const U32 ENB_POS   = 11;
    const U32 ENB_MASK  = 1UL << ENB_POS;
    
    MES_ASSERT( CNULL != __g_pRegister );
    
    return  (CBOOL)( (__g_pRegister->CONT1 & ENB_MASK) >> ENB_POS );    
}

//------------------------------------------------------------------------------
/**
 * @brief       Read Byte Data
 * @return      Byte Data
 * @see         MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *              MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *              MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *              MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *                                           MES_SSPSPI_GetHalfWord,
 *              MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
U8        MES_SSPSPI_GetByte(void)
{
  	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_SSPSPI_GetEnable() );
	MES_ASSERT( 8 >= MES_SSPSPI_GetBitWidth() );

	return (U8)((__g_pRegister->DATA) & 0xff);
}

//------------------------------------------------------------------------------
/**
 * @brief      Read Half Word Data( 2byte )
 * @return     Half Word Data( 2byte )
 * @see        MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *             MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *             MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *             MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *             MES_SSPSPI_GetByte,          
 *             MES_SSPSPI_PutByte,          MES_SSPSPI_PutHalfWord
 */
U16       MES_SSPSPI_GetHalfWord(void)
{
    MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CTRUE == MES_SSPSPI_GetEnable() );
	MES_ASSERT( 9 <= MES_SSPSPI_GetBitWidth() );

	return ( __g_pRegister->DATA );
}

//------------------------------------------------------------------------------
/**
 * @brief      Send Byte
 * @param[in]  ByteData     Value of Data ( 0x0 ~ 0xFF )
 * @return     None.
 * @see        MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *             MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *             MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *             MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *             MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *             MES_SSPSPI_PutHalfWord
 */
void      MES_SSPSPI_PutByte(U8 ByteData)
{
    MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 8 >= MES_SSPSPI_GetBitWidth() );

	__g_pRegister->DATA = (U16)ByteData;	//put the byte data
}

//------------------------------------------------------------------------------
/**
 * @brief      Send Half Word( 2byte )
 * @param[in]  HalfWordData    Value of Data ( 0x0 ~ 0xFFFF )
 * @return     None.
 * @see        MES_SSPSPI_ResetFIFO,        MES_SSPSPI_StartRxBurst,
 *             MES_SSPSPI_SetRxBurstEnable, MES_SSPSPI_GetRxBurstEnable,
 *             MES_SSPSPI_SetBurstRxSize,   MES_SSPSPI_GetBurstRxSize,
 *             MES_SSPSPI_SetEnable,        MES_SSPSPI_GetEnable,
 *             MES_SSPSPI_GetByte,          MES_SSPSPI_GetHalfWord,
 *             MES_SSPSPI_PutByte
 */
void      MES_SSPSPI_PutHalfWord(U16 HalfWordData)
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 9 <= MES_SSPSPI_GetBitWidth() );

	__g_pRegister->DATA = HalfWordData;
}


//--------------------------------------------------------------------------
// FIFO State Check Function
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief      Check Tx Shift Register is Empty or Not
 * @return     \b CTRUE Indicate that Tx Shift Register is Empty.\n
 *             \b CFALSE Indicate that Tx Shift Register is NOT Empty.
 * @see        MES_SSPSPI_IsTxFIFOFull,    MES_SSPSPI_IsTxFIFOEmpty,
 *             MES_SSPSPI_IsRxFIFOFull,    MES_SSPSPI_IsRxFIFOEmpty,
 *             MES_SSPSPI_IsTxRxEnd,
 */            
CBOOL     MES_SSPSPI_IsTxShiftEmpty( void )
{              
    const U32    TXSHIFTEREMPTY_POS = 8;

    MES_ASSERT( CNULL != __g_pRegister );
   
    return (CBOOL)(( __g_pRegister->STATE >> TXSHIFTEREMPTY_POS ) & 0x01);
}

//------------------------------------------------------------------------------
/**
 * @brief      Check Tx FIFO is Full or Not
 * @return     \b CTRUE Indicate that FIFO Full.\n
 *             \b CFALSE Indicate that FIFO NOT Full.
 * @see        MES_SSPSPI_IsTxShiftEmpty,
 *             MES_SSPSPI_IsTxFIFOEmpty,
 *             MES_SSPSPI_IsRxFIFOFull,    MES_SSPSPI_IsRxFIFOEmpty,
 *             MES_SSPSPI_IsTxRxEnd,
 */
CBOOL     MES_SSPSPI_IsTxFIFOFull(void)
{
    const U32    WFFFULL_POS = 3;

    MES_ASSERT( CNULL != __g_pRegister );
   
    return(CBOOL)((__g_pRegister->STATE >> WFFFULL_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 * @brief      Check Tx FIFO is Empty or Not
 * @return     \b CTRUE Indicate that FIFO Empty.\n
 *             \b CFALSE Indicate that FIFO NOT Empty
 * @see        MES_SSPSPI_IsTxShiftEmpty,
 *             MES_SSPSPI_IsTxFIFOFull,    
 *             MES_SSPSPI_IsRxFIFOFull,    MES_SSPSPI_IsRxFIFOEmpty,
 *             MES_SSPSPI_IsTxRxEnd,
 */
CBOOL     MES_SSPSPI_IsTxFIFOEmpty(void)
{
    const U32    WFFEMPTY_POS     = 2;

    MES_ASSERT( CNULL != __g_pRegister );
   
    return(CBOOL)((__g_pRegister->STATE >> WFFEMPTY_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 * @brief      Check Rx FIFO is Full or Not
 * @return     \b CTRUE Indicate that FIFO Full.\n
 *             \b CFALSE Indicate that FIFO NOT Full.
 * @see        MES_SSPSPI_IsTxShiftEmpty,
 *             MES_SSPSPI_IsTxFIFOFull,    MES_SSPSPI_IsTxFIFOEmpty,
 *             MES_SSPSPI_IsRxFIFOEmpty,
 *             MES_SSPSPI_IsTxRxEnd,
 */
CBOOL     MES_SSPSPI_IsRxFIFOFull(void)
{
    const U32    RFFFULL_POS = 1;

    MES_ASSERT( CNULL != __g_pRegister );
    
    return(CBOOL)((__g_pRegister->STATE >> RFFFULL_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 * @brief      Check Rx FIFO is Empty or Not
 * @return     \b CTRUE Indicate that FIFO Empty.\n
 *             \b CFALSE Indicate that FIFO NOT Empty.
 * @see        MES_SSPSPI_IsTxShiftEmpty,
 *             MES_SSPSPI_IsTxFIFOFull,    MES_SSPSPI_IsTxFIFOEmpty,
 *             MES_SSPSPI_IsRxFIFOFull,    
 *             MES_SSPSPI_IsTxRxEnd,
 */
CBOOL     MES_SSPSPI_IsRxFIFOEmpty(void)
{
    const U32    RFFEMPTY_POS     = 0;

    MES_ASSERT( CNULL != __g_pRegister );
    
    return(CBOOL)((__g_pRegister->STATE >> RFFEMPTY_POS ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 * @brief      Check Tx/Rx is End or Not
 * @return     \b CTRUE Indicate that Tx/Rx is End.\n
 *             \b CFALSE Indicate that Tx/Rx is NOT End.
 * @remarks    This function is same to MES_SSPSPI_GetInterruptPending(2)
 * @see        MES_SSPSPI_IsTxShiftEmpty,
 *             MES_SSPSPI_IsTxFIFOFull,    MES_SSPSPI_IsTxFIFOEmpty,
 *             MES_SSPSPI_IsRxFIFOFull,    MES_SSPSPI_IsRxFIFOEmpty
 */
CBOOL     MES_SSPSPI_IsTxRxEnd( void )
{
    const U32    IRQE_POS = 6;

    MES_ASSERT( CNULL != __g_pRegister );

    return(CBOOL)((__g_pRegister->STATE >> IRQE_POS ) & 0x01 );
}





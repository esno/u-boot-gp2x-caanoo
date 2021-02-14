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
//	Module     : UART
//	File       : mes_uart.c
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------

#include "mes_uart.h"

/// @brief  UART Module's Register List
struct  MES_UART_RegisterSet
{
	volatile U16	LCON;		    ///< 0x00 : Line Control Register
	volatile U16	UCON;		    ///< 0x02 : Control Register
	volatile U16	FCON;           ///< 0x04 : FIFO Control Register
	volatile U16	MCON;           ///< 0x06 : Modem Control Register
	volatile U16	TRSTATUS;       ///< 0x08 : Tx/Rx Status Register
	volatile U16	ESTATUS;        ///< 0x0a : Error Status Register
	volatile U16	FSTATUS;        ///< 0x0c : FIFO Status Register
	volatile U16	MSTATUS;        ///< 0x0e : Modem Status Register
	volatile U16	THB;            ///< 0x10 : Transmit Buffer Register
	volatile U16	RHB;            ///< 0x12 : Receive Buffer Register
	volatile U16	BRD;            ///< 0x14 : Baud Rate Divisor Register
	volatile U16	TIMEOUT;	    ///< 0x16 : Receive TimeOut Register
	volatile U16	INTCON;		    ///< 0x18 : Interrupt Control Register
	const    U16    __Reserved[0x13];///< 0x1A ~ 0x3E : Reserved Region
    volatile U32    CLKENB;         ///< 0x40 : Clock Enable Register
    volatile U32    CLKGEN;         ///< 0x44 : Clock Generate Register
};

#define NUMBER_UART_MODULE  (4)

static  struct
{
    struct MES_UART_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_UART_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_UART_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see    MES_UART_SelectModule,
 *          MES_UART_GetCurrentModule,   MES_UART_GetNumberOfModule
 */
CBOOL   MES_UART_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_UART_MODULE; i++ )
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
 *  @see        MES_UART_Initialize,
 *              MES_UART_GetCurrentModule,   MES_UART_GetNumberOfModule
 */
U32    MES_UART_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_UART_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_UART_Initialize,         MES_UART_SelectModule,
 *              MES_UART_GetNumberOfModule
 */
U32     MES_UART_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_UART_Initialize,         MES_UART_SelectModule,
 *              MES_UART_GetCurrentModule
 */
U32     MES_UART_GetNumberOfModule( void )
{
    return NUMBER_UART_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see        MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_SetBaseAddress,       MES_UART_GetBaseAddress,
 *              MES_UART_OpenModule,           MES_UART_CloseModule,
 *              MES_UART_CheckBusy,            MES_UART_CanPowerDown
 */
U32     MES_UART_GetPhysicalAddress( void )
{
    static const U32 UartPhysicalAddr[NUMBER_UART_MODULE] =
                    {
                        PHY_BASEADDR_UART0_MODULE, PHY_BASEADDR_UART1_MODULE,
                        PHY_BASEADDR_UART2_MODULE, PHY_BASEADDR_UART3_MODULE
                    };

    MES_ASSERT( NUMBER_UART_MODULE > __g_CurModuleIndex );

    return  (U32)UartPhysicalAddr[__g_CurModuleIndex];
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_UART_GetPhysicalAddress,
 *              MES_UART_SetBaseAddress,       MES_UART_GetBaseAddress,
 *              MES_UART_OpenModule,           MES_UART_CloseModule,
 *              MES_UART_CheckBusy,            MES_UART_CanPowerDown
 */
U32     MES_UART_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_UART_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_UART_GetPhysicalAddress,   MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_GetBaseAddress,
 *              MES_UART_OpenModule,           MES_UART_CloseModule,
 *              MES_UART_CheckBusy,            MES_UART_CanPowerDown
 */
void    MES_UART_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_UART_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_UART_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_UART_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_UART_GetPhysicalAddress,   MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_SetBaseAddress,
 *              MES_UART_OpenModule,           MES_UART_CloseModule,
 *              MES_UART_CheckBusy,            MES_UART_CanPowerDown
 */
U32     MES_UART_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_UART_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_UART_GetPhysicalAddress,   MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_SetBaseAddress,       MES_UART_GetBaseAddress,
 *              MES_UART_CloseModule,
 *              MES_UART_CheckBusy,            MES_UART_CanPowerDown
 */
CBOOL   MES_UART_OpenModule( void )
{
    register struct MES_UART_RegisterSet    *pRegister;
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

	pRegister->LCON		= 0;
	pRegister->UCON		= 0;
	pRegister->FCON		= 6; 		// FIFO Reset
	pRegister->MCON     = 0x0040;	// Full channel,UART TX/RX, No AFC
	pRegister->TIMEOUT	= 0x001F;
	pRegister->INTCON	&= 0x000F;

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_UART_GetPhysicalAddress,   MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_SetBaseAddress,       MES_UART_GetBaseAddress,
 *              MES_UART_OpenModule,
 *              MES_UART_CheckBusy,            MES_UART_CanPowerDown
 */
CBOOL   MES_UART_CloseModule( void )
{
    register struct MES_UART_RegisterSet    *pRegister;
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

	pRegister->LCON		= 0;
	pRegister->UCON		= 0;
	pRegister->FCON		= 6; 		// FIFO Reset
	pRegister->MCON     = 0x0040;	// Full channel,UART TX/RX, No AFC
	pRegister->TIMEOUT	= 0x001F;
	pRegister->INTCON	&= 0x000F;

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_UART_GetPhysicalAddress,   MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_SetBaseAddress,       MES_UART_GetBaseAddress,
 *              MES_UART_OpenModule,           MES_UART_CloseModule,
 *              MES_UART_CanPowerDown
 */
CBOOL   MES_UART_CheckBusy( void )
{
	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_UART_GetPhysicalAddress,   MES_UART_GetSizeOfRegisterSet,
 *              MES_UART_SetBaseAddress,       MES_UART_GetBaseAddress,
 *              MES_UART_OpenModule,           MES_UART_CloseModule,
 *              MES_UART_CheckBusy
 */
CBOOL   MES_UART_CanPowerDown( void )
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
 *  @see        MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
S32     MES_UART_GetInterruptNumber( void )
{
    const U32   UartInterruptNumber[NUMBER_UART_MODULE] =
                {
                    INTNUM_OF_UART0_MODULE, INTNUM_OF_UART1_MODULE,
                    INTNUM_OF_UART2_MODULE, INTNUM_OF_UART3_MODULE
                };

    return  UartInterruptNumber[__g_CurModuleIndex];
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number ( 0:Tx, 1:Rx, 2:Error, 3:Modem ).
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @see        MES_UART_GetInterruptNumber,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
void    MES_UART_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    const U32   PEND_POS    =   0;
    const U32   PEND_MASK   =   0x0F << PEND_POS;

    register struct MES_UART_RegisterSet*   pRegister;
    register U32    ReadValue;

    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

    ReadValue   =   pRegister->INTCON & ~PEND_MASK;

    ReadValue   &=   ~(1UL << (IntNum+4));
    ReadValue   |=  (U32)Enable << (IntNum+4) ;

    pRegister->INTCON   =   ReadValue;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number ( 0:Tx, 1:Rx, 2:Error, 3:Modem ).
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSE indicate that Interrupt is disabled.
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
CBOOL   MES_UART_GetInterruptEnable( S32 IntNum )
{
    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->INTCON >> (IntNum+4)) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number( 0:Tx, 1:Rx, 2:Error, 3:Modem ).
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
CBOOL   MES_UART_GetInterruptPending( S32 IntNum )
{
    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->INTCON >> IntNum) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt number( 0:Tx, 1:Rx, 2:Error, 3:Modem ).
 *  @return     None.
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
void    MES_UART_ClearInterruptPending( S32 IntNum )
{
    const U32   PEND_POS    =   0;
    const U32   PEND_MASK   =   0x0F << PEND_POS;

    register struct MES_UART_RegisterSet*   pRegister;
    register U32    ReadValue;

    MES_ASSERT( 4 > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

    ReadValue   =   pRegister->INTCON & ~PEND_MASK;

    ReadValue   |=  1UL << (IntNum) ;

    pRegister->INTCON   =   ReadValue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
void    MES_UART_SetInterruptEnableAll( CBOOL Enable )
{
    const U32   INT_MASK    =   0x0F << 4;

    register U32    SetValue;

    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    SetValue   =   0;

    if( Enable )
    {
        SetValue   |=  INT_MASK;
    }

    __g_pRegister->INTCON   =   SetValue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
CBOOL   MES_UART_GetInterruptEnableAll( void )
{
    const U32   INT_MASK    =   0x0F << 4;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->INTCON & INT_MASK )
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
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,
 *              MES_UART_ClearInterruptPendingAll, MES_UART_GetInterruptPendingNumber
 */
CBOOL   MES_UART_GetInterruptPendingAll( void )
{
    const U32   PEND_MASK    =   0x0F;

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->INTCON & PEND_MASK )
    {
        return CTRUE;
    }

    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_GetInterruptPendingNumber
 */
void    MES_UART_ClearInterruptPendingAll( void )
{
    const U32   PEND_POS    =   0;
    const U32   PEND_MASK   =   0x0F << PEND_POS;

    register struct MES_UART_RegisterSet*   pRegister;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister   =   __g_pRegister;

    pRegister->INTCON |= PEND_MASK;


}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_UART_GetInterruptNumber,       MES_UART_SetInterruptEnable,
 *              MES_UART_GetInterruptEnable,       MES_UART_GetInterruptPending,
 *              MES_UART_ClearInterruptPending,    MES_UART_SetInterruptEnableAll,
 *              MES_UART_GetInterruptEnableAll,    MES_UART_GetInterruptPendingAll,
 *              MES_UART_ClearInterruptPendingAll
 */
S32     MES_UART_GetInterruptPendingNumber( void )  // -1 if None
{
    const U32 TXPEND_MASK  = 1UL << 0;
    const U32 RXPEND_MASK  = 1UL << 1;
    const U32 ERRPEND_MASK = 1UL << 2;
    const U32 MPEND_MASK   = 1UL << 3;

    register U32 Pend;

    MES_ASSERT( CNULL != __g_pRegister );

    Pend    =   __g_pRegister->INTCON;

    if( Pend & TXPEND_MASK )
    {
        return 0;
    }
    else if( Pend & RXPEND_MASK )
    {
        return 1;        
    }
    else if( Pend & ERRPEND_MASK )
    {
        return 2;        
    }
    else if( Pend & MPEND_MASK )
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
 *  @brief      Get DMA peripheral index of UART's Tx
 *  @return     DMA peripheral index of UART's Tx
 *  @see        MES_UART_GetDMAIndex_Rx, MES_UART_GetDMABusWidth
 */
U32     MES_UART_GetDMAIndex_Tx( void )
{
    const U32 UartDmaIndexTx[NUMBER_UART_MODULE] =
                {
                    DMAINDEX_OF_UART0_MODULE_TX, DMAINDEX_OF_UART1_MODULE_TX,
                    DMAINDEX_OF_UART2_MODULE_TX, DMAINDEX_OF_UART3_MODULE_TX
                };

    MES_ASSERT( NUMBER_UART_MODULE > __g_CurModuleIndex );

    return UartDmaIndexTx[__g_CurModuleIndex];
}

/**
 *  @brief      Get DMA peripheral index of UART's Rx
 *  @return     DMA peripheral index of UART's Rx
 *  @see        MES_UART_GetDMAIndex_Tx, MES_UART_GetDMABusWidth
 */
U32     MES_UART_GetDMAIndex_Rx( void )
{
    const U32 UartDmaIndexRx[NUMBER_UART_MODULE] =
                {
                    DMAINDEX_OF_UART0_MODULE_RX, DMAINDEX_OF_UART1_MODULE_RX,
                    DMAINDEX_OF_UART2_MODULE_RX, DMAINDEX_OF_UART3_MODULE_RX
                };

    MES_ASSERT( NUMBER_UART_MODULE > __g_CurModuleIndex );

    return UartDmaIndexRx[__g_CurModuleIndex];
}

/**
 *  @brief      Get DMA bus width of UART
 *  @return     DMA bus width of UART
 *  @see        MES_UART_GetDMAIndex_Tx, MES_UART_GetDMAIndex_Rx
 */
U32     MES_UART_GetDMABusWidth( void )
{
    return 8;
}

//------------------------------------------------------------------------------
// Clock Control Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Set a PCLK mode
 *  @param[in]  mode    PCLK mode
 *  @return     None.
 *  @remarks    UART controller only support MES_PCLKMODE_ALWAYS.\n
 *              If user set to MES_PCLKMODE_DYNAMIC, then uart controller \b NOT operate.
 *  @see        MES_UART_GetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,        MES_UART_GetClockDivisor,
 *              MES_UART_SetClockDivisorEnable,  MES_UART_GetClockDivisorEnable
 */
void            MES_UART_SetClockPClkMode( MES_PCLKMODE mode )
{
    const U32 PCLKMODE_POS  =   3;

    register U32 regvalue;
    register struct MES_UART_RegisterSet* pRegister;

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
 *  @see        MES_UART_SetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,        MES_UART_GetClockDivisor,
 *              MES_UART_SetClockDivisorEnable,  MES_UART_GetClockDivisorEnable
 */
MES_PCLKMODE    MES_UART_GetClockPClkMode( void )
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
 *  @param[in]  Index   Select clock generator( 0 : clock generator 0 );
 *  @param[in]  ClkSrc  Select clock source of clock generator ( 0: PLL0, 1:PLL1 ).
 *  @remarks    UART have one clock generator. so \e Index must set to 0.
 *  @return     None.
 *  @see        MES_UART_SetClockPClkMode,       MES_UART_GetClockPClkMode,
 *              MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,        MES_UART_GetClockDivisor,
 *              MES_UART_SetClockDivisorEnable,  MES_UART_GetClockDivisorEnable
 */
void    MES_UART_SetClockSource( U32 Index, U32 ClkSrc )
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
 *  @return     Clock source of clock generator ( 0:PLL0, 1:PLL1 ).
 *  @remarks    UART have one clock generator. so \e Index must set to 0.
 *  @see        MES_UART_SetClockPClkMode,       MES_UART_GetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,        MES_UART_GetClockDivisor,
 *              MES_UART_SetClockDivisorEnable,  MES_UART_GetClockDivisorEnable
 */
U32             MES_UART_GetClockSource( U32 Index )
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
 *  @param[in]  Index       Select clock generator( 0 : clock generator 0 )
 *  @param[in]  Divisor     Clock divisor ( 1 ~ 64 ).
 *  @return     None.
 *  @remarks    UART have one clock generator. so \e Index must set to 0.
 *  @see        MES_UART_SetClockPClkMode,       MES_UART_GetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_GetClockDivisor,
 *              MES_UART_SetClockDivisorEnable,  MES_UART_GetClockDivisorEnable
 */
void            MES_UART_SetClockDivisor( U32 Index, U32 Divisor )
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
 *  @param[in]  Index       Select clock generator( 0 : clock generator 0  );
 *  @return     Clock divisor ( 1 ~ 64 ).
 *  @remarks    UART have one clock generator. so \e Index must set to 0.
 *  @see        MES_UART_SetClockPClkMode,       MES_UART_GetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,
 *              MES_UART_SetClockDivisorEnable,  MES_UART_GetClockDivisorEnable
 */
U32             MES_UART_GetClockDivisor( U32 Index )
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
 *  @see        MES_UART_SetClockPClkMode,       MES_UART_GetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,        MES_UART_GetClockDivisor,
 *              MES_UART_GetClockDivisorEnable
 */
void            MES_UART_SetClockDivisorEnable( CBOOL Enable )
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
 *  @return     \b CTRUE indicate that Clock generator is enabled. \n
 *              \b CFALSE indicate that Clock generator is disabled.
 *  @see        MES_UART_SetClockPClkMode,       MES_UART_GetClockPClkMode,
 *              MES_UART_SetClockSource,         MES_UART_GetClockSource,
 *              MES_UART_SetClockDivisor,        MES_UART_GetClockDivisor,
 *              MES_UART_SetClockDivisorEnable
 */
CBOOL           MES_UART_GetClockDivisorEnable( void )
{
    const U32   CLKGENENB_POS   =   2;
    const U32   CLKGENENB_MASK  =   1UL << CLKGENENB_POS;

    MES_ASSERT( CNULL != __g_pRegister );

    return  (CBOOL)( (__g_pRegister->CLKENB & CLKGENENB_MASK) >> CLKGENENB_POS );
}


//--------------------------------------------------------------------------
// Configuration operations
//--------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 * @brief       Enable/Disable SIR mode
 * @param[in]   bEnb    Set as CTRUE to use SIR Mode
 * @return      None.
 * @remarks     determine whether or not to use the infra-red Tx/Rx mode
 * @see                                                 MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	MES_UART_SetSIRMode( CBOOL bEnb )
{
	const U32 SIR_MODE_MASK = (1<<6);

	MES_ASSERT( CNULL != __g_pRegister );

	if ( CTRUE == bEnb ) __g_pRegister->LCON =  (U16)( __g_pRegister->LCON | SIR_MODE_MASK );
	else		         __g_pRegister->LCON &= ~SIR_MODE_MASK;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get SIR mode's state
 * @return  \b CTRUE indicate that SIR Mode is Enabled.\n
 *          \b CFALSE indicate that SIR Mode is Disabled.
 * @see     MES_UART_SetSIRMode,                    
 *          MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *          MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *          MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *          MES_UART_SetBRD,                        MES_UART_GetBRD,
 *          MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *          MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *          MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *          MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *          MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *          MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *          MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *          MES_UART_GetRxOperationMode     
 */
CBOOL   MES_UART_GetSIRMode( void )
{
	const U32 SIR_MODE_MASK = (1<<6);

	MES_ASSERT( CNULL != __g_pRegister );


	if( __g_pRegister->LCON & SIR_MODE_MASK ){ return CTRUE; }
    else                                     { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Enable/Disable Loop-back mode.
 * @param[in]   bEnb    Set as CTRUE to use Loop-back Mode
 * @return      None.
 * @remarks     determine whether or not to use the Loop-back mode.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *                                                      MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	MES_UART_SetLoopBackMode( CBOOL bEnb )
{
	const U32 LOOPBACK_MODE_MASK = (1<<5);

	MES_ASSERT( CNULL != __g_pRegister );

	if ( CTRUE == bEnb ) __g_pRegister->UCON = (U16)( __g_pRegister->UCON | LOOPBACK_MODE_MASK );
	else		         __g_pRegister->UCON &= ~LOOPBACK_MODE_MASK;
}

//-------------------------------------------------------------------------------
/**
 * @brief   Get Loop-Back mode's state
 * @return  \b CTRUE Indicate that Loop-back mode is Enabled.\n
 *          \b CFALSE Indicate that Loop-back mode is Disabled.
 * @see     MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *          MES_UART_SetLoopBackMode,               
 *          MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *          MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *          MES_UART_SetBRD,                        MES_UART_GetBRD,
 *          MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *          MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *          MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *          MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *          MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *          MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *          MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *          MES_UART_GetRxOperationMode     
 */
CBOOL   MES_UART_GetLoopBackMode( void )
{
	const U32 LOOPBACK_MODE_MASK = (1<<5);

	MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->UCON & LOOPBACK_MODE_MASK ){ return CTRUE; }
    else                                          { return CFALSE; }
}

//-------------------------------------------------------------------------------
/**
 * @brief       Enable/Disable of Error interrupt when exception is generated
 * @param[in]   bEnb    \b CTRUE indicate that Error interrupt Enable.\n
 *                      \b CFALSE indicate that Error interrupt Disable.
 * @return      None.
 * @remarks     Enable the UART to generate an interrupt if an exception, such as a
 *              break, frame error, parity error, or everrun error occurs during a receive operation.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *                                                      MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void    MES_UART_SetIntEnbWhenExceptionOccur( CBOOL bEnb )
{
	const U32   ERRSTATE_MASK = ( 1 << 6 );

	MES_ASSERT( CNULL != __g_pRegister );

	if( CTRUE == bEnb ) __g_pRegister->UCON = (U16)( __g_pRegister->UCON | ERRSTATE_MASK);
    else                __g_pRegister->UCON &= ~ERRSTATE_MASK;
}

//-------------------------------------------------------------------------------
/**
 * @brief   Get exception generation's state
 * @return  \b CTRUE indicate that exception generation is enabled.\n
 *          \b CFALSE indicate that exception generation is disabled.
 * @see     MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *          MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *          MES_UART_SetIntEnbWhenExceptionOccur,   
 *          MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *          MES_UART_SetBRD,                        MES_UART_GetBRD,
 *          MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *          MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *          MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *          MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *          MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *          MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *          MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *          MES_UART_GetRxOperationMode     
 */
CBOOL   MES_UART_GetIntEnbWhenExceptionOccur( void )
{
	const U32   ERRSTATE_MASK = ( 1 << 6 );

	MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->UCON & ERRSTATE_MASK ){ return CTRUE; }
    else                                     { return CFALSE;}
}

//------------------------------------------------------------------------------
/**
 * @brief       Set frame configuration during UART trasmit and receive operation.
 * @param[in]   Parity	    Parity generation/check type
 * @param[in]   DataWidth	The number of data bit( 5, 6, 7, 8 bit )
 * @param[in]   StopBit	    The number of stop bit( 1, 2 )
 * @return      None.
 * @remarks     determine the number of data/stop bit and parity mode.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *                                                      MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	MES_UART_SetFrameConfiguration( MES_UART_PARITYMODE Parity,  U32 DataWidth,  U32 StopBit )
{
	register U16 temp;

	const U32 SIRMODE_MASK	= 0x40;
	const U32 PARITY_BITPOS	= 3;
	const U32 STOP_BITPOS = 2;
	const U32 DATA_BITPOS = 0;


	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 0==Parity ||  ( 4 <= Parity && Parity <= 7 ) );
	MES_ASSERT( 5 <= DataWidth && DataWidth <=8 );
	MES_ASSERT( (1==StopBit) || (2==StopBit) );


	temp = __g_pRegister->LCON;

	temp = (U16)( (temp & SIRMODE_MASK)
	     | (U16)( Parity         <<PARITY_BITPOS)
	     | (U16)( (StopBit-1)    <<STOP_BITPOS)
	     | (U16)( (DataWidth-5)  <<DATA_BITPOS) );

	__g_pRegister->LCON = temp;
}

//-------------------------------------------------------------------------------
/**
 * @brief       Get Frame Configuration
 * @param[out]  pParity	Parity generation/check type
 * @param[out]  pDataWidth	The number of data bit ( 5, 6, 7, 8 )
 * @param[out]  pStopBit	The number of stop bit ( 1, 2 )
 * @return      None.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void    MES_UART_GetFrameConfiguration( MES_UART_PARITYMODE* pParity, U32* pDataWidth, U32* pStopBit )
{
	const U32   PARI_MASK   = ( 0x07 << 3 );
	const U32   PARI_BITPOS = 3;
	const U32   DATA_MASK   = ( 0x03 << 0 );
	const U32   DATA_BITPOS = 0;
	const U32   STOP_MASK   = ( 0x01 << 2 );
	const U32   STOP_BITPOS = 2;

	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( CNULL != pParity );
    MES_ASSERT( CNULL != pDataWidth );    
    MES_ASSERT( CNULL != pStopBit );

	temp    =   __g_pRegister->LCON;

    *pParity    =   (MES_UART_PARITYMODE)(( temp & PARI_MASK ) >> PARI_BITPOS);
    *pDataWidth =   (( temp & DATA_MASK ) >> DATA_BITPOS)+5;
    *pStopBit   =   (( temp & STOP_MASK ) >> STOP_BITPOS)+1;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set a BRD(Baud Rate Division) value.
 * @param[in]   BRD    Baud rate division value, It must be greater than 0.
 * @return      None.
 * @remarks     The following fomula can determine the BRD\n
 *              BRD = (UART_CLK / (bps x 16)) - 1
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *                                                      MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	MES_UART_SetBRD( U16 BRD )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CNULL != BRD );

    __g_pRegister->BRD = BRD;
}

//-------------------------------------------------------------------------------
/**
 * @brief       Get Baud Rate Divison Value
 * @return      Baud Rate Divison Value ( 0x0 ~ 0xFFFF )
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
U16     MES_UART_GetBRD( void )
{
	MES_ASSERT( CNULL != __g_pRegister );

    return (__g_pRegister->BRD);
}

//------------------------------------------------------------------------------
/**
 *  @brief      Make a BRD with a baud rate and clock in Hz.
 *  @param[in]  BaudRate	a desired baud rate.
 *  @param[in]  CLKinHz		a uart clock in Hz.
 *  @return     a calculated BRD value.
 *  @remarks    The following fomula can determine the BRD\n
 *              BRD = (UART_CLK / (bps x 16)) - 1
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *                                                      MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
U16		MES_UART_MakeBRD( U32 BaudRate, U32 CLKinHz )
{
	register int temp;

	MES_ASSERT( 0 < BaudRate );
	MES_ASSERT( 0 < CLKinHz );


	temp = (int)(CLKinHz / (16 * BaudRate)) - 1;
	MES_ASSERT( (0x0000FFFF >= temp) && (0 < temp) );
	return (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Enable/Disable RX time out interrupt when UART FIFO is enabled.\n
 *              The interrupt is a receive interrupt.
 * @param[in]   bEnb    \b CTRUE indicate that RX Time Out interrupt Enable. \n
 *                      \b CFALSE indicate that RX Time Out interrupt Disable.
 * @return      None.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void    MES_UART_SetRxTimeOutEnb( CBOOL bEnb )
{
    const U32   RXTIME_MASK  =   ( 0x01 << 7 );

	MES_ASSERT( CNULL != __g_pRegister );

    if( CTRUE == bEnb ){ __g_pRegister->UCON   = (U16)( __g_pRegister->UCON |  RXTIME_MASK ); }
    else               { __g_pRegister->UCON   = (U16)( __g_pRegister->UCON & ~RXTIME_MASK ); }
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Status of RX Time Out interrupt is Enabled or Disabled.
 * @return  \b CTRUE Indicate that RX Time Out Interrupt is Enabled.\n
 *          \b CFALSE Indicate that RX Time Out Interrupt is Disabled.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *                                                      MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
CBOOL   MES_UART_GetRxTimeOutEnb( void )
{
    const U32   RXTIME_MASK  =   ( 0x01 << 7 );

	MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->UCON & RXTIME_MASK ){ return CTRUE;  }
    else                                   { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Set a Rx Time-out period.
 * @param[in]   TimeOut    a Rx time-out period ( 0x0 ~ 0xFFFF )
 * @return      None.
 * @remarks     In case of FIFO mode, when the number of data in FIFO does not
 *              reaches RX FIFO trigger level and does not receive data during
 *              a Rx time-out period, the Rx interrupt will be generated.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	MES_UART_SetRxTimeOut( U16 TimeOut )
{
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->TIMEOUT = TimeOut;
}

//-------------------------------------------------------------------------------
/**
 * @brief       Get a Rx Time-out period.
 * @return      Rx time-out period ( 0x0 ~ 0xFFFF )
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *                                                      MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
U16     MES_UART_GetRxTimeOut( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (__g_pRegister->TIMEOUT);
}

//------------------------------------------------------------------------------
/**
 * @brief   Set irq Pending Signal(Rx, Tx) Clear
 * @return  None.
 * @remarks This bit is cleared automatically.\n
 *          This Function execute one time when initialize.
 * @see     MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *          MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *          MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *          MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *          MES_UART_SetBRD,                        MES_UART_GetBRD,
 *          MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *          MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *          MES_UART_GetRxTimeOut,                  
 *          MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *          MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *          MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *          MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *          MES_UART_GetRxOperationMode     
 */
void    MES_UART_SetSYNCPendClear( void )
{
    const U32   SYNCPENDCLEAR_MASK  =   ( 0x01 << 7 );

	MES_ASSERT( CNULL != __g_pRegister );

    __g_pRegister->LCON   = (U16)( __g_pRegister->LCON | SYNCPENDCLEAR_MASK );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set interrupt request type of Tx operation
 * @param[in]   IrqType    interrupt request type ( pulse or level )
 * @return      None.
 * @remarks     Pulse : Interrupt is requested just ONE PLUSE as soon as Tx buffer becomes empty
 *                      or Tx FIFO reaches Trigger level in FIFO mode.\n
 *              Level : Interrupt is requested just LEVEL while Tx buffer is empty
 *                      or Tx FIFO reaches in trigger level in FIFO mode.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *                                                      MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	            MES_UART_SetTxIRQType( MES_UART_IRQTYPE IrqType )
{
	const U32 TX_IRQTYPE_BITPOS = 9;
	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 2 > IrqType );

	temp = __g_pRegister->UCON;
	temp &= ~(1<<TX_IRQTYPE_BITPOS);
	temp = ( temp | ((U32)IrqType<<TX_IRQTYPE_BITPOS) );
	__g_pRegister->UCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get interrupt request type of Tx operation
 * @return  Value of interrupt request type( pulse or level )
 * @see     MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *          MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *          MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *          MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *          MES_UART_SetBRD,                        MES_UART_GetBRD,
 *          MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *          MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *          MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *          MES_UART_SetTxIRQType,                  
 *          MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *          MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *          MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *          MES_UART_GetRxOperationMode     
 */
MES_UART_IRQTYPE	MES_UART_GetTxIRQType( void )
{
   	const U32 TX_IRQTYPE_MASK = ( 0x01 << 9 );
  	const U32 TX_IRQTYPE_BITPOS = 9;

	MES_ASSERT( CNULL != __g_pRegister );

    return(MES_UART_IRQTYPE)((__g_pRegister->UCON & TX_IRQTYPE_MASK) >> TX_IRQTYPE_BITPOS );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set interrupt request type of Rx operation
 * @param[in]   IrqType    interrupt request type ( pulse of level )
 * @return      None.
 * @remarks     Pulse : Interrupt is requested just ONE PULSE when Rx buffer receives the
 *                     data Non-FIFO mode or reaches Rx FIFO trigger level in
 *                     FIFO mode.\n
 *              Level : Interrupt is requested just LEVEL while Rx buffer is receiving data in
 *                     Non-FIFO or reaches Rx FIFO trigger level in FIFO mode.
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *                                                      MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	            MES_UART_SetRxIRQType( MES_UART_IRQTYPE IrqType )
{
	const U32 RX_IRQTYPE_BITPOS = 8;

	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 2 > IrqType );


	temp = __g_pRegister->UCON;
	temp &= ~(1<<RX_IRQTYPE_BITPOS);
	temp |= ((U32)IrqType<<RX_IRQTYPE_BITPOS);
	__g_pRegister->UCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get interrupt request type of Rx operation
 * @return  Value of interrupt request type( pulse or level )
 * @see     MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *          MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *          MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *          MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *          MES_UART_SetBRD,                        MES_UART_GetBRD,
 *          MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *          MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *          MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *          MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *          MES_UART_SetRxIRQType,                  
 *          MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *          MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *          MES_UART_GetRxOperationMode     
 */
MES_UART_IRQTYPE	MES_UART_GetRxIRQType( void )
{
  	const U32 RX_IRQTYPE_MASK = ( 0x01 << 8 );
  	const U32 RX_IRQTYPE_BITPOS = 8;

	MES_ASSERT( CNULL != __g_pRegister );

  	return (MES_UART_IRQTYPE)((__g_pRegister->UCON & RX_IRQTYPE_MASK) >> RX_IRQTYPE_BITPOS );
}

//------------------------------------------------------------------------------
/**
 * @brief       Send a break signal during 1 fream time.
 * @return      None.
 * @remarks     Auto cleared after sending the break signal
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *                                                      MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
void	MES_UART_SendBreak( void )
{
	const U32 SENDBREAK_MASK = (1<<4);

	MES_ASSERT( CNULL != __g_pRegister );

    __g_pRegister->UCON = (U16)( __g_pRegister->UCON | SENDBREAK_MASK );

}

//------------------------------------------------------------------------------
/**
 * @brief      Set Tx operation mode
 * @param[in]  mode     Operation mode ( Disable or Normal or DMA )
 * @return     None.
 * @see        MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *             MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *             MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *             MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *             MES_UART_SetBRD,                        MES_UART_GetBRD,
 *             MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *             MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *             MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *             MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *             MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *             MES_UART_SendBreak,                     
 *             MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode,
 *             MES_UART_GetRxOperationMode     
 */
void        	    MES_UART_SetTxOperationMode( MES_UART_OPERMODE mode )
{
	const U32 TXOPERMODE_MASK	= (3<<2);
	const U32 TXOPERMODE_BITPOS	= 2;

	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 3 > mode );

	temp = __g_pRegister->UCON;
	temp &= ~TXOPERMODE_MASK;
	temp |= ((U32)mode<<TXOPERMODE_BITPOS);
	__g_pRegister->UCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Tx operation mode
 * @return      Value of operation mode ( Disable or Normal or DMA )
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *                                                      MES_UART_SetRxOperationMode,
 *              MES_UART_GetRxOperationMode     
 */
MES_UART_OPERMODE	MES_UART_GetTxOperationMode( void )
{
	const U32 TXOPERMODE_MASK	= (3<<2);
	const U32 TXOPERMODE_BITPOS	= 2;

	MES_ASSERT( CNULL != __g_pRegister );

    return (MES_UART_OPERMODE)(( __g_pRegister->UCON & TXOPERMODE_MASK ) >> TXOPERMODE_BITPOS );
}

//------------------------------------------------------------------------------
/**
 * @brief      Set Rx operation mode
 * @param[in]  mode     Operation mode ( Disable or Normal or DMA )
 * @return     None.
 * @see        MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *             MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *             MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *             MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *             MES_UART_SetBRD,                        MES_UART_GetBRD,
 *             MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *             MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *             MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *             MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *             MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *             MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *             MES_UART_GetTxOperationMode,            
 *             MES_UART_GetRxOperationMode     
 */
void 	            MES_UART_SetRxOperationMode( MES_UART_OPERMODE mode )
{
	const U32 RXOPERMODE_MASK	= (3<<0);
	const U32 RXOPERMODE_BITPOS	= 0;

	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 3 > mode );

	temp = __g_pRegister->UCON;
	temp &= ~RXOPERMODE_MASK;
	temp |= ((U32)mode<<RXOPERMODE_BITPOS);
	__g_pRegister->UCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Rx operation mode
 * @return      Value of operation mode ( Disable or Normal or DMA )
 * @see         MES_UART_SetSIRMode,                    MES_UART_GetSIRMode,
 *              MES_UART_SetLoopBackMode,               MES_UART_GetLoopBackMode,
 *              MES_UART_SetIntEnbWhenExceptionOccur,   MES_UART_GetIntEnbWhenExceptionOccur,
 *              MES_UART_SetFrameConfiguration,         MES_UART_GetFrameConfiguration,
 *              MES_UART_SetBRD,                        MES_UART_GetBRD,
 *              MES_UART_MakeBRD,                       MES_UART_SetRxTimeOutEnb,
 *              MES_UART_GetRxTimeOutEnb,               MES_UART_SetRxTimeOut,
 *              MES_UART_GetRxTimeOut,                  MES_UART_SetSYNCPendClear,
 *              MES_UART_SetTxIRQType,                  MES_UART_GetTxIRQType,
 *              MES_UART_SetRxIRQType,                  MES_UART_GetRxIRQType,
 *              MES_UART_SendBreak,                     MES_UART_SetTxOperationMode,
 *              MES_UART_GetTxOperationMode,            MES_UART_SetRxOperationMode
 */
MES_UART_OPERMODE	MES_UART_GetRxOperationMode( void )
{
	const U32 RXOPERMODE_MASK	= (3<<0);
	const U32 RXOPERMODE_BITPOS	= 0;

	MES_ASSERT( CNULL != __g_pRegister );

    return (MES_UART_OPERMODE)(( __g_pRegister->UCON & RXOPERMODE_MASK ) >> RXOPERMODE_BITPOS );
}



//--------------------------------------------------------------------------
//	FIFO control operations
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief       Set the trigger level of Tx FIFO
 * @param[in]   level    FIFO trigger level ( 0 byte(None), 4byte, 8byte, 12byte )
 * @return      None.
 * @see                                         MES_UART_GetTxFIFOTriggerLevel,
 *              MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
void	MES_UART_SetTxFIFOTriggerLevel( U32 level )
{
	const U32 TXFIFOTL_MASK		= (3<<6);
	const U32 TXFIFOTL_BITPOS	= 6;

	register U32 temp;
	register U32 SetValue = 0;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( (0==level) || (4==level) || (8==level) || (12==level) );

    switch( level )
    {
        case 0:     SetValue = 0;   break;
        case 4:     SetValue = 1;   break;
        case 8:     SetValue = 2;   break;
        case 12:    SetValue = 3;   break;
    }

	temp = __g_pRegister->FCON;
	temp &= ~TXFIFOTL_MASK;
	temp |= ((U32)SetValue<<TXFIFOTL_BITPOS);
	__g_pRegister->FCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get the trigger level of Tx FIFO
 * @return      Value of FIFO trigger level ( 0 byte(None), 4byte, 8byte, 12byte )
 * @see         MES_UART_SetTxFIFOTriggerLevel, 
 *              MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
U32     MES_UART_GetTxFIFOTriggerLevel( void )
{
	const U32 TXFIFOTL_MASK		= (3<<6);
	const U32 TXFIFOTL_BITPOS	= 6;

    register U32    RetValue=0;
    register U32    ReadValue;

    MES_ASSERT( CNULL != __g_pRegister );

    ReadValue = (__g_pRegister->FCON & TXFIFOTL_MASK ) >> TXFIFOTL_BITPOS;

    switch( ReadValue )
    {
        case 0:     RetValue = 0;   break;
        case 1:     RetValue = 4;   break;
        case 2:     RetValue = 8;   break;
        case 3:     RetValue = 12;   break;
    }

    return RetValue;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set the trigger level of Rx FIFO
 * @param[in]   level    FIFO trigger level ( 1byte, 4byte, 8byte, 12byte )
 * @return      None.
 * @see         MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *                                              MES_UART_GetRxFIFOTriggerLevel,
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
void	MES_UART_SetRxFIFOTriggerLevel( U32 level )
{
	const U32 RXFIFOTL_MASK		= (3<<4);
	const U32 RXFIFOTL_BITPOS	= 4;

	register U32 temp;
	register U32 SetValue=0;

	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( (1==level) || (4==level) || (8==level) || (12==level) );

    switch( level )
    {
        case 1:     SetValue = 0;   break;
        case 4:     SetValue = 1;   break;
        case 8:     SetValue = 2;   break;
        case 12:    SetValue = 3;   break;
    }

	temp = __g_pRegister->FCON;
	temp &= ~RXFIFOTL_MASK;
	temp |= ((U32)SetValue<<RXFIFOTL_BITPOS);
	__g_pRegister->FCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get the trigger level of Rx FIFO
 * @return      Value of FIFO trigger level ( 1byte, 4byte, 8byte, 12byte )
 * @see         MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *              MES_UART_SetRxFIFOTriggerLevel, 
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
U32     MES_UART_GetRxFIFOTriggerLevel( void )
{
   	const U32 RXFIFOTL_MASK		= (3<<4);
	const U32 RXFIFOTL_BITPOS	= 4;

    register U32    RetValue=0;
    register U32    ReadValue;

    MES_ASSERT( CNULL != __g_pRegister );

	ReadValue = (( __g_pRegister->FCON & RXFIFOTL_MASK ) >> RXFIFOTL_BITPOS );

    switch( ReadValue )
    {
        case 0:     RetValue = 1;   break;
        case 1:     RetValue = 4;   break;
        case 2:     RetValue = 8;   break;
        case 3:     RetValue = 12;   break;
    }

    return RetValue;

}

//------------------------------------------------------------------------------
/**
 * @brief       Reset Tx FIFO
 * @return      None.
 * @remarks     This bit is auto cleared when FIFO is reseted.
 * @see         MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *              MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *                                              MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
void	MES_UART_ResetTxFIFO( void )
{
	const U32 TX_FIFO_RESET	= (1<<2);

	MES_ASSERT( CNULL != __g_pRegister );

	__g_pRegister->FCON = (U16)( __g_pRegister->FCON | TX_FIFO_RESET );
}

//------------------------------------------------------------------------------
/**
 * @brief   Check Tx FIFO's state
 * @return  \b CTRUE Indicate that Reset is processing.\n
 *          \b CFALSE Indicate that FIFO is reseted.
 * @see     MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *          MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *          MES_UART_ResetTxFIFO,           
 *          MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *          MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */

CBOOL   MES_UART_IsTxFIFOReset( void )
{
  	const U32 TX_FIFO_RESET	= (1<<2);

	MES_ASSERT( CNULL != __g_pRegister );

  	if( __g_pRegister->FCON & TX_FIFO_RESET ){ return CTRUE; }
  	else                                     { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Reset Rx FIFO
 * @return      None.
 * @remarks     This bit is auto cleared when FIFO is reseted.
 * @see         MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *              MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *                                              MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
void	MES_UART_ResetRxFIFO( void )
{
	const U32 RX_FIFO_RESET	= (1<<1);

	MES_ASSERT( CNULL != __g_pRegister );

	__g_pRegister->FCON = (U16)( __g_pRegister->FCON | RX_FIFO_RESET );
}

//------------------------------------------------------------------------------
/**
 * @brief   Check Rx FIFO's state
 * @return  \b CTRUE Indicate that Reset is processing.\n
 *          \b CFALSE Indicate that FIFO is reseted.
 * @see     MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *          MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *          MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *          MES_UART_ResetRxFIFO,           
 *          MES_UART_SetFIFOEnb,            MES_UART_GetFIFOEnb
 */
CBOOL   MES_UART_IsRxFIFOReset( void )
{
	const U32 RX_FIFO_RESET	= (1<<1);

	MES_ASSERT( CNULL != __g_pRegister );

  	if( __g_pRegister->FCON & RX_FIFO_RESET ){ return CTRUE; }
  	else                                     { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Enable/disable FIFO mode
 * @param[in]   bEnb    Set as CTRUE to use FIFO
 * @return      None.
 * @see         MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *              MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *                                              MES_UART_GetFIFOEnb
 */
void	MES_UART_SetFIFOEnb( CBOOL bEnb )
{
	const U32 FIFOEN_MASK	= (1<<0);

	MES_ASSERT( CNULL != __g_pRegister );

	if ( CTRUE == bEnb ) __g_pRegister->FCON = (U16)( __g_pRegister->FCON | FIFOEN_MASK );
	else		         __g_pRegister->FCON &= ~FIFOEN_MASK;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get FIFO's mode
 * @return      \b CTRUE Indicate that FIFO is Enabled.\n
 *              \b CFALSE Indicate that FIFO is Disabled.
 * @see         MES_UART_SetTxFIFOTriggerLevel, MES_UART_GetTxFIFOTriggerLevel,
 *              MES_UART_SetRxFIFOTriggerLevel, MES_UART_GetRxFIFOTriggerLevel,
 *              MES_UART_ResetTxFIFO,           MES_UART_IsTxFIFOReset,
 *              MES_UART_ResetRxFIFO,           MES_UART_IsRxFIFOReset,
 *              MES_UART_SetFIFOEnb            
 */
CBOOL   MES_UART_GetFIFOEnb( void )
{
	const U32 FIFOEN_MASK	= (1<<0);

	MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->FCON & FIFOEN_MASK ){ return CTRUE; }
    else                                   { return CFALSE; }
}


//--------------------------------------------------------------------------
//	Smartmedia card control operations
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief       Enable/disable to transmit data to smartmedia card.
 * @param[in]   bEnb    Set as CTRUE to transmit data to smartmedia card.
 * @return      None.
 * @remarks     If you want to use normal UART, you have to set it as CFALSE.
 * @see                                 MES_UART_GetSCTxEnb,
 *              MES_UART_SetSCRxEnb,    MES_UART_GetSCRxEnb,
 *              MES_UART_SetSCTxMode,   MES_UART_SetSCRxMode,
 *              MES_UART_GetSCMode
 */
void	MES_UART_SetSCTxEnb( CBOOL bEnb )
{
	const U32 SMCTXENB_MASK	= (1<<5);

	MES_ASSERT( CNULL != __g_pRegister );

	if( CTRUE == bEnb ) 	__g_pRegister->MCON = (U16)( __g_pRegister->MCON | SMCTXENB_MASK );
	else		            __g_pRegister->MCON &= ~SMCTXENB_MASK;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get TX operation of Smart Card
 * @return      \b CTRUE indicate that Smart card TX is Enabled.\n
 *              \b CFALSE indicate that Smart card TX is Disabled.
 * @see         MES_UART_SetSCTxEnb,    
 *              MES_UART_SetSCRxEnb,    MES_UART_GetSCRxEnb,
 *              MES_UART_SetSCTxMode,   MES_UART_SetSCRxMode,
 *              MES_UART_GetSCMode
 */
CBOOL   MES_UART_GetSCTxEnb( void )
{
	const U32 SMCTXENB_MASK	= (1<<5);

	MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->MCON & SMCTXENB_MASK ){ return CTRUE; }
    else                                     { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Enable/disable to receive data from smartmedia card.
 * @param[in]   bEnb    Set as CTURE to receive data from smartmedia card.
 * @return      None.
 * @remarks     If you want to use normal UART, you have to set it as CTRUE.
 * @see         MES_UART_SetSCTxEnb,    MES_UART_GetSCTxEnb,
 *                                      MES_UART_GetSCRxEnb,
 *              MES_UART_SetSCTxMode,   MES_UART_SetSCRxMode,
 *              MES_UART_GetSCMode
 */
void	MES_UART_SetSCRxEnb( CBOOL bEnb )
{
	const U32 SMCRXENB_MASK	= (1<<6);

	MES_ASSERT( CNULL != __g_pRegister );

	if( CTRUE == bEnb ) 	__g_pRegister->MCON = (U16)( __g_pRegister->MCON | SMCRXENB_MASK );
	else		            __g_pRegister->MCON &= ~SMCRXENB_MASK;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get RX operation of Smart Card
 * @return      \b CTRUE Indicate that Smart card RX is Enabled.\n
 *              \b CFALSE Indicate that Smart card RX is Disabled.
 * @see         MES_UART_SetSCTxEnb,    MES_UART_GetSCTxEnb,
 *              MES_UART_SetSCRxEnb,    
 *              MES_UART_SetSCTxMode,   MES_UART_SetSCRxMode,
 *              MES_UART_GetSCMode
 */
CBOOL	MES_UART_GetSCRxEnb( void )
{
	const U32 SMCRXENB_MASK	= (1<<6);

   	MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->MCON & SMCRXENB_MASK ){ return CTRUE; }
    else                                     { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Set UART in order to transmit data to smartmedia card.
 * @return      None.
 * @remarks     Same as SetSCTxEnb(CTRUE) and SetSCRxEnb(CFALSE).\n
 * @see         MES_UART_SetSCTxEnb,    MES_UART_GetSCTxEnb,
 *              MES_UART_SetSCRxEnb,    MES_UART_GetSCRxEnb,
 *                                      MES_UART_SetSCRxMode,
 *              MES_UART_GetSCMode
 */
void	MES_UART_SetSCTxMode( void )
{
	const U32 SCTXENB_MASK	= (1<<5);
	const U32 SCRXENB_MASK	= (1<<6);

	register U16 temp;

	MES_ASSERT( CNULL != __g_pRegister );

	temp = __g_pRegister->MCON;
	temp &= ~SCRXENB_MASK;
	temp =  (U16)( temp | SCTXENB_MASK );
	__g_pRegister->MCON = temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Set UART in order to receive data from smartmedia card.
 * @return      None.
 * @remarks     Same as SetSCTxEnb(CFALSE) and SetSCRxEnb(CTURE)
 * @see         MES_UART_SetSCTxEnb,    MES_UART_GetSCTxEnb,
 *              MES_UART_SetSCRxEnb,    MES_UART_GetSCRxEnb,
 *              MES_UART_SetSCTxMode,   
 *              MES_UART_GetSCMode
 */
void	MES_UART_SetSCRxMode( void )
{
	const U32 SMCTXENB_MASK	= (1<<5);
	const U32 SMCRXENB_MASK	= (1<<6);

	register U16 temp;

	MES_ASSERT( CNULL != __g_pRegister );

	temp = __g_pRegister->MCON;
	temp &= ~SMCTXENB_MASK;
	temp =  (U16)( temp | SMCRXENB_MASK );
	__g_pRegister->MCON = temp;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Smart Card's Setting Mode
 * @return      Smart Card's mode( SCMODE_TX(2) or SCMODE_RX(1) or SCMODE_DISABLE(0) )
 * @see         MES_UART_SetSCTxEnb,    MES_UART_GetSCTxEnb,
 *              MES_UART_SetSCRxEnb,    MES_UART_GetSCRxEnb,
 *              MES_UART_SetSCTxMode,   MES_UART_SetSCRxMode,
 */
MES_UART_SCMODE  MES_UART_GetSCMode( void )
{
    const U32   SC_MASK     = ( 0x03 << 5 );
    const U32   SC_BITPOS   = 5;

	MES_ASSERT( CNULL != __g_pRegister );

    return(MES_UART_SCMODE)( (__g_pRegister->MCON & SC_MASK) >> SC_BITPOS );
}


//--------------------------------------------------------------------------
//	Status operations
//--------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 * @brief   Get Tx/Rx status
 * @return  return status of Transmitter, Tx Buffer and Rx Buffer
 * @remarks Check state Using TXRXSTATUS.
 * @code
 *     U32 state;
 *     state = MES_UART_GetTxRxStatus();
 *     if( MES_UART_RX_BUFFER_READY & state ){ ... }
 *     if( MES_UART_TX_BUFFER_EMPTY & state ){ ... }
 *     ...
 * @endcode
 * @see                            MES_UART_GetErrorStatus,
 *         MES_UART_GetFIFOStatus, MES_UART_GetTxFIFOCount,
 *         MES_UART_GetRxFIFOCount
 */
U32		MES_UART_GetTxRxStatus( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(__g_pRegister->TRSTATUS & 0x7);
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Error status
 * @return  return status of Error which occurs during UART operation.
 * @remarks Check state Using ERRSTAT.
 * @code
 *     U32 state;
 *     state = MES_UART_GetErrorStatus();
 *     if( MES_UART_ERRSTAT_OVERRUN & state ){ ... }
 *     if( MES_UART_ERRSTAT_PARITY & state ) { ... }
 *     ...
 * @endcode
 * @see    MES_UART_GetTxRxStatus, 
 *         MES_UART_GetFIFOStatus, MES_UART_GetTxFIFOCount,
 *         MES_UART_GetRxFIFOCount
 */
U32		MES_UART_GetErrorStatus( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	return (U32)(__g_pRegister->ESTATUS & 0xF);
}

//------------------------------------------------------------------------------
/**
 * @brief       Get FIFO's status
 * @return      FIFO's status
 * @code
 *     U32 state;
 *     state = MES_UART_GetFIFOStatus();
 *     if( MES_UART_FIFOSTAT_RXFULL & state ){ ... }
 *     if( MES_UART_FIFOSTAT_TXFULL & state ){ ... }
 *     ...
 * @endcode
 * @see    MES_UART_GetTxRxStatus, MES_UART_GetErrorStatus,
 *                                 MES_UART_GetTxFIFOCount,
 *         MES_UART_GetRxFIFOCount
 */
U32     MES_UART_GetFIFOStatus( void )
{
    const U32 FIFO_MASK     = ( 0x07 << 8 );
    const U32 FIFO_BITPOS   = 8;

    MES_ASSERT( CNULL != __g_pRegister );

    return ( ( __g_pRegister->FSTATUS & FIFO_MASK ) >> FIFO_BITPOS );
}

//------------------------------------------------------------------------------
/**
 * @brief  Get the number of data in Tx FIFO.
 * @return return the number of data in Tx FIFO.
 * @see    MES_UART_GetTxRxStatus, MES_UART_GetErrorStatus,
 *         MES_UART_GetFIFOStatus, 
 *         MES_UART_GetRxFIFOCount
 */
U32		MES_UART_GetTxFIFOCount( void )
{
	const U32 TXFIFO_FULL_POS	= 9;
	const U32 TXFIFO_FULL_MASK	= (0x1<<TXFIFO_FULL_POS);
	const U32 TXFIFOCOUNT_POS	= 4;
	const U32 TXFIFOCOUNT_MASK 	= (0xF<<TXFIFOCOUNT_POS);

	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );

	temp = __g_pRegister->FSTATUS;
	temp = ((temp & TXFIFO_FULL_MASK)>>(TXFIFO_FULL_POS-4)) | ((temp & TXFIFOCOUNT_MASK) >> TXFIFOCOUNT_POS);

	return temp;
}

//------------------------------------------------------------------------------
/**
 * @brief  Get the number of data in Rx FIFO.
 * @return return the number of data in Rx FIFO.
 * @see    MES_UART_GetTxRxStatus, MES_UART_GetErrorStatus,
 *         MES_UART_GetFIFOStatus, MES_UART_GetTxFIFOCount
 */
U32		MES_UART_GetRxFIFOCount( void )
{
	const U32 RXFIFO_FULL_POS	= 8;
	const U32 RXFIFO_FULL_MASK	= (0x1<<RXFIFO_FULL_POS);
	const U32 RXFIFOCOUNT_POS	= 0;
	const U32 RXFIFOCOUNT_MASK 	= (0xF<<RXFIFOCOUNT_POS);

	register U32 temp;

	MES_ASSERT( CNULL != __g_pRegister );

	temp = __g_pRegister->FSTATUS;
	temp = ((temp & RXFIFO_FULL_MASK)>>(RXFIFO_FULL_POS-4)) | ((temp & RXFIFOCOUNT_MASK) >> RXFIFOCOUNT_POS);
	return temp;
}


//--------------------------------------------------------------------------
// Modem operations
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief       Activate/Deactivate nDTR signal.
 * @param[in]   bActive    Set as CTURE to activate nDTR signal.
 * @return      None.
 * @remarks     Output low level in active state.
 * @see                                         MES_UART_GetDTR,
 *              MES_UART_SetRTS,                MES_UART_GetRTS,
 *              MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *              MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 *              MES_UART_GetModemStatus
 */
void	MES_UART_SetDTR( CBOOL bActive )
{
	const U32 DTR_MASK	= (1<<1);

	MES_ASSERT( CNULL != __g_pRegister );

	if( CTRUE == bActive ) 	__g_pRegister->MCON = (U16)( __g_pRegister->MCON | DTR_MASK );
	else		        	__g_pRegister->MCON &= ~DTR_MASK;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get DTR signal's state
 * @return      \b CTRUE indicate that DTR is Actived.\n
 *              \b CFALSE indicate that DTR is Inactived.
 * @remarks     Output low level in active state.
 * @see         MES_UART_SetDTR,                
 *              MES_UART_SetRTS,                MES_UART_GetRTS,
 *              MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *              MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 *              MES_UART_GetModemStatus
 */
CBOOL   MES_UART_GetDTR( void )
{
    const U32 DTR_MASK	= (1<<1);

	MES_ASSERT( CNULL != __g_pRegister );

	if( __g_pRegister->MCON & DTR_MASK ){ return CTRUE; }
	else                                { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Activate/Deactivate nRTS signal.
 * @param[in]   bActive    Set as CTURE to activate nRTS signal.
 * @return      None.
 * @remarks     output low level in active state.\n
 *              In AFC mode, nRTS signal is controled automatically by UART module .
 * @see         MES_UART_SetDTR,                MES_UART_GetDTR,
 *                                              MES_UART_GetRTS,
 *              MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *              MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 *              MES_UART_GetModemStatus
 */
void	MES_UART_SetRTS( CBOOL bActive )
{
	const U32 RTS_MASK	= (1<<0);

	MES_ASSERT( CNULL != __g_pRegister );

	if( CTRUE == bActive ) 	__g_pRegister->MCON =  (U16)( __g_pRegister->MCON | RTS_MASK );
	else		        	__g_pRegister->MCON &= ~RTS_MASK;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get RTS signal's state
 * @return      \b CTRUE Indicate that RTS is Actived.\n
 *              \b CFALSE Indicate that RTS is Inactived.
 * @remarks     Output low level in active state.\n
 *              In AFC mode, nRTS signal is controled automatically by UART module .
 * @see         MES_UART_SetDTR,                MES_UART_GetDTR,
 *              MES_UART_SetRTS,                
 *              MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *              MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 *              MES_UART_GetModemStatus
 */
CBOOL	MES_UART_GetRTS( void )
{
	const U32 RTS_MASK	= (1<<0);

	MES_ASSERT( CNULL != __g_pRegister );


    if( __g_pRegister->MCON & RTS_MASK ){ return CTRUE; }
    else                               { return CFALSE; }
}


//------------------------------------------------------------------------------
/**
 * @brief       Set Auto Flow Control  Operation
 * @param[in]   enable    \b CTRUE indicate that Auto Flow Control Enable. \n
 *                        \b CFALSE indicate that Auto Flow Control Disable.
 * @return      None.
 * @see         MES_UART_SetDTR,                MES_UART_GetDTR,
 *              MES_UART_SetRTS,                MES_UART_GetRTS,
 *                                              MES_UART_GetAutoFlowControl,
 *              MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 *              MES_UART_GetModemStatus
 */
void    MES_UART_SetAutoFlowControl( CBOOL enable )
{
    const U32   AFC_MASK = ( 0x01 << 4 );
    register U32   temp;

    MES_ASSERT( CNULL != __g_pRegister );

    temp = __g_pRegister->MCON;

    if( enable == CTRUE ){ temp |=  AFC_MASK; }
    else                 { temp &=  ~AFC_MASK; }

    __g_pRegister->MCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Auto Flow Control  Operation's state
 * @return  \b CTRUE Indicate that Auto Flow Control is Enabled.\n
 *          \b CFALSE Indicate that Auto Flow Control is Disabled.
 * @see     MES_UART_SetDTR,                MES_UART_GetDTR,
 *          MES_UART_SetRTS,                MES_UART_GetRTS,
 *          MES_UART_SetAutoFlowControl,    
 *          MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 *          MES_UART_GetModemStatus
 */
CBOOL   MES_UART_GetAutoFlowControl( void )
{
    const U32   AFC_MASK = ( 0x01 << 4 );

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->MCON & AFC_MASK ){ return CTRUE; }
    else                                { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Half Channel Operation
 * @param[in]   enable    \b CTRUE indicate that Half Channel Enable. \n
 *                        \b CFALSE indicate that Half Channel Disable.
 * @return      None.
 * @see         MES_UART_SetDTR,                MES_UART_GetDTR,
 *              MES_UART_SetRTS,                MES_UART_GetRTS,
 *              MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *                                              MES_UART_GetHalfChannelEnable,
 *              MES_UART_GetModemStatus
 */
void    MES_UART_SetHalfChannelEnable( CBOOL enable )
{
    const U32   HC_MASK = ( 0x01 << 7 );
    register U32   temp;

    MES_ASSERT( CNULL != __g_pRegister );

    temp = __g_pRegister->MCON;

    if( CTRUE == enable ){ temp |=  HC_MASK; }
    else                 { temp &=  ~HC_MASK; }

    __g_pRegister->MCON = (U16)temp;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Half Channel Operation's state
 * @return  \b CTRUE Indicate that Half Channel is Enable.\n
 *          \b CFALSE Indicate that Half Channel is Disable.
 * @see     MES_UART_SetDTR,                MES_UART_GetDTR,
 *          MES_UART_SetRTS,                MES_UART_GetRTS,
 *          MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *          MES_UART_SetHalfChannelEnable,  
 *          MES_UART_GetModemStatus
 */
CBOOL   MES_UART_GetHalfChannelEnable( void )
{
    const U32   HC_MASK = ( 0x01 << 7 );

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->MCON & HC_MASK ){ return CTRUE; }
    else                               { return CFALSE; }
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Modem Status.
 * @return  return status of Modem Pin ( DCD, RI, DSR, CTS )
 * @remarks Modem's status is cleared after status read.
 * @code
 *     U32 state;
 *     state = MES_UART_GetModemStatus();
 *     if( MES_UART_MODEMSTAT_DCD_CHANGED & state ){ ... }
 *     if( MES_UART_MODEMSTAT_RI_CHANGED  & state ){ ... }
 *     ...
 * @endcode
 
 * @see    MES_UART_SetDTR,                MES_UART_GetDTR,
 *         MES_UART_SetRTS,                MES_UART_GetRTS,
 *         MES_UART_SetAutoFlowControl,    MES_UART_GetAutoFlowControl,
 *         MES_UART_SetHalfChannelEnable,  MES_UART_GetHalfChannelEnable,
 */
U32 	MES_UART_GetModemStatus( void )
{
    MES_ASSERT( CNULL != __g_pRegister );                              
                                                                   
    return (__g_pRegister->MSTATUS & 0xFF );
}

//--------------------------------------------------------------------------
//	Basic operations
//--------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 * @brief       Send a data to be transmitted.
 * @param[in]   Data    a data to be transmitted.
 * @return      If the result is successful, then return CTRUE.
 * @see         MES_UART_GetByte
 */
void	MES_UART_SendByte( U8 Data )
{
	MES_ASSERT( CNULL != __g_pRegister );
	__g_pRegister->THB = Data;
}


//------------------------------------------------------------------------------
/**
 * @brief   Get a received data
 * @return  Value of Received Data
 * @see     MES_UART_SendByte
 */
U8      MES_UART_GetByte( void )       
{
    MES_ASSERT( CNULL != __g_pRegister );

    return (U8)__g_pRegister->RHB;
}

//--------------------------------------------------------------------------
// UART Configuration Function
//--------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 * @brief       Set UART Line Configuration
 * @param[in]   sirMode     Set as CTRUE to use SIR Mode
 * @param[in]   Parity      Parity generation/check type
 * @param[in]   DataWidth   The number of data bit ( 5, 6, 7, 8 )
 * @param[in]   StopBit     The number of stop bit ( 1 ,2 )
 * @return      None.
 * @see                                     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_SetLineConfig( CBOOL sirMode, MES_UART_PARITYMODE Parity, U32 DataWidth, U32 StopBit )
{
    MES_UART_SetSIRMode( sirMode );
    MES_UART_SetFrameConfiguration( Parity, DataWidth, StopBit );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set UART Control Configuration
 * @param[in]   syncPendClear    CTRUE(Set irq Pending Signal(Rx, Tx) Clear), CFALSE( Nothing )
 * @param[in]   txType           interrupt request type ( pulse or level )
 * @param[in]   rxType           interrupt request type ( pulse or level )
 * @param[in]   txOper           Operation mode ( Disable or Normal or DMA )
 * @param[in]   rxOper           Operation mode ( Disable or Normal or DMA )
 * @return      None
 * @see         MES_UART_SetLineConfig,     
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_SetControlConfig( CBOOL syncPendClear, MES_UART_IRQTYPE txType, MES_UART_IRQTYPE rxType, MES_UART_OPERMODE txOper, MES_UART_OPERMODE rxOper )
{
    if( CTRUE == syncPendClear ){ MES_UART_SetSYNCPendClear(); }
    MES_UART_SetTxIRQType( txType );
    MES_UART_SetRxIRQType( rxType );
    MES_UART_SetTxOperationMode( txOper );
    MES_UART_SetRxOperationMode( rxOper );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set FIFO Control Configuration
 * @param[in]   fifoEnb          Set as CTRUE to use FIFO
 * @param[in]   txFIFOReset      CTRUE( Reset Tx FIFO ), CFALSE( Nothing )
 * @param[in]   rxFIFOReset      CTRUE( Reset Rx FIFO ), CFALSE( Nothing )
 * @param[in]   txLevel          FIFO trigger level ( 0, 4byte, 8byte, 12byte )
 * @param[in]   rxLevel          FIFO trigger level ( 1byte, 4byte, 8byte, 12byte )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *                                          MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_SetFIFOConfig( CBOOL fifoEnb, CBOOL txFIFOReset, CBOOL rxFIFOReset, U32 txLevel, U32 rxLevel )
{
    MES_UART_SetFIFOEnb( fifoEnb );
    if( CTRUE == txFIFOReset ){ MES_UART_ResetTxFIFO(); }
    if( CTRUE == rxFIFOReset ){ MES_UART_ResetRxFIFO(); }
    MES_UART_SetTxFIFOTriggerLevel( txLevel );
    MES_UART_SetRxFIFOTriggerLevel( rxLevel );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Baud Rate & Rx Time Out Configuration
 * @param[in]   baudRate       Baud rate division value, It must be greater than 0.
 * @param[in]   timeOut        a Rx time-out period ( 0x0 ~ 0xFFFF )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_SetBaudRateConfig( U16 baudRate, U16 timeOut )
{
    MES_UART_SetBRD(baudRate);
    MES_UART_SetRxTimeOut(timeOut);
}

//------------------------------------------------------------------------------
/**
 * @brief       Set UART Modem Configuration
 * @param[in]   autoflow         CTRUE( Auto Flow Control Enable ), CFALSE( Auto Flow Control Disable )
 * @param[in]   halfChannel      CTRUE( Half Channel Enable ), CFALSE( Half Channel Disable )
 * @param[in]   SCTxEnb          CTRUE( Smart Card Tx Enable ), CFALSE( Smart Card Tx Disable )
 * @param[in]   SCRxEnb          CTRUE( Smart Card Rx Enable ), CFALSE( Smart Card Rx Disable )
 * @param[in]   DTR              CTRUE( DTR Active), CFALSE( DTR InActive )
 * @param[in]   RTS              CTRUE( RTS Active), CFALSE( RTS InActive )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *                                          MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_SetModemConfig( CBOOL autoflow, CBOOL halfChannel, CBOOL SCTxEnb, CBOOL SCRxEnb, CBOOL DTR, CBOOL RTS )
{
    MES_ASSERT( CNULL != __g_pRegister );

    MES_UART_SetAutoFlowControl( autoflow );
    MES_UART_SetHalfChannelEnable( halfChannel );
    MES_UART_SetSCTxEnb( SCTxEnb );
    MES_UART_SetSCRxEnb( SCRxEnb );
    MES_UART_SetDTR( DTR );
    MES_UART_SetRTS( RTS );
}

//------------------------------------------------------------------------------
/**
 * @brief       Get UART Line Configuration
 * @param[out]  pSirMode     1 : SIR Mode, 0 : UART Mode
 * @param[out]  pParity      Parity generation/check type
 * @param[out]  pDataWidth   The number of data bit ( 5, 6, 7, 8 )
 * @param[out]  pStopBit     The number of stop bit ( 1, 2 )
 * @return      None.
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_GetLineConfig( U32* pSirMode, MES_UART_PARITYMODE* pParity, U32* pDataWidth, U32* pStopBit )
{
    MES_ASSERT( CNULL != pSirMode );
    MES_ASSERT( CNULL != pParity );   
    MES_ASSERT( CNULL != pDataWidth );
    MES_ASSERT( CNULL != pStopBit );

    if( CTRUE == MES_UART_GetSIRMode(  ) ){ *pSirMode = 1;   }
    else                                  { *pSirMode = 0;   }

    MES_UART_GetFrameConfiguration( pParity, pDataWidth, pStopBit );

	MES_ASSERT( 0==(*pParity) ||  ( 4 <= (*pParity) && (*pParity) <= 7 ) );
	MES_ASSERT( 5 <= (*pDataWidth) && (*pDataWidth) <=8 );
	MES_ASSERT( (1==(*pStopBit)) || (2==(*pStopBit)) );
}

//------------------------------------------------------------------------------
/**
 * @brief       Get UART Control Configuration
 * @param[out]  pTxType        interrupt request type ( pulse or level )
 * @param[out]  pRxType        interrupt request type ( pulse or level )
 * @param[out]  pTxOper        Operation mode ( Disable or Normal or DMA )
 * @param[out]  pRxOper        Operation mode ( Disable or Normal or DMA )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *                                          MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_GetControlConfig( MES_UART_IRQTYPE* pTxType, MES_UART_IRQTYPE* pRxType, MES_UART_OPERMODE* pTxOper, MES_UART_OPERMODE* pRxOper )
{
    MES_ASSERT( CNULL != pTxType );
    MES_ASSERT( CNULL != pRxType );
    MES_ASSERT( CNULL != pTxOper );
    MES_ASSERT( CNULL != pRxOper );    
    
    *pTxType = MES_UART_GetTxIRQType(  );
    *pRxType = MES_UART_GetRxIRQType(  );
	*pTxOper = MES_UART_GetTxOperationMode(  );
	*pRxOper = MES_UART_GetRxOperationMode(  );

	MES_ASSERT( 2 > *pTxType );
    MES_ASSERT( 2 > *pRxType );
    MES_ASSERT( 3 > *pTxOper );
    MES_ASSERT( 3 > *pRxOper );
}

//------------------------------------------------------------------------------
/**
 * @brief       Get FIFO Control Configuration
 * @param[out]  pFIFOEnb       1 : FIFO Enable          0 : FIFO Disable
 * @param[out]  pTxFIFOReset   1 : Processing of Reset  0 : Nothing
 * @param[out]  pRxFIFOReset   1 : Processing of Reset  0 : Nothing
 * @param[out]  pTxLevel       FIFO trigger level ( Empty, 4byte, 8byte, 12byte )
 * @param[out]  pRxLevel       FIFO trigger level ( Empty, 4byte, 8byte, 12byte )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  
 *              MES_UART_GetBaudRateConfig, MES_UART_GetModemConfig
 */
void    MES_UART_GetFIFOConfig( U32* pFIFOEnb, U32* pTxFIFOReset, U32* pRxFIFOReset, U32* pTxLevel, U32* pRxLevel )
{

    MES_ASSERT( CNULL != pFIFOEnb );
    MES_ASSERT( CNULL != pTxFIFOReset );
    MES_ASSERT( CNULL != pRxFIFOReset );
    MES_ASSERT( CNULL != pTxLevel );
    MES_ASSERT( CNULL != pRxLevel );            

    if( CTRUE == MES_UART_GetFIFOEnb(  ) ){ *pFIFOEnb = 1; }
    else                                  { *pFIFOEnb = 0; }

    if( CTRUE == MES_UART_IsTxFIFOReset(  ) ){ *pTxFIFOReset = 1; }
    else                                     { *pTxFIFOReset = 0; }

    if( CTRUE == MES_UART_IsRxFIFOReset(  ) ){ *pRxFIFOReset = 1; }
    else                                     { *pRxFIFOReset = 0; }

	*pTxLevel = MES_UART_GetTxFIFOTriggerLevel(  );
	*pRxLevel = MES_UART_GetRxFIFOTriggerLevel(  );

	MES_ASSERT( (0==(*pTxLevel)) || (4==(*pTxLevel)) || (8==(*pTxLevel)) || (12==(*pTxLevel)) );
	MES_ASSERT( (1==(*pRxLevel)) || (4==(*pRxLevel)) || (8==(*pRxLevel)) || (12==(*pRxLevel)) );

}

//------------------------------------------------------------------------------
/**
 * @brief       Get Baud Rate & Rx Time Out Configuration
 * @param[out]  pBaudRate       Baud rate division value, It must be greater than 0.
 * @param[out]  pTimeOut        a Rx time-out period ( 0x0 ~ 0xFFFF )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *                                          MES_UART_GetModemConfig
 */
void    MES_UART_GetBaudRateConfig( U16* pBaudRate, U16* pTimeOut )
{
    MES_ASSERT( CNULL != pBaudRate );
    MES_ASSERT( CNULL != pTimeOut );

    *pBaudRate    = MES_UART_GetBRD(  );
    *pTimeOut     = MES_UART_GetRxTimeOut(  );
}

//------------------------------------------------------------------------------
/**
 * @brief       Get UART Modem Configuration
 * @param[out]  pAutoflow       1:( Auto Flow Control Enable ),  0:( Auto Flow Control Disable )
 * @param[out]  pHalfChannel    1:( Half Channel Enable ),       0:( Half Channel Disable )
 * @param[out]  pSCTxEnb        1:( Smart Card Tx Enable ),      0:( Smart Card Tx Disable )
 * @param[out]  pSCRxEnb        1:( Smart Card Rx Enable ),      0:( Smart Card Rx Disable )
 * @param[out]  pDTR            1:( DTR Active),                 0:( DTR InActive )
 * @param[out]  pRTS            1:( RTS Active),                 0:( RTS InActive )
 * @return      None
 * @see         MES_UART_SetLineConfig,     MES_UART_SetControlConfig,
 *              MES_UART_SetFIFOConfig,     MES_UART_SetBaudRateConfig,
 *              MES_UART_SetModemConfig,    MES_UART_GetLineConfig,
 *              MES_UART_GetControlConfig,  MES_UART_GetFIFOConfig,
 *              MES_UART_GetBaudRateConfig 
 */
void    MES_UART_GetModemConfig( U32* pAutoflow, U32* pHalfChannel, U32* pSCTxEnb, U32* pSCRxEnb, U32* pDTR, U32* pRTS )
{
    MES_ASSERT( CNULL != pAutoflow );
    MES_ASSERT( CNULL != pHalfChannel );    
    MES_ASSERT( CNULL != pSCTxEnb );
    MES_ASSERT( CNULL != pSCRxEnb );
    MES_ASSERT( CNULL != pDTR );
    MES_ASSERT( CNULL != pRTS );

    if( CTRUE == MES_UART_GetAutoFlowControl() ){ *pAutoflow = 1; }
    else                                        { *pAutoflow = 0; }

    if( CTRUE == MES_UART_GetHalfChannelEnable() ){ *pHalfChannel = 1; }
    else                                          { *pHalfChannel = 0; }

    if( CTRUE == MES_UART_GetSCTxEnb() ){ *pSCTxEnb = 1; }
    else                                { *pSCTxEnb = 0; }

    if( CTRUE == MES_UART_GetSCRxEnb() ){ *pSCRxEnb = 1; }
    else                                { *pSCRxEnb = 0; }

    if( CTRUE == MES_UART_GetDTR() ){ *pDTR= 1; }
    else                            { *pDTR= 0; }

    if( CTRUE == MES_UART_GetRTS() ){ *pRTS = 1; }
    else                            { *pRTS = 0; }
}

//------------------------------------------------------------------------------
/**
 * @brief   Set Line Control Register
 * @return  None.
 * @see                 SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
void    MES_UART_SetLCON( U16 value )		
{
	MES_ASSERT( CNULL != __g_pRegister );    

    __g_pRegister->LCON = value;
}

//------------------------------------------------------------------------------
/**
 * @brief   Set Control Register
 * @return  None.
 * @see     SetLCON,    
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
void    MES_UART_SetUCON( U16 value )		
{
	MES_ASSERT( CNULL != __g_pRegister );    
	
    __g_pRegister->UCON = value;

}

//------------------------------------------------------------------------------
/**
 * @brief   Set FIFO Control Register
 * @return  None.
 * @see     SetLCON,    SetUCON,
 *                      SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
void    MES_UART_SetFCON( U16 value )       
{
	MES_ASSERT( CNULL != __g_pRegister );    

    __g_pRegister->FCON = value;
}

//------------------------------------------------------------------------------
/**
 * @brief   Set Modem Control Register
 * @return  None.
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
void    MES_UART_SetMCON( U16 value )       
{
	MES_ASSERT( CNULL != __g_pRegister );    

    __g_pRegister->MCON = value;
}

//------------------------------------------------------------------------------
/**
 * @brief   Set TimeOut Register
 * @return  None.
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *                      SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
void    MES_UART_SetTIMEOUT( U16 value )	
{
	MES_ASSERT( CNULL != __g_pRegister );  
    __g_pRegister->TIMEOUT = value;  
}

//------------------------------------------------------------------------------
/**
 * @brief   Set Interrupt Status Register
 * @return  None.
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, 
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
void    MES_UART_SetINTCON( U16 value )		
{
	MES_ASSERT( CNULL != __g_pRegister );    
    __g_pRegister->INTCON = value;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Line Control Register's state
 * @return  Line Control Register's value
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *                      GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
U16     MES_UART_GetLCON( void )
{
	MES_ASSERT( CNULL != __g_pRegister );    
    
    return __g_pRegister->LCON;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Control Register's state
 * @return  Control Register's value
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
U16     MES_UART_GetUCON( void )
{
	MES_ASSERT( CNULL != __g_pRegister );    

    return __g_pRegister->UCON;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get FIFO Control Register's state
 * @return  FIFO Control Register's value
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *                      GetMCON,
 *          GetTIMEOUT, GetINTCON
 */
U16     MES_UART_GetFCON( void )
{
	MES_ASSERT( CNULL != __g_pRegister );    

    return __g_pRegister->FCON;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Modem Control Register's state
 * @return  Modem Control Register's value
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    
 *          GetTIMEOUT, GetINTCON
 */
U16     MES_UART_GetMCON( void )
{
	MES_ASSERT( CNULL != __g_pRegister );    

    return __g_pRegister->MCON;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Time Out Register's Value
 * @return  Time Out Register's Value
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *                      GetINTCON
 */
U16     MES_UART_GetTIMEOUT( void )
{
	MES_ASSERT( CNULL != __g_pRegister );    

    return __g_pRegister->TIMEOUT;
}

//------------------------------------------------------------------------------
/**
 * @brief   Get Interrupt Status Register's state
 * @return  Interrupt Status Register's value
 * @see     SetLCON,    SetUCON,
 *          SetFCON,    SetMCON,
 *          SetTIMEOUT, SetINTCON,
 *          GetLCON,    GetUCON,
 *          GetFCON,    GetMCON,
 *          GetTIMEOUT
 */
U16     MES_UART_GetINTCON( void )
{
	MES_ASSERT( CNULL != __g_pRegister );   
	
    return __g_pRegister->INTCON; 
}



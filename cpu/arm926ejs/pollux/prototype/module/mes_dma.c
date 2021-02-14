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
//	Module     : DMA
//	File       : mes_dma.c
//	Description:
//	Author     : sunny
//	History    :
//		2007/05/17  Sunny	rewrite ( using command buffer )
//		2007/08/16	Goofy 	DMACMDBUSY -> Reserved
//------------------------------------------------------------------------------
#include "mes_dma.h"

/// @brief  DMA Module's Register List
struct  MES_DMA_RegisterSet
{
    volatile U32    DMASRCADDR;     ///< 0x00 : Source Address Register
    volatile U32    DMADSTADDR;     ///< 0x04 :	Destination Address Register
    volatile U16    DMALENGTH;      ///< 0x08 :	Transfer Length Register
    volatile U16    DMAREQID;       ///< 0x0A : Peripheral ID Register
    volatile U32    DMAMODE;        ///< 0x0C : DMA Mode Register
    volatile U32    DMASRCADDR_WB;  ///< 0x10 : Source Address Write Back Register
    volatile U32    DMADSTADDR_WB;  ///< 0x24 :	Destination Address Write Back Register
    volatile U16    DMALENGTH_WB;   ///< 0x18 :	Transfer Length Write Back Register
    volatile U16    DMAREQID_WB;    ///< 0x1A : Peripheral ID Write Back Register
    volatile U32    DMAMODE_WB;     ///< 0x1C : DMA Mode Write Back Register
    volatile U32    DMACMDWAIT;     ///< 0x20 : Command Wait Register
    volatile U32    DMACMDSTOP;     ///< 0x24 : Command Stop Register
//	volatile U32    DMACMDBUSY;     ///< 0x28 : Command Busy Register
	volatile U32	Reserved;		///< 0x28 : Reserved
    volatile U32    DMACMDEMPTY;    ///< 0x2C : Command Empty Space Register
};

static  struct tag_ModuleVariables
{
    struct MES_DMA_RegisterSet *pRegister;
    U32    DMASRCADDR;
    U32    DMADSTADDR;
    U16    DMALENGTH;
    U16    DMAREQID;
    U32    DMAMODE;
    CBOOL  bCommandBufferMode;
} __g_ModuleVariables[NUMBER_OF_DMA_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_DMA_RegisterSet* __g_pRegister = CNULL;
static  struct tag_ModuleVariables* __g_pModuleVariables = CNULL;

//------------------------------------------------------------------------------
//  Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see                                MES_DMA_SelectModule,
 *          MES_DMA_GetCurrentModule,   MES_DMA_GetNumberOfModule
 */
CBOOL   MES_DMA_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;

        for( i=0; i < NUMBER_OF_DMA_MODULE; i++ )
        {
            __g_ModuleVariables[i].pRegister            = CNULL;
            __g_ModuleVariables[i].DMASRCADDR           = 0;
            __g_ModuleVariables[i].DMADSTADDR           = 0;
            __g_ModuleVariables[i].DMALENGTH            = 0;
            __g_ModuleVariables[i].DMAREQID             = 0;
            __g_ModuleVariables[i].DMAMODE              = 0;
            __g_ModuleVariables[i].bCommandBufferMode   = CFALSE;
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
 *  @see        MES_DMA_Initialize,         
 *              MES_DMA_GetCurrentModule,   MES_DMA_GetNumberOfModule
 */
U32    MES_DMA_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_DMA_MODULE > ModuleIndex );

    __g_CurModuleIndex  = ModuleIndex;
    __g_pRegister       = __g_ModuleVariables[ModuleIndex].pRegister;
    __g_pModuleVariables= &__g_ModuleVariables[ModuleIndex];

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_DMA_Initialize,         MES_DMA_SelectModule,
 *                                          MES_DMA_GetNumberOfModule
 */
U32     MES_DMA_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_DMA_Initialize,         MES_DMA_SelectModule,
 *              MES_DMA_GetCurrentModule   
 */
U32     MES_DMA_GetNumberOfModule( void )
{
    return NUMBER_OF_DMA_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                        MES_DMA_GetSizeOfRegisterSet,
 *              MES_DMA_SetBaseAddress,         MES_DMA_GetBaseAddress,
 *              MES_DMA_OpenModule,             MES_DMA_CloseModule,
 *              MES_DMA_CheckBusy,              MES_DMA_CanPowerDown
 */             
U32     MES_DMA_GetPhysicalAddress( void )
{               
    MES_ASSERT( NUMBER_OF_DMA_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_DMA_MODULE + (OFFSET_OF_DMA_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_DMA_GetPhysicalAddress,     
 *              MES_DMA_SetBaseAddress,         MES_DMA_GetBaseAddress,
 *              MES_DMA_OpenModule,             MES_DMA_CloseModule,
 *              MES_DMA_CheckBusy,              MES_DMA_CanPowerDown
 */
U32     MES_DMA_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_DMA_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_DMA_GetPhysicalAddress,     MES_DMA_GetSizeOfRegisterSet,
 *                                              MES_DMA_GetBaseAddress,
 *              MES_DMA_OpenModule,             MES_DMA_CloseModule,
 *              MES_DMA_CheckBusy,              MES_DMA_CanPowerDown
 */
void    MES_DMA_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_DMA_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_DMA_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_DMA_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_DMA_GetPhysicalAddress,     MES_DMA_GetSizeOfRegisterSet,
 *              MES_DMA_SetBaseAddress,         
 *              MES_DMA_OpenModule,             MES_DMA_CloseModule,
 *              MES_DMA_CheckBusy,              MES_DMA_CanPowerDown
 */
U32     MES_DMA_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_DMA_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_DMA_GetPhysicalAddress,     MES_DMA_GetSizeOfRegisterSet,
 *              MES_DMA_SetBaseAddress,         MES_DMA_GetBaseAddress,
 *                                              MES_DMA_CloseModule,
 *              MES_DMA_CheckBusy,              MES_DMA_CanPowerDown
 */
CBOOL   MES_DMA_OpenModule( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CNULL != __g_pModuleVariables );
	MES_ASSERT(	! MES_DMA_CheckBusy() );

    __g_pRegister->DMASRCADDR = 0;
    __g_pRegister->DMADSTADDR = 0;
    __g_pRegister->DMALENGTH  = 0; 
    __g_pRegister->DMAREQID   = 0;  
    __g_pRegister->DMAMODE    = 1UL<<17;   
    
    __g_pModuleVariables->DMASRCADDR           = 0;
    __g_pModuleVariables->DMADSTADDR           = 0;
    __g_pModuleVariables->DMALENGTH            = 0;
    __g_pModuleVariables->DMAREQID             = 0;
    __g_pModuleVariables->DMAMODE              = 0;
    __g_pModuleVariables->bCommandBufferMode   = CFALSE;

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_DMA_GetPhysicalAddress,     MES_DMA_GetSizeOfRegisterSet,
 *              MES_DMA_SetBaseAddress,         MES_DMA_GetBaseAddress,
 *              MES_DMA_OpenModule,             
 *              MES_DMA_CheckBusy,              MES_DMA_CanPowerDown
 */
CBOOL   MES_DMA_CloseModule( void )
{
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( CNULL != __g_pModuleVariables );
	MES_ASSERT(	! MES_DMA_CheckBusy() );

    __g_pRegister->DMASRCADDR = 0;
    __g_pRegister->DMADSTADDR = 0;
    __g_pRegister->DMALENGTH  = 0; 
    __g_pRegister->DMAREQID   = 0;  
    __g_pRegister->DMAMODE    = 1UL<<17;   

    __g_pModuleVariables->DMASRCADDR           = 0;
    __g_pModuleVariables->DMADSTADDR           = 0;
    __g_pModuleVariables->DMALENGTH            = 0;
    __g_pModuleVariables->DMAREQID             = 0;
    __g_pModuleVariables->DMAMODE              = 0;
    __g_pModuleVariables->bCommandBufferMode   = CFALSE;

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_DMA_GetPhysicalAddress,     MES_DMA_GetSizeOfRegisterSet,
 *              MES_DMA_SetBaseAddress,         MES_DMA_GetBaseAddress,
 *              MES_DMA_OpenModule,             MES_DMA_CloseModule,
 *                                              MES_DMA_CanPowerDown
 */
CBOOL   MES_DMA_CheckBusy( void )
{
	return MES_DMA_CheckRunning();
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_DMA_GetPhysicalAddress,     MES_DMA_GetSizeOfRegisterSet,
 *              MES_DMA_SetBaseAddress,         MES_DMA_GetBaseAddress,
 *              MES_DMA_OpenModule,             MES_DMA_CloseModule,
 *              MES_DMA_CheckBusy              
 */
CBOOL   MES_DMA_CanPowerDown( void )
{
    if( CTRUE == MES_DMA_CheckRunning() )
    {
        return CFALSE;
    }

    return CTRUE;
}


//------------------------------------------------------------------------------
// Interrupt Interface

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number for interrupt controller.
 *  @return     Interrupt number
 *  @see                                            MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
S32     MES_DMA_GetInterruptNumber( void )
{
    return  INTNUM_OF_DMA_MODULE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number .
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @see        MES_DMA_GetInterruptNumber,         
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
void    MES_DMA_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    const U32 PEND_POS      = 17;
	const U32 PEND_MASK     = (1UL<<PEND_POS);
  	const U32 INTENB_POS	= 18;
	const U32 INTENB_MASK  	= (1UL<<INTENB_POS);

    register struct MES_DMA_RegisterSet *pRegister;
    register    U32 DmaMode;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( CNULL != __g_pModuleVariables );

    pRegister = __g_pRegister;

    DmaMode = pRegister->DMAMODE;
    DmaMode &= ~(INTENB_MASK | PEND_MASK);
    DmaMode |= (Enable << INTENB_POS);
    pRegister->DMAMODE = DmaMode;

    // We have to update local DMAMODE also for INTENB coherency.
    DmaMode = __g_pModuleVariables->DMAMODE;
    DmaMode &= ~(INTENB_MASK | PEND_MASK);
    DmaMode |= (Enable << INTENB_POS);
    __g_pModuleVariables->DMAMODE = DmaMode;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSe indicate that Interrupt is disabled.
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *                                                  MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
CBOOL   MES_DMA_GetInterruptEnable( S32 IntNum )
{
  	const U32 INTENB_POS	= 18;
	const U32 INTENB_MASK  	= (1UL<<INTENB_POS);

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->DMAMODE & INTENB_MASK )
    {
        return CTRUE;
    }

    return CFALSE;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
CBOOL   MES_DMA_GetInterruptPending( S32 IntNum )
{
	const U32 INTPEND_POS	= 17;
	const U32 INTPEND_MASK 	= (1UL<<INTPEND_POS);

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

	if( __g_pRegister->DMAMODE & INTPEND_MASK )
	{
	    return CTRUE;
	}

    return CFALSE;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear a pending state of specified interrupt.
 *  @param[in]  IntNum  Interrupt number.
 *  @return     None.
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *                                                  MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
void    MES_DMA_ClearInterruptPending( S32 IntNum )
{
  	const U32 INTPEND_POS	= 17;
	const U32 INTPEND_MASK 	= (1UL<<INTPEND_POS);

    register    struct MES_DMA_RegisterSet     *pRegister;
    register    U32 DmaMode;

    MES_ASSERT( 0 == IntNum );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    DmaMode = pRegister->DMAMODE;

    DmaMode |= INTPEND_MASK;

    pRegister->DMAMODE = DmaMode;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
void    MES_DMA_SetInterruptEnableAll( CBOOL Enable )
{
    const U32 PEND_POS      = 17;
	const U32 PEND_MASK     = (1UL<<PEND_POS);
  	const U32 INTENB_POS	= 18;
	const U32 INTENB_MASK  	= (1UL<<INTENB_POS);

    register struct MES_DMA_RegisterSet *pRegister;
    register    U32 DmaMode;

    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( CNULL != __g_pModuleVariables );

    pRegister = __g_pRegister;

    DmaMode = pRegister->DMAMODE;
    DmaMode &= ~(INTENB_MASK | PEND_MASK);
    DmaMode |= (Enable << INTENB_POS);
    pRegister->DMAMODE = DmaMode;

    // We have to update local DMAMODE also for INTENB coherency.
    DmaMode = __g_pModuleVariables->DMAMODE;
    DmaMode &= ~(INTENB_MASK | PEND_MASK);
    DmaMode |= (Enable << INTENB_POS);
    __g_pModuleVariables->DMAMODE = DmaMode;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *                                                  MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
CBOOL   MES_DMA_GetInterruptEnableAll( void )
{
  	const U32 INTENB_POS	= 18;
	const U32 INTENB_MASK  	= (1UL<<INTENB_POS);

    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->DMAMODE_WB & INTENB_MASK )
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
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      
 *              MES_DMA_ClearInterruptPendingAll,   MES_DMA_GetInterruptPendingNumber
 */
CBOOL   MES_DMA_GetInterruptPendingAll( void )
{
	const U32 INTPEND_POS	= 17;
	const U32 INTPEND_MASK 	= (1UL<<INTPEND_POS);

    MES_ASSERT( CNULL != __g_pRegister );

	if( __g_pRegister->DMAMODE & INTPEND_MASK )
	{
	    return CTRUE;
	}

    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *                                                  MES_DMA_GetInterruptPendingNumber
 */
void    MES_DMA_ClearInterruptPendingAll( void )
{
  	const U32 INTPEND_POS	= 17;
	const U32 INTPEND_MASK 	= (1UL<<INTPEND_POS);

    register    struct MES_DMA_RegisterSet     *pRegister;
    register    U32 DmaMode;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

    DmaMode = pRegister->DMAMODE;

    DmaMode |= INTPEND_MASK;

    pRegister->DMAMODE = DmaMode;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_DMA_GetInterruptNumber,         MES_DMA_SetInterruptEnable,
 *              MES_DMA_GetInterruptEnable,         MES_DMA_GetInterruptPending,
 *              MES_DMA_ClearInterruptPending,      MES_DMA_SetInterruptEnableAll,
 *              MES_DMA_GetInterruptEnableAll,      MES_DMA_GetInterruptPendingAll,
 *              MES_DMA_ClearInterruptPendingAll   
 */
S32     MES_DMA_GetInterruptPendingNumber( void )  // -1 if None
{
	const U32 INTPEND_POS	= 17;
	const U32 INTPEND_MASK 	= (1UL<<INTPEND_POS);

    register    struct MES_DMA_RegisterSet     *pRegister = __g_pRegister;

    MES_ASSERT( CNULL != __g_pRegister );

	if( pRegister->DMAMODE & INTPEND_MASK )
	{
	    return 0;
	}

    return -1;
}


//------------------------------------------------------------------------------
// DMA Configuration Function
//------------------------------------------------------------------------------

/**
 *  @brief	    Transfer memory to memory.
 *  @param[in]  pSource      source buffer address.\n this value must be aligned by 64 bits(8 bytes).
 *  @param[in]  pDestination destination buffer address.\n this value must be aligned by 64 bits(8 bytes).
 *  @param[in]  TransferSize transfer size in bytes.
 *  @return	    None.
 *  @see                                        MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
void 	MES_DMA_TransferMemToMem( const void* pSource, void* pDestination, U32 TransferSize )
{
    const       U32   INTENB  = (1UL<<18);
    register    struct tag_ModuleVariables*     pVariables;
	register    U32   regvalue;

    MES_ASSERT ( 0 == (((U32)pSource) % 8) );
    MES_ASSERT ( 0 == (((U32)pDestination) % 8) );
    MES_ASSERT ( (MES_DMA_MAX_TRANSFER_SIZE >= TransferSize) && (0 != TransferSize) );
	MES_ASSERT( CNULL != __g_pModuleVariables );

    pVariables = __g_pModuleVariables;
    regvalue  = pVariables->DMAMODE & INTENB;

	pVariables->DMASRCADDR = (U32)pSource;
	pVariables->DMADSTADDR = (U32)pDestination;
	pVariables->DMALENGTH  = (U16)(TransferSize-1);

	regvalue 	|= (U32)( 0
				| (0UL<< 0) // SRCIOSIZE 0:8bit 1:16bit 2:32bit
				| (0UL<< 2) // SRCIOMODE 0:memory to memory 1:i/o to memory
				| (0UL<< 3) // PACKMODE must be 0 (not supported)
				| (0UL<< 4) // SRCNOTINC 0:Normal, 1:not incremented
				| (1UL<< 5) // SRCNOTREQCHK 1:No Chk, 0:Chk Request, when SRCIOMODE=1
				| (0UL<< 8) // DSTIOSIZE 0:8bit 1:16bit 2:32bit
				| (0UL<<10) // DSTIOMODE 0:memory to memory 1:memory to i/o
				| (0UL<<12)	// DSTNOTINC 0:Normal, 1:not incremented
				| (1UL<<13)	// DSTNOTREQCHK	1:No Chk, 0:Chk Request, when DSTIOMODE=1
				| (1UL<<19) // RUN
				| (0UL<<20) // STOP
				);
	pVariables->DMAMODE = regvalue;

	MES_DMA_Run();
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Transfer memory to peripheral.
 *  @param[in]  pSource			        Source buffer address.
 *  @param[in]  DestinationBaseAddress	A baseaddress of peripheral device.
 *  @param[in]  DestinationPeriID	    An index of peripheral device.
 *  @param[in]  DestinationBitWidth	    IO size, in bits, of peripheral device.
 *  @param[in]  TransferSize			Transfer size in bytes.
 *  @return	    None.
 *  @see        MES_DMA_TransferMemToMem,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
void 	MES_DMA_TransferMemToIO(  const void* pSource,
                				  U32 DestinationBaseAddress,
                				  U32 DestinationPeriID,
                				  U32 DestinationBitWidth,
                				  U32 TransferSize )
{
    const       U32   INTENB  = (1UL<<18);
    register    struct tag_ModuleVariables*     pVariables;
	register    U32   regvalue;

    MES_ASSERT ( 0 == (((U32)pSource) % 4) );
    MES_ASSERT ( 64 > DestinationPeriID );
    MES_ASSERT ( 8 == DestinationBitWidth || 16 == DestinationBitWidth ||
    			  32 == DestinationBitWidth );
    MES_ASSERT ( (MES_DMA_MAX_TRANSFER_SIZE >= TransferSize) && (0 != TransferSize) );
	MES_ASSERT( CNULL != __g_pModuleVariables );

    pVariables = __g_pModuleVariables;
    regvalue  = pVariables->DMAMODE & INTENB;

	pVariables->DMASRCADDR = (U32)pSource;
	pVariables->DMADSTADDR = (U32)DestinationBaseAddress;
	pVariables->DMALENGTH  = (U16)(TransferSize-1);
	pVariables->DMAREQID   = (U16)DestinationPeriID;

	regvalue 	|= (U32)( 0
				| (0UL<< 0) // SRCIOSIZE 0:8bit 1:16bit 2:32bit
				| (0UL<< 2) // SRCIOMODE 0:memory to memory 1:i/o to memory
				| (0UL<< 3) // PACKMODE must be 0 (not supported)
				| (0UL<< 4) // SRCNOTINC 0:Normal, 1:not incremented
				| (1UL<< 5) // SRCNOTREQCHK 1:No Chk, 0:Chk Request, when SRCIOMODE=1
				| ((DestinationBitWidth>>4)<< 8) // DSTIOSIZE 0:8bit 1:16bit 2:32bit
				| (1UL<<10) // DSTIOMODE 0:memory to memory 1:memory to i/o
				| (1UL<<12)	// DSTNOTINC 0:Normal, 1:not incremented
				| (0UL<<13)	// DSTNOTREQCHK	1:No Chk, 0:Chk Request, when DSTIOMODE=1
				| (1UL<<19) // RUN
				| (0UL<<20) // STOP
				);
	pVariables->DMAMODE = regvalue;

	MES_DMA_Run();
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Transfer peripheral to memory.
 *  @param[in]  SourceBaseAddress	A baseaddress of peripheral device.
 *  @param[in]  SourcePeriID	    An index of peripheral device.
 *  @param[in]  SourceBitWidth		IO size, in bits, of peripheral device.
 *  @param[in]  pDestination 		Destination buffer address.
 *  @param[in]  TransferSize		Transfer size in bytes.
 *  @return	    None.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *                                              MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
void 	MES_DMA_TransferIOToMem( U32 SourceBaseAddress,
                				  U32 SourcePeriID,
                				  U32 SourceBitWidth,
                				  void* pDestination,
                				  U32 TransferSize )
{
    const       U32   INTENB  = (1UL<<18);
    register    struct tag_ModuleVariables*     pVariables;
	register    U32   regvalue;

    MES_ASSERT ( 64 > SourcePeriID );
    MES_ASSERT ( 8 == SourceBitWidth || 16 == SourceBitWidth ||
    			  32 == SourceBitWidth );
    MES_ASSERT ( 0 == (((U32)pDestination) % 4) );
    MES_ASSERT ( (MES_DMA_MAX_TRANSFER_SIZE >= TransferSize) && (0 != TransferSize) );
	MES_ASSERT( CNULL != __g_pModuleVariables );

    pVariables = __g_pModuleVariables;
    regvalue  = pVariables->DMAMODE & INTENB;

	pVariables->DMASRCADDR = (U32)SourceBaseAddress;
	pVariables->DMADSTADDR = (U32)pDestination;
	pVariables->DMALENGTH  = (U16)(TransferSize-1);
	pVariables->DMAREQID   = (U16)SourcePeriID;

	regvalue 	|= (U32)( 0
				| ((SourceBitWidth>>4)<< 0) // SRCIOSIZE 0:8bit 1:16bit 2:32bit
				| (1UL<< 2) // SRCIOMODE 0:memory to memory 1:i/o to memory
				| (0UL<< 3) // PACKMODE must be 0 (not supported)
				| (1UL<< 4) // SRCNOTINC 0:Normal, 1:not incremented
				| (0UL<< 5) // SRCNOTREQCHK 1:No Chk, 0:Chk Request, when SRCIOMODE=1
				| (0UL<< 8) // DSTIOSIZE 0:8bit 1:16bit 2:32bit
				| (0UL<<10) // DSTIOMODE 0:memory to memory 1:memory to i/o
				| (0UL<<12)	// DSTNOTINC 0:Normal, 1:not incremented
				| (1UL<<13)	// DSTNOTREQCHK	1:No Chk, 0:Chk Request, when DSTIOMODE=1
				| (1UL<<19) // RUN
				| (0UL<<20) // STOP
				);
	pVariables->DMAMODE = regvalue;

	MES_DMA_Run();
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get a maximum size for DMA transfer at once.
 *  @return	    A maximum transfer size.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
U32 	MES_DMA_GetMaxTransferSize ( void )
{
    return    MES_DMA_MAX_TRANSFER_SIZE;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a source address.
 *  @param[in]  dwAddr  A source address
 *  @return	    None.
 *	@remarks    
 *              - If the source is peripheral device, set it to base address of peripheral device.
 *              - If the source is memory and destination is peripheral device,
 *		    	  the source address must be aligned by IO size of peripheral device.
 *              - If the source and the destination are memory, source address must
 *			      be aligned by 64 bit(8 bytes).
 *  @see         MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *               MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *                                               MES_DMA_GetSourceAddress,
 *               MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *               MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *               MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *               MES_DMA_SetAttribute
 */
void	MES_DMA_SetSourceAddress( U32 dwAddr )
{
   	MES_ASSERT( CNULL != __g_pModuleVariables );

	__g_pModuleVariables->DMASRCADDR = (U32)dwAddr;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get a source address.
 *  @return	    A source address.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
U32		MES_DMA_GetSourceAddress( void )
{
   	MES_ASSERT( CNULL != __g_pRegister );

	return __g_pRegister->DMASRCADDR;

}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a destination address.
 *  @param[in]  dwAddr   A destination address.
 *  @return	    None.
 *	@remarks
 *              - If the destination is peripheral device, set it to base address of
 *			      peripheral device.
 *              - If the source is peripheral device and destination is memory,
 *			      the destination address must be aligned by IO size of peripheral
 *	    		  device.
 *              - If the source and the destination are memory, destination address
 *			      must be aligned by 64 bit(8 bytes).
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *                                              MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
void	MES_DMA_SetDestinationAddress( U32 dwAddr )
{
   	MES_ASSERT( CNULL != __g_pModuleVariables );

	__g_pModuleVariables->DMADSTADDR = (U32)dwAddr;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get a destination address.
 *  @return	    a destination address.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
U32		MES_DMA_GetDestinationAddress( void )
{
   	MES_ASSERT( CNULL != __g_pRegister );

	return __g_pRegister->DMADSTADDR;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a transfer size in bytes.
 *  @param[in]  dwSize A transfer size in bytes.
 *  @return	    None.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *                                              MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
void	MES_DMA_SetDataSize( U32 dwSize )
{
	MES_ASSERT( (MES_DMA_MAX_TRANSFER_SIZE >= dwSize) && (0 != dwSize) );
   	MES_ASSERT( CNULL != __g_pModuleVariables );
   	MES_ASSERT( CNULL != __g_pRegister );

    if ( !__g_pModuleVariables->bCommandBufferMode )
    {
        __g_pRegister->DMALENGTH = (U16)(dwSize - 1);
    }
	__g_pModuleVariables->DMALENGTH = (U16)(dwSize - 1);
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get a transfer size in bytes.
 *  @return	    A transfer size in bytes.
 *	@remarks    This function informs a remained data size to be transferred while
 *			    DMA transfer is running.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
U32		MES_DMA_GetDataSize( void )
{
   	MES_ASSERT( CNULL != __g_pRegister );

	return (U32)__g_pRegister->DMALENGTH;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a peripheral index.
 *  @param[in]  dwIndex A peripheral index.
 *  @return	    None.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *                                              MES_DMA_GetIOIndex,
 *              MES_DMA_SetAttribute
 */
void	MES_DMA_SetIOIndex( U32 dwIndex )
{
	MES_ASSERT( 64 > dwIndex );
   	MES_ASSERT( CNULL != __g_pModuleVariables );

	__g_pModuleVariables->DMAREQID = (U16)dwIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get a peripheral index.
 *  @return	    A peripheral index.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,
 *              MES_DMA_SetAttribute
 */
U32		MES_DMA_GetIOIndex( void )
{
   	MES_ASSERT( CNULL != __g_pModuleVariables );

	return (U32)__g_pModuleVariables->DMAREQID;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a DMA transfer mode.
 *  @param[in]  OpMode		A DMA transfer mode.
 *  @param[in]  IOBitWidth	IO size, in bits, of peripheral device.\n
 *	                      	If OpMode is TRANSFER_MEM_TO_MEM, this parameter is not used.
 *	@param[in] 	bSrcMemInc	Specifies whether to increase source address or not.\n
 *							This value is only used when source is memory and
 * 							default	is CTRUE.
 *	@param[in] 	bDstMemInc	Specifies whether to increase destination address or not.\n
 *							This value is only used when destination is memory and
 * 							default	is CTRUE.
 *  @return	    None.
 *	@remark	    You have not to call this function while DMA transfer is busy.
 *  @see        MES_DMA_TransferMemToMem,       MES_DMA_TransferMemToIO,
 *              MES_DMA_TransferIOToMem,        MES_DMA_GetMaxTransferSize,
 *              MES_DMA_SetSourceAddress,       MES_DMA_GetSourceAddress,
 *              MES_DMA_SetDestinationAddress,  MES_DMA_GetDestinationAddress,
 *              MES_DMA_SetDataSize,            MES_DMA_GetDataSize,
 *              MES_DMA_SetIOIndex,             MES_DMA_GetIOIndex
 */
void	MES_DMA_SetAttribute( MES_DMA_OPMODE OpMode, U32 IOBitWidth, CBOOL bSrcMemInc, CBOOL bDstMemInc )
{
    const U32    INTENB  = (1UL<<18);
    register    struct tag_ModuleVariables      *pVariables;
	register U32 regvalue;

	MES_ASSERT ( 3 > OpMode );
    MES_ASSERT ( MES_DMA_OPMODE_MEM_TO_MEM == OpMode || 8 == IOBitWidth || 16 == IOBitWidth || 32 == IOBitWidth );
   	MES_ASSERT( CNULL != __g_pModuleVariables );

    pVariables = __g_pModuleVariables;
    regvalue = pVariables->DMAMODE & INTENB;

	switch( OpMode )
	{
	case MES_DMA_OPMODE_MEM_TO_MEM :
		regvalue 	|= (U32)( 0
					| (0UL<< 0) // SRCIOSIZE 0:8bit 1:16bit 2:32bit
					| (0UL<< 2) // SRCIOMODE 0:memory to memory 1:i/o to memory
					| (0UL<< 3) // PACKMODE must be 0 (not supported)
					| (0UL<< 4) // SRCNOTINC 0:Normal, 1:not incremented
					| (1UL<< 5) // SRCNOTREQCHK 1:No Chk, 0:Chk Request, when SRCIOMODE=1
					| (0UL<< 8) // DSTIOSIZE 0:8bit 1:16bit 2:32bit
					| (0UL<<10) // DSTIOMODE 0:memory to memory 1:memory to i/o
					| (0UL<<12)	// DSTNOTINC 0:Normal, 1:not incremented
					| (1UL<<13)	// DSTNOTREQCHK	1:No Chk, 0:Chk Request, when DSTIOMODE=1
					| (0UL<<19) // RUN
        			| (0UL<<20) // STOP
					);
		if( bSrcMemInc == CFALSE )	regvalue |= (1UL<< 4);	// SRCNOTINC
		if( bDstMemInc == CFALSE )	regvalue |= (1UL<<12);	// DSTNOTINC
		break;

	case MES_DMA_OPMODE_MEM_TO_IO :
		regvalue 	|= (U32)( 0
					| (0UL<< 0) // SRCIOSIZE 0:8bit 1:16bit 2:32bit
					| (0UL<< 2) // SRCIOMODE 0:memory to memory 1:i/o to memory
					| (0UL<< 3) // PACKMODE must be 0 (not supported)
					| (0UL<< 4) // SRCNOTINC 0:Normal, 1:not incremented
					| (1UL<< 5) // SRCNOTREQCHK 1:No Chk, 0:Chk Request, when SRCIOMODE=1
					| ((IOBitWidth>>4)<< 8) // DSTIOSIZE 0:8bit 1:16bit 2:32bit
					| (1UL<<10) // DSTIOMODE 0:memory to memory 1:memory to i/o
					| (1UL<<12)	// DSTNOTINC 0:Normal, 1:not incremented
					| (0UL<<13)	// DSTNOTREQCHK	1:No Chk, 0:Chk Request, when DSTIOMODE=1
					| (0UL<<19) // RUN
        			| (0UL<<20) // STOP
					);
		if( bSrcMemInc == CFALSE )	regvalue |= (1UL<< 4);	// SRCNOTINC
		break;

	case MES_DMA_OPMODE_IO_TO_MEM :
		regvalue 	|= (U32)( 0
					| ((IOBitWidth>>4)<< 0) // SRCIOSIZE 0:8bit 1:16bit 2:32bit
					| (1UL<< 2) // SRCIOMODE 0:memory to memory 1:i/o to memory
					| (0UL<< 3) // PACKMODE must be 0 (not supported)
					| (1UL<< 4) // SRCNOTINC 0:Normal, 1:not incremented
					| (0UL<< 5) // SRCNOTREQCHK 1:No Chk, 0:Chk Request, when SRCIOMODE=1
					| (0UL<< 8) // DSTIOSIZE 0:8bit 1:16bit 2:32bit
					| (0UL<<10) // DSTIOMODE 0:memory to memory 1:memory to i/o
					| (0UL<<12)	// DSTNOTINC 0:Normal, 1:not incremented
					| (1UL<<13)	// DSTNOTREQCHK	1:No Chk, 0:Chk Request, when DSTIOMODE=1
					| (0UL<<19) // RUN
        			| (0UL<<20) // STOP
					);
		if( bDstMemInc == CFALSE )	regvalue |= (1UL<<12);	// DSTNOTINC
		break;
	}
	pVariables->DMAMODE = regvalue;
}

//------------------------------------------------------------------------------
// DMA Operation Function
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief	    Set DMA operation mode ( Write Back or Write Through )
 *  @param[in]  bEnb  \b CTRUE indicate that DMA opeate Write Back mode. \n
 *                    \b CFALSE indicate that DMA opeate Write Through mode.
 *  @return	    None.
 *	@remarks	\b Write \b Back Mode can save next DMA setting value using command buffer during DMA is run.\n
 *              But, Save only one setting value.\n
 *                  - 1. DMA setting
 *                  - 2. DMA run
 *                  - 3. save next DMA command
 *                  - 4. if DMA is completed, saved DMA command execute.
 *                  - 5. DMA complete
 *                      - If next command exist, then save next DMA command ( repeat 3 - 4 ).
 *
 *	@remarks	\b Write \b Through Mode can't save next DMA command. so DMA operate below.
 *                  - 1. DMA setting
 *                  - 2. DMA run
 *                  - 3. DMA complete
 *                  - 4. repeat 1 - 3.
 *  @see                                        MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferReady,   
 *              MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
void    MES_DMA_SetCommandBufferMode( CBOOL bEnb )
{
    MES_ASSERT( (0==bEnb) || (1==bEnb) );
    MES_ASSERT( CNULL != __g_pModuleVariables );

    __g_pModuleVariables->bCommandBufferMode = bEnb;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get DMA operation mode
 *  @return	    \b CTRUE indicate that DMA opeation mode is Write Back. \n
 *              \b CFALSE indicate that DMA opeation mode is Write Through.
 *  @see        MES_DMA_SetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferReady,   
 *              MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
CBOOL   MES_DMA_GetCommandBufferMode( void )
{
    MES_ASSERT( CNULL != __g_pModuleVariables );

    return __g_pModuleVariables->bCommandBufferMode;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Run DMA transfer.
 *  @return	    None.
 *  @remarks    DMA have different sequence by DMA operation mode ( Wirte Back or Write Through ). \n
 *  @code
 *              //--------------------------------------------------------------
 *              // Write Back Mode Operation
 *              //--------------------------------------------------------------
 *
 *              MES_DMA_SetCommandBufferMode( CTRUE );      // DMA set Write Back Mode
 *              ...                                         // DMA configuration
 *
 *              while( MES_DMA_IsCommandBufferReady() )     // Check command buffer
 *              {
 *                  CNULL;
 *              }
 *               MES_DMA_Run();                             // DMA run
 *
 *              //--------------------------------------------------------------
 *              // Write Through Mode Operation
 *              //--------------------------------------------------------------
 *
 *              MES_DMA_SetCommandBufferMode( CFALSE );     // DMA set Write Through Mode
 *              ...
 *
 *              MES_DMA_Run();                              // DMA run

 *  @endcode
 *  @see        MES_DMA_SetCommandBufferMode,   MES_DMA_GetCommandBufferMode,
 *                                              MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferReady,   
 *              MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
void	MES_DMA_Run( void )
{
	const U32 RUN       = (1UL<<19);
    MES_DEBUG_CODE(
	const U32 INTPEND   = (1UL<<17);
	const U32 STOP      = (1UL<<20); )

    register    struct  tag_ModuleVariables     *pVariables;
    register    struct  MES_DMA_RegisterSet     *pRegister;
	volatile    U32 *pSrcAddr, *pDestAddr;

	MES_ASSERT( CNULL != __g_pModuleVariables );
	MES_ASSERT( CNULL != __g_pRegister );

    // save loacl variable DMAMODE for run bit setting
    pRegister   = __g_pRegister;
    pVariables  = __g_pModuleVariables;

    MES_ASSERT( 0 == (pVariables->DMAMODE & (INTPEND|STOP) ) );
    pVariables->DMAMODE |= RUN;

    // command copy according to Write Back or Wirte Through Mode.
    pSrcAddr     =   &(pVariables->DMASRCADDR);

    MES_ASSERT( MES_DMA_GetCommandBufferMode() || !MES_DMA_CheckRunning() );
    MES_ASSERT( !MES_DMA_GetCommandBufferMode() || MES_DMA_IsCommandBufferReady() );

	if( pVariables->bCommandBufferMode )
	{
        pDestAddr    =   &(pRegister->DMASRCADDR_WB);
	    pRegister->DMACMDWAIT = 1;  // command wait
	}
	else    // write through
    {
        pDestAddr    =   &(pRegister->DMASRCADDR);
    }

    *pDestAddr++ = *pSrcAddr++;
    *pDestAddr++ = *pSrcAddr++;
    *pDestAddr++ = *pSrcAddr++;
    *pDestAddr++ = *pSrcAddr++;
}

//------------------------------------------------------------------------------
/**
 *  @brief	Check whether DMA transfer is running or not.
 *  @return	\b CTURE indicates DMA transfer is running.\n
 *			\b CFALSE indicates DMA transfer is idle.
 *  @see    MES_DMA_SetCommandBufferMode,   MES_DMA_GetCommandBufferMode,
 *          MES_DMA_Run,
 *          MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *          MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferReady,   
 *          MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
CBOOL 	MES_DMA_CheckRunning ( void )
{
    const U32   BUSY_POS    = 16;
	const U32   BUSY_MASK	= (1UL<<BUSY_POS);
    register    struct MES_DMA_RegisterSet     *pRegister;

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;

	return (CBOOL)((pRegister->DMAMODE & BUSY_MASK) >> BUSY_POS);
}


//------------------------------------------------------------------------------
/**
 *  @brief	    Stop/Cancel DMA Transfer.
 *  @param      Enable  \b CTRUE indicate that DMA Stop is Enable. \n
 *                      \b CFALSE indicate that DMA Stop is Disable.
 *  @return	    None.
 *	@remark     If DMA running write back mode, then user have to clear command buffer.\n
 *              Also, user should check command buffer flush is completed or not.\n \n
 *              After MES_DMA_Stop( CTRUE ), user should check DMA running status. \n
 *              If DMA is really stopped, user should setting MES_DMA_Stop( CFALSE ).
 *	@code
 *              if( MES_DMA_GetCommandBufferMode() )
 *              {
 *                  MES_DMA_CommandBufferFlush();                      // Command Buffer Flush
 *                  while(  CTRUE != MES_DMA_IsCommandBufferFlush() )   // wait during command buffer flushing
 *                  { CNULL; }
 *              }
 *
 *              MES_DMA_Stop(CTRUE);                                    
 *              while( MES_DMA_CheckRunning() )
 *              {
 *                  CNULL;                                              // wait during DMA stop
 *              }
 *              MES_DMA_Stop(CFALSE);
 *  @endcode
 *  @see        MES_DMA_SetCommandBufferMode,   MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *                                              MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferReady,   
 *              MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
void 	MES_DMA_Stop ( CBOOL Enable )
{
   	const U32 RUN	= (1UL<<19);
   	const U32 STOP	= (1UL<<20);

    register    struct MES_DMA_RegisterSet     *pRegister;
    register    U32 DmaMode;

    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;
    DmaMode = pRegister->DMAMODE;

    if( Enable )
    {
        DmaMode =  ( DmaMode & ~RUN ) | STOP ;
    }
    else
    {
        DmaMode = ( DmaMode & ~STOP );
    }

    pRegister->DMAMODE = DmaMode;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    DMA's command buffer flushing.
 *  @return	    None.
 *	@remarks    After command buffer flushing, should check command buffer state.
 *  @code
 *              MES_DMA_CommandBufferFlush();                      // command buffer flushing
 *              while( CTRUE != MES_DMA_IsCommandBufferFlush() )    // wait during command buffer flushing
 *              {
 *                  CNULL;
 *              }
 *  @endcode
 *  @see        MES_DMA_SetCommandBufferMode,   MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferReady,   
 *              MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
void    MES_DMA_CommandBufferFlush( void )
{
    MES_ASSERT( CNULL != __g_pRegister );

    __g_pRegister->DMACMDSTOP = 1;
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Get state of DMA's command buffer
 *  @return     \b CTRUE indicate that command buffer flush is completed. \n
 *              \b CFALSE indicate that command buffer flush is NOT completed.
 *	@remarks
 *  @see        MES_DMA_SetCommandBufferMode,           MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *                                              MES_DMA_IsCommandBufferReady,   
 *              MES_DMA_IsCommandBufferEmpty,   MES_DMA_GetCommandBufferFreeCount
 */
CBOOL   MES_DMA_IsCommandBufferFlush( void )
{
    const U32 STOP_MASK = 1UL;

    MES_ASSERT( CNULL != __g_pRegister );

    return (CBOOL)((__g_pRegister->DMACMDSTOP & STOP_MASK) ^ 0x01);    // 0: Complete,  1: flushing.
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Check DMA's available command buffer
 *  @return	    \b CTRUE indicate that command buffer is available. \n
 *              \b CFALSE indicate that command buffer is NOT available. \n
 *  @see        MES_DMA_SetCommandBufferMode,           MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferEmpty, 
 *              MES_DMA_GetCommandBufferFreeCount
 */
CBOOL   MES_DMA_IsCommandBufferReady( void )
{
/*
    MES_ASSERT( CNULL != __g_pRegister );
	return (5<=__g_pRegister->DMACMDEMPTY) ? CTRUE : CFALSE;
*/    
    register U32 reg;
    MES_ASSERT( CNULL != __g_pRegister );
    reg = __g_pRegister->DMACMDEMPTY;
    return (CBOOL)((reg >> 2) & (reg & 1));	// DMACMDEMPTY[2] & DMACMDEMPTY[0]
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Check command buffer is empty or Not
 *  @return	    \b CTRUE indicate that command buffer is Empty. \n
 *              \b CFALSE indicate that command buffer is NOT Empty. \n
 *  @see        MES_DMA_SetCommandBufferMode,   MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_GetCommandBufferFreeCount
 */
CBOOL   MES_DMA_IsCommandBufferEmpty( void )
{
/*	
    MES_ASSERT( CNULL != __g_pRegister );
	return (5==__g_pRegister->DMACMDEMPTY) ? CTRUE : CFALSE;
*/	
    register U32 reg;
    MES_ASSERT( CNULL != __g_pRegister );
    reg = __g_pRegister->DMACMDEMPTY;
    return (CBOOL)((reg >> 2) & (reg & 1));	// DMACMDEMPTY[2] & DMACMDEMPTY[0]
}

//------------------------------------------------------------------------------
/**
 *  @brief	    Check count of DMA's available command buffer 
 *  @return	    Get count of DMA's available command buffer 
 *  @see        MES_DMA_SetCommandBufferMode,           MES_DMA_GetCommandBufferMode,
 *              MES_DMA_Run,                    MES_DMA_CheckRunning,
 *              MES_DMA_Stop,                   MES_DMA_CommandBufferFlush,
 *              MES_DMA_IsCommandBufferFlush,   MES_DMA_IsCommandBufferEmpty
 */
U32     MES_DMA_GetCommandBufferFreeCount( void )
{
    MES_ASSERT( CNULL != __g_pRegister );
    return __g_pRegister->DMACMDEMPTY;    
}


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
//	Module     : Interrupt Controller
//	File       : mes_intc.c
//	Description: 
//	Author     : 
//	History    : 
//                  2007/04/03  first 
//------------------------------------------------------------------------------

#include "mes_intc.h"

//------------------------------------------------------------------------------
/// @brief	INTC03 register set structure.
//------------------------------------------------------------------------------
struct MES_INTC_RegisterSet
{
	const 	 U32 __Reserved[2];	///< 0x00 ~ 0x07 : Reserved
    volatile U32 INTMODE[2];	///< 0x08, 0x0C : Interrupt Mode Register
	volatile U32 INTMASK[2];	///< 0x10, 0x14 : Interrupt Mask Register
	volatile U32 PRIORDER;		///< 0x18 : Priority Order Register
	volatile U32 __Reserved01;	///< 0x1C : Reserved
	volatile U32 INTPEND[2];	///< 0x20, 0x24 : Interrupt Pending Register
	const    U32 __Reserved02;	///< 0x28 : Reserved
};

static  struct
{
    struct MES_INTC_RegisterSet *pRegister;   
} __g_ModuleVariables[NUMBER_OF_INTC_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_INTC_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
//   Module Interface
//------------------------------------------------------------------------------

/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.
 *  @see                                 MES_INTC_SelectModule,
 *          MES_INTC_GetCurrentModule,   MES_INTC_GetNumberOfModule
 */
CBOOL   MES_INTC_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_INTC_MODULE; i++ )
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
 *  @see        MES_INTC_Initialize,         
 *              MES_INTC_GetCurrentModule,   MES_INTC_GetNumberOfModule
 */
U32    MES_INTC_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_INTC_MODULE > ModuleIndex );
    
    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_INTC_Initialize,         MES_INTC_SelectModule,
 *                                           MES_INTC_GetNumberOfModule
 */
U32     MES_INTC_GetCurrentModule( void )
{
    return __g_CurModuleIndex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_INTC_Initialize,         MES_INTC_SelectModule,
 *              MES_INTC_GetCurrentModule   
 */
U32     MES_INTC_GetNumberOfModule( void )
{
    return NUMBER_OF_INTC_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                         MES_INTC_GetSizeOfRegisterSet,
 *              MES_INTC_SetBaseAddress,         MES_INTC_GetBaseAddress,
 *              MES_INTC_OpenModule,             MES_INTC_CloseModule,
 *              MES_INTC_CheckBusy,              MES_INTC_CanPowerDown
 */
U32     MES_INTC_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_INTC_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_INTC_MODULE );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_INTC_GetPhysicalAddress,     
 *              MES_INTC_SetBaseAddress,         MES_INTC_GetBaseAddress,
 *              MES_INTC_OpenModule,             MES_INTC_CloseModule,
 *              MES_INTC_CheckBusy,              MES_INTC_CanPowerDown
 */
U32     MES_INTC_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_INTC_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_INTC_GetPhysicalAddress,     MES_INTC_GetSizeOfRegisterSet,
 *                                               MES_INTC_GetBaseAddress,
 *              MES_INTC_OpenModule,             MES_INTC_CloseModule,
 *              MES_INTC_CheckBusy,              MES_INTC_CanPowerDown
 */
void    MES_INTC_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_INTC_MODULE > __g_CurModuleIndex );    
    
    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_INTC_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_INTC_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_INTC_GetPhysicalAddress,     MES_INTC_GetSizeOfRegisterSet,
 *              MES_INTC_SetBaseAddress,         
 *              MES_INTC_OpenModule,             MES_INTC_CloseModule,
 *              MES_INTC_CheckBusy,              MES_INTC_CanPowerDown
 */
U32     MES_INTC_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_INTC_MODULE > __g_CurModuleIndex );
        
    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;
}


//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_INTC_GetPhysicalAddress,     MES_INTC_GetSizeOfRegisterSet,
 *              MES_INTC_SetBaseAddress,         MES_INTC_GetBaseAddress,
 *                                               MES_INTC_CloseModule,
 *              MES_INTC_CheckBusy,              MES_INTC_CanPowerDown
 */
CBOOL   MES_INTC_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_INTC_GetPhysicalAddress,     MES_INTC_GetSizeOfRegisterSet,
 *              MES_INTC_SetBaseAddress,         MES_INTC_GetBaseAddress,
 *              MES_INTC_OpenModule,             
 *              MES_INTC_CheckBusy,              MES_INTC_CanPowerDown
 */
CBOOL   MES_INTC_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_INTC_GetPhysicalAddress,     MES_INTC_GetSizeOfRegisterSet,
 *              MES_INTC_SetBaseAddress,         MES_INTC_GetBaseAddress,
 *              MES_INTC_OpenModule,             MES_INTC_CloseModule,
 *                                               MES_INTC_CanPowerDown
 */
CBOOL   MES_INTC_CheckBusy( void )
{
    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_INTC_GetPhysicalAddress,     MES_INTC_GetSizeOfRegisterSet,
 *              MES_INTC_SetBaseAddress,         MES_INTC_GetBaseAddress,
 *              MES_INTC_OpenModule,             MES_INTC_CloseModule,
 *              MES_INTC_CheckBusy              
 */
CBOOL   MES_INTC_CanPowerDown( void )
{
    return CTRUE;
}

//------------------------------------------------------------------------------
// Interrupt Interface
//------------------------------------------------------------------------------

/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number .
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @see                 
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
void    MES_INTC_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
	register struct MES_INTC_RegisterSet *pRegister;
	register int RegSel, RegBit;
	register U32 regvalue;
				
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( MES_INTC_NUM_OF_INT > IntNum );
	MES_ASSERT( (0==Enable) || (1==Enable) );
	Enable = (CBOOL)((U32)Enable ^ 1);	// Service:0, Masked:1
	
    pRegister = __g_pRegister;
    RegSel  = IntNum >> 5;      // 0 or 1
    RegBit  = IntNum & 0x1F;    // 0 ~ 31
    
    regvalue = pRegister->INTMASK[RegSel];
    regvalue &= ~( (U32)1      << RegBit);
    regvalue |=  ( (U32)Enable << RegBit);
    pRegister->INTMASK[RegSel] = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSe indicate that Interrupt is disabled.
 *  @see        MES_INTC_SetInterruptEnable,
 *                                                   MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
CBOOL   MES_INTC_GetInterruptEnable( S32 IntNum )
{
	register struct MES_INTC_RegisterSet *pRegister;
	register int RegSel, RegBit;
	register U32 regvalue;
				
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( MES_INTC_NUM_OF_INT > IntNum );

    pRegister = __g_pRegister;
    RegSel  = IntNum >> 5;      // 0 or 1
    RegBit  = IntNum & 0x1F;    // 0 ~ 31

    regvalue = pRegister->INTMASK[RegSel];

    if( regvalue & ( 1UL << RegBit) )
    {
        return CFALSE;   
    }    
    
    return CTRUE;   
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is pended or not
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Pending is seted. \n
 *              \b CFALSE indicate that Pending is Not Seted.
 *  @see        MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
CBOOL   MES_INTC_GetInterruptPending( S32 IntNum )
{
	register struct MES_INTC_RegisterSet *pRegister;
	register int RegSel, RegBit;
	register U32 regvalue;
				
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( MES_INTC_NUM_OF_INT > IntNum );

    pRegister = __g_pRegister;
    RegSel  = IntNum >> 5;      // 0 or 1
    RegBit  = IntNum & 0x1F;    // 0 ~ 31

    regvalue = pRegister->INTPEND[RegSel];
    
    if( regvalue & ( 1UL << RegBit) )
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
 *  @see                                             MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *                                                   MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
void    MES_INTC_ClearInterruptPending( S32 IntNum )
{
	register struct MES_INTC_RegisterSet *pRegister;
	register int RegSel, RegBit;
	register U32 regvalue;
				
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( MES_INTC_NUM_OF_INT > IntNum );

    pRegister = __g_pRegister;
    RegSel  = IntNum >> 5;      // 0 or 1
    RegBit  = IntNum & 0x1F;    // 0 ~ 31

    regvalue = pRegister->INTPEND[RegSel];
    regvalue = 1UL << RegBit;
    pRegister->INTPEND[RegSel] = regvalue;

}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see                                             MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
void    MES_INTC_SetInterruptEnableAll( CBOOL Enable )
{
	register struct MES_INTC_RegisterSet *pRegister;
	register U32 regvalue;
				
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( (0==Enable) || (1==Enable) );

    pRegister = __g_pRegister;

    if( Enable )
    {   regvalue = 0;   }
    else  
    {   regvalue = (U32)(~0); }

    pRegister->INTMASK[0] = regvalue;
    pRegister->INTMASK[1] = regvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see                                             MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *                                                   MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
CBOOL   MES_INTC_GetInterruptEnableAll( void )
{
    register struct MES_INTC_RegisterSet *pRegister;	

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;
    
	return (CBOOL)( (0xFFFFFFFF != pRegister->INTMASK[0]) || (0xFFFFFFFF != pRegister->INTMASK[1]) );    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are pended or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) pending is seted. \n
 *              \b CFALSE indicate that All pending is NOT seted.
 *  @see                                             MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      
 *              MES_INTC_ClearInterruptPendingAll,   MES_INTC_GetInterruptPendingNumber
 */
CBOOL   MES_INTC_GetInterruptPendingAll( void )
{
    register struct MES_INTC_RegisterSet *pRegister;	

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;	
	
	return (CBOOL)( (0 != pRegister->INTPEND[0]) || (0 != pRegister->INTPEND[1]) );	
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see                                             MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *                                                   MES_INTC_GetInterruptPendingNumber
 */
void    MES_INTC_ClearInterruptPendingAll( void )
{
    register struct MES_INTC_RegisterSet *pRegister;	

	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;


	pRegister->INTPEND[0] = (U32)(0xFFFFFFFF);
	pRegister->INTPEND[1] = (U32)(0xFFFFFFFF);    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see                                             MES_INTC_SetInterruptEnable,
 *              MES_INTC_GetInterruptEnable,         MES_INTC_GetInterruptPending,
 *              MES_INTC_ClearInterruptPending,      MES_INTC_SetInterruptEnableAll,
 *              MES_INTC_GetInterruptEnableAll,      MES_INTC_GetInterruptPendingAll,
 *              MES_INTC_ClearInterruptPendingAll   
 */
S32     MES_INTC_GetInterruptPendingNumber( void )  // -1 if None
{
	U32 intnum, intoff;
	U32 intpend;
	register struct MES_INTC_RegisterSet *pRegister;
	
	MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;	

	intoff	= 0;
	intpend = pRegister->INTPEND[0];
	if( intpend == 0)
	{
		intoff	= 32;
		intpend = pRegister->INTPEND[1];
	}

	for( intnum=0 ; intnum<32 ; intnum++ )
	{
		if( 0 != (intpend & (1UL<<intnum)) )
		{
		    return ( intnum + intoff );
		}
	}	
    
    return -1;
}


//------------------------------------------------------------------------------
//Interrupt Controller Operation.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 *  @brief	    Set an interrupt mode to be generated when the corresponding interrupt is issued.
 *  @param[in]  IntNum		An interrupt number.          
 *  @param[in]  IntMode		An interrupt mode, IRQ or FIQ.
 *  @return	    None.
 */
void 	MES_INTC_SetInterruptMode ( U32 IntNum, MES_INTC_INTMODE IntMode  )
{
	register struct MES_INTC_RegisterSet *pRegister;
	register int RegSel, RegBit;
				
	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( MES_INTC_NUM_OF_INT > IntNum );
    MES_ASSERT( MES_INTC_INTMODE_FIQ >= IntMode );

    pRegister = __g_pRegister;
    RegSel  = IntNum >> 5;      // 0 or 1
    RegBit  = IntNum & 0x1F;    // 0 ~ 31

	switch( IntMode )
	{
	case MES_INTC_INTMODE_IRQ:
 		    pRegister->INTMODE[RegSel] &= (~(U32)(1UL<<RegBit));
		break;

	case MES_INTC_INTMODE_FIQ:
		    pRegister->INTMODE[RegSel] |= ( (U32)(1UL<<RegBit));
		break;

	default:
		MES_ASSERT( CFALSE );
	}

}

//------------------------------------------------------------------------------
/**
 *  @brief	    Set a priority control.
 *  @param[in]  ArbiterNum	An arbiter number to be changed, 0 ~ 11 are valid.	                   
 *  @param[in]  OrderSel 	Determines the order of priority manually.                               
 *  @return	    None.
 *	@remark	    Arbiter0  : for interrupt  0 ~  5 \n
 *			    Arbiter1  : for interrupt  6 ~ 11 \n
 *			    Arbiter2  : for interrupt 12 ~ 17 \n
 *			    Arbiter3  : for interrupt 18 ~ 23 \n
 *			    Arbiter4  : for interrupt 24 ~ 29 \n
 *			    Arbiter5  : for interrupt 30 ~ 35 \n
 *			    Arbiter6  : for interrupt 36 ~ 41 \n
 *			    Arbiter7  : for interrupt 42 ~ 47 \n
 *			    Arbiter8  : for interrupt 48 ~ 53 \n
 *			    Arbiter9  : for interrupt 54 ~ 59 \n
 *			    Arbiter10 : for interrupt 60 ~ 63 \n
 *			    Arbiter11 : arbiter 0 ~ 10
 */
void	MES_INTC_SetPriorityMode( U32 ArbiterNum, U32 OrderSel )
{
    register struct MES_INTC_RegisterSet *pRegister;	
	register U32 reg;
    
   	MES_ASSERT( CNULL != __g_pRegister );
	MES_ASSERT( 12 > ArbiterNum );
	MES_ASSERT( ((4 > OrderSel) && (11 > ArbiterNum)) || ((8 > OrderSel) && (11 == ArbiterNum)) );

    pRegister = __g_pRegister;	

	reg = pRegister->PRIORDER;
	if( 11 > ArbiterNum )	reg &= ~(3UL<<(ArbiterNum<<1));
	else					reg &= ~(7UL<<(ArbiterNum<<1));
	reg |= (OrderSel<<(ArbiterNum<<1));
	pRegister->PRIORDER = reg; 
}


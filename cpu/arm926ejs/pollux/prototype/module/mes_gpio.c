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
//	Module     : GPIO
//	File       : mes_gpio.h
//	Description: 
//	Author     : Firmware Team
//	History    : 
//------------------------------------------------------------------------------

#include "mes_gpio.h"


/// @brief  GPIO Module's Register List
struct  MES_GPIO_RegisterSet
{
	volatile U32 GPIOxOUT;           ///< 0x00   : Output Register
	volatile U32 GPIOxOUTENB;        ///< 0x04   : Output Enable Register
	volatile U32 GPIOxDETMODE[2];    ///< 0x08   : Event Detect Mode Register 
	volatile U32 GPIOxINTENB;        ///< 0x10   : Interrupt Enable Register
	volatile U32 GPIOxDET;           ///< 0x14   : Event Detect Register
	volatile U32 GPIOxPAD;           ///< 0x18   : PAD Status Register
	volatile U32 GPIOxPUENB;         ///< 0x1C   : Pull Up Enable Register
	volatile U32 GPIOxALTFN[2];      ///< 0x20   : Alternate Function Select Register
};

U32 __g_MES_GPIO_VALID_BIT[NUMBER_OF_GPIO_MODULE] = { 0xFFFFFFFF, 0xFFFFFFFF, 0x001FFFFF }; 

static  struct
{
    struct MES_GPIO_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_GPIO_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_GPIO_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// inline Function
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief	    Set 1bit value
 * @param[in]   Value   This Value is changed when SetBit() executed.
 * @param[in]   Bit     Bit number                                   
 * @param[in]   Enable  \b CTRUE( Set ).\n
 *                      \b CFALSE( Clear )                
 * @return	    None.
 * @see         MES_GPIO_GetBit
 */
__inline void MES_GPIO_SetBit
( 
    volatile U32* Value,        
    U32 Bit,                    
    CBOOL Enable                
)
{
	register U32 newvalue;

    MES_ASSERT( CNULL != Value );
    MES_ASSERT( MES_GPIO_MAX_BIT > Bit );

    newvalue = *Value;

    newvalue &=     ~(1UL       << Bit );
    newvalue |=     (U32)Enable << Bit ;

	*Value = newvalue;
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get bit value
 * @param[in]   Value     Check this value of 1bit state
 * @param[in]   Bit       Bit number                    
 * @return      \b CTRUE indicate that bit is Seted.\n
 *              \b CFALSE indicate that bit is cleared.
 * @see         MES_GPIO_SetBit
 */
__inline CBOOL MES_GPIO_GetBit
( 
    U32 Value,                  
    U32 Bit                     
)
{
    MES_ASSERT( MES_GPIO_MAX_BIT > Bit );
    
	return (CBOOL)(( Value >> Bit ) & ( 1UL ) );
}	

//------------------------------------------------------------------------------
/**
 * @brief	    Set 2bit value
 * @param[in]   Value       This Value is changed when SetBit2() executed.
 * @param[in]   Bit         Bit number                                    
 * @param[in]   BitValue    Set value of bit                              
 * @return	    None.
 * @see         MES_GPIO_GetBit2
 */
__inline void MES_GPIO_SetBit2
( 
    volatile U32* Value,        
    U32 Bit,                    
    U32 BitValue                
)
{
	register U32 newvalue = *Value;

    MES_ASSERT( CNULL != Value );
    MES_ASSERT( MES_GPIO_MAX_BIT > Bit );

	newvalue = (U32)(newvalue & ~(3UL<<(Bit*2)));
	newvalue = (U32)(newvalue | (BitValue<<(Bit*2)));
	*Value = newvalue;
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get 2bit value
 * @param[in]   Value   Check this value of 2bit state          
 * @param[in]   Bit     Bit number                              
 * @return	    2bit value
 * @see         MES_GPIO_SetBit2
 */
 
__inline U32 MES_GPIO_GetBit2
( 
    U32 Value,          
    U32 Bit             
)
{
    MES_ASSERT( MES_GPIO_MAX_BIT > Bit );    
    
	return (U32)((U32)(Value>>(Bit*2)) & 3UL);
}

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE  indicate that Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see                                MES_GPIO_SelectModule, 
 *          MES_GPIO_GetCurrentModule,  MES_GPIO_GetNumberOfModule
 */
CBOOL   MES_GPIO_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_GPIO_MODULE; i++ )
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
 *  @see        MES_GPIO_Initialize,        
 *              MES_GPIO_GetCurrentModule,  MES_GPIO_GetNumberOfModule
 */
U32    MES_GPIO_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_GPIO_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;    

	return oldindex;        
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_GPIO_Initialize,        MES_GPIO_SelectModule, 
 *                                          MES_GPIO_GetNumberOfModule
 */
U32     MES_GPIO_GetCurrentModule( void )
{
    return __g_CurModuleIndex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_GPIO_Initialize,        MES_GPIO_SelectModule, 
 *              MES_GPIO_GetCurrentModule  
 */
U32     MES_GPIO_GetNumberOfModule( void )
{
    return NUMBER_OF_GPIO_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                        MES_GPIO_GetSizeOfRegisterSet,
 *              MES_GPIO_SetBaseAddress,        MES_GPIO_GetBaseAddress,
 *              MES_GPIO_OpenModule,            MES_GPIO_CloseModule,
 *              MES_GPIO_CheckBusy,             MES_GPIO_CanPowerDown
 */
U32     MES_GPIO_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_GPIO_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_GPIO_MODULE + (OFFSET_OF_GPIO_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_GPIO_GetPhysicalAddress,    
 *              MES_GPIO_SetBaseAddress,        MES_GPIO_GetBaseAddress,
 *              MES_GPIO_OpenModule,            MES_GPIO_CloseModule,
 *              MES_GPIO_CheckBusy,             MES_GPIO_CanPowerDown
 */
U32     MES_GPIO_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_GPIO_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_GPIO_GetPhysicalAddress,    MES_GPIO_GetSizeOfRegisterSet,
 *                                              MES_GPIO_GetBaseAddress,
 *              MES_GPIO_OpenModule,            MES_GPIO_CloseModule,
 *              MES_GPIO_CheckBusy,             MES_GPIO_CanPowerDown
 */
void    MES_GPIO_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_GPIO_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_GPIO_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_GPIO_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_GPIO_GetPhysicalAddress,    MES_GPIO_GetSizeOfRegisterSet,
 *              MES_GPIO_SetBaseAddress,        
 *              MES_GPIO_OpenModule,            MES_GPIO_CloseModule,
 *              MES_GPIO_CheckBusy,             MES_GPIO_CanPowerDown
 */
U32     MES_GPIO_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_GPIO_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_GPIO_GetPhysicalAddress,    MES_GPIO_GetSizeOfRegisterSet,
 *              MES_GPIO_SetBaseAddress,        MES_GPIO_GetBaseAddress,
 *                                              MES_GPIO_CloseModule,
 *              MES_GPIO_CheckBusy,             MES_GPIO_CanPowerDown
 */
CBOOL   MES_GPIO_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE  indicate that Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_GPIO_GetPhysicalAddress,    MES_GPIO_GetSizeOfRegisterSet,
 *              MES_GPIO_SetBaseAddress,        MES_GPIO_GetBaseAddress,
 *              MES_GPIO_OpenModule,            
 *              MES_GPIO_CheckBusy,             MES_GPIO_CanPowerDown
 */
CBOOL   MES_GPIO_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_GPIO_GetPhysicalAddress,    MES_GPIO_GetSizeOfRegisterSet,
 *              MES_GPIO_SetBaseAddress,        MES_GPIO_GetBaseAddress,
 *              MES_GPIO_OpenModule,            MES_GPIO_CloseModule,
 *                                              MES_GPIO_CanPowerDown
 */
CBOOL   MES_GPIO_CheckBusy( void )
{
	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_GPIO_GetPhysicalAddress,    MES_GPIO_GetSizeOfRegisterSet,
 *              MES_GPIO_SetBaseAddress,        MES_GPIO_GetBaseAddress,
 *              MES_GPIO_OpenModule,            MES_GPIO_CloseModule,
 *              MES_GPIO_CheckBusy             
 */
CBOOL   MES_GPIO_CanPowerDown( void )
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
 *  @see                                            MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
S32     MES_GPIO_GetInterruptNumber( void )
{
    return  INTNUM_OF_GPIO_MODULE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a specified interrupt to be enable or disable.
 *  @param[in]  IntNum  Interrupt Number .
 *  @param[in]  Enable  \b CTRUE  indicate that Interrupt Enable. \n
 *                      \b CFALSE indicate that Interrupt Disable.
 *  @return     None.
 *  @see        MES_GPIO_GetInterruptNumber,        
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
void    MES_GPIO_SetInterruptEnable( S32 IntNum, CBOOL Enable )
{
    register    struct MES_GPIO_RegisterSet *pRegister;
    register    U32 ReadValue ;
    
    MES_ASSERT( MES_GPIO_NUM_OF_INT > IntNum );
    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );

    pRegister = __g_pRegister;
    
    ReadValue = pRegister->GPIOxINTENB;

    ReadValue &= ~( (U32)1        << IntNum);
    ReadValue |=  ( (U32)Enable   << IntNum );

    pRegister->GPIOxINTENB = ReadValue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether a specified interrupt is enabled or disabled.
 *  @param[in]  IntNum  Interrupt Number.
 *  @return     \b CTRUE  indicate that Interrupt is enabled. \n
 *              \b CFALSe indicate that Interrupt is disabled.
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *                                                  MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
CBOOL   MES_GPIO_GetInterruptEnable( S32 IntNum )
{
    MES_ASSERT( MES_GPIO_NUM_OF_INT > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
   
    if( __g_pRegister->GPIOxINTENB & (1UL << IntNum ) )
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
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
CBOOL   MES_GPIO_GetInterruptPending( S32 IntNum )
{
    MES_ASSERT( MES_GPIO_NUM_OF_INT > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
    
    if( __g_pRegister->GPIOxDET & (1UL << IntNum ) )
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
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *                                                  MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
void    MES_GPIO_ClearInterruptPending( S32 IntNum )
{
    MES_ASSERT( MES_GPIO_NUM_OF_INT > IntNum );
    MES_ASSERT( CNULL != __g_pRegister );
    
    __g_pRegister->GPIOxDET = 1UL << IntNum;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set all interrupts to be enables or disables.
 *  @param[in]  Enable  \b CTRUE  indicate that Set to all interrupt enable. \n
 *                      \b CFALSE indicate that Set to all interrupt disable.
 *  @return     None.
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
void    MES_GPIO_SetInterruptEnableAll( CBOOL Enable )
{
    register     U32    setvalue;

    MES_ASSERT( (0==Enable) || (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );
    
    if( Enable )
    {   setvalue = ~(0UL);  }
    else
    {   setvalue = 0UL;  }

    __g_pRegister->GPIOxINTENB  = setvalue;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether some of interrupts are enable or not.
 *  @return     \b CTRUE  indicate that At least one( or more ) interrupt is enabled. \n
 *              \b CFALSE indicate that All interrupt is disabled.
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *                                                  MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
CBOOL   MES_GPIO_GetInterruptEnableAll( void )
{
    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->GPIOxINTENB )   
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
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     
 *              MES_GPIO_ClearInterruptPendingAll,  MES_GPIO_GetInterruptPendingNumber
 */
CBOOL   MES_GPIO_GetInterruptPendingAll( void )
{
    MES_ASSERT( CNULL != __g_pRegister );

    if( __g_pRegister->GPIOxDET )  
    {
        return CTRUE;    
    }
    return CFALSE;            
}

//------------------------------------------------------------------------------
/**
 *  @brief      Clear pending state of all interrupts.
 *  @return     None.
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *                                                  MES_GPIO_GetInterruptPendingNumber
 */
void    MES_GPIO_ClearInterruptPendingAll( void )
{
    MES_ASSERT( CNULL != __g_pRegister );
    
    __g_pRegister->GPIOxDET = (U32)(0xFFFFFFFF);
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a interrupt number which has the most prority of pended interrupts
 *  @return     Pending Number( If all pending is not set then return -1 ).
 *  @see        MES_GPIO_GetInterruptNumber,        MES_GPIO_SetInterruptEnable,
 *              MES_GPIO_GetInterruptEnable,        MES_GPIO_GetInterruptPending,
 *              MES_GPIO_ClearInterruptPending,     MES_GPIO_SetInterruptEnableAll,
 *              MES_GPIO_GetInterruptEnableAll,     MES_GPIO_GetInterruptPendingAll,
 *              MES_GPIO_ClearInterruptPendingAll  
 */
S32     MES_GPIO_GetInterruptPendingNumber( void )  // -1 if None
{
    register    U32 intnum;
    register    U32 intpend;

    MES_ASSERT( CNULL != __g_pRegister );
    
    intpend = __g_pRegister->GPIOxDET;
    
	for( intnum=0 ; intnum<32 ; intnum++ )
	{
		if( 0 != (intpend & (1UL<<intnum)) )
		{
		    return intnum;
		}
	}	
    
    return -1;    
}

//------------------------------------------------------------------------------
// GPIO Operation.
//------------------------------------------------------------------------------

/**
 * @brief	    Set interrupt mode
 * @param[in]   BitNumber   Bit number                
 * @param[in]   IntMode     GPIO interrupt detect mode
 * @return      None.
 * @see                                         MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
void	            MES_GPIO_SetInterruptMode( U32 BitNumber, MES_GPIO_INTMODE IntMode )
{
    register U32    bitNumber;
    
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_INTMODE_RISINGEDGE >= IntMode );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );

    bitNumber = BitNumber;

	MES_GPIO_SetBit2( &__g_pRegister->GPIOxDETMODE[bitNumber/16], bitNumber%16, (U32)IntMode );    
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get interrupt mode
 * @param[in]   BitNumber    Bit number
 * @return      GPIO interrupt detect mode
 * @see         MES_GPIO_SetInterruptMode,      
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
MES_GPIO_INTMODE	MES_GPIO_GetInterruptMode( U32 BitNumber )
{
    register    U32 bitNumber;
    
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );

    bitNumber   = BitNumber;    
    
    return (MES_GPIO_INTMODE)MES_GPIO_GetBit2( __g_pRegister->GPIOxDETMODE[bitNumber/16], bitNumber%16 );
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get gpio output enable
 * @param[in]   BitNumber    Bit number                                 
 * @param[in]   OutputEnb    \b CTRUE indicate that Output Mode. \n
 *                           \b CFALSE indicate that Input Mode.
 * @return      None.
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *                                              MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
void    MES_GPIO_SetOutputEnable ( U32 BitNumber, CBOOL OutputEnb )
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );
    MES_ASSERT( (0==OutputEnb) || (1==OutputEnb) );

	MES_GPIO_SetBit( &__g_pRegister->GPIOxOUTENB, BitNumber, OutputEnb );	    
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get gpio output enable
 * @param[in]   BitNumber    Bit number
 * @return      \b CTRUE indicate that GPIO's current Setting is Output Mode.\n
 *              \b CFALSE indicate that GPIO's current Setting is Input Mode.
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
CBOOL    MES_GPIO_GetOutputEnable ( U32 BitNumber )
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );
    
    return MES_GPIO_GetBit( __g_pRegister->GPIOxOUTENB, BitNumber );	
}

//------------------------------------------------------------------------------
/**
 * @brief	    Set gpio output value
 * @param[in]   BitNumber   Bit number                               
 * @param[in]   Value       \b CTRUE indicate that High Level. \n 
 *                          \b CFALSE indicate that Low Level.
 * @return      None.
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *                                              MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
void	MES_GPIO_SetOutputValue  ( U32 BitNumber, CBOOL Value )
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );
    MES_ASSERT( (0==Value) || (1==Value) );

	MES_GPIO_SetBit( &__g_pRegister->GPIOxOUT, BitNumber, Value );    
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get gpio output value
 * @param[in]   BitNumber    Bit number
 * @return      \b CTRUE indicate that GPIO's output value is High Level.\n
 *              \b CFALSE indicate that GPIO's output value is Low Level.
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
CBOOL	MES_GPIO_GetOutputValue  ( U32 BitNumber )
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );

	return MES_GPIO_GetBit( __g_pRegister->GPIOxOUT, BitNumber );    
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get gpio input value
 * @param[in]   BitNumber    Bit number
 * @return      \b CTRUE indicate that GPIO's input value is High Level.\n
 *              \b CFALSE indicate that GPIO's input value is Low Level. 
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *                                              MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit 
 */
CBOOL	MES_GPIO_GetInputValue 	 ( U32 BitNumber )
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );

	return MES_GPIO_GetBit( __g_pRegister->GPIOxPAD, BitNumber );	    
}

//------------------------------------------------------------------------------
/**
 * @brief	    Set Pull up of GPIO Pin
 * @param[in]   BitNumber   Bit number                           
 * @param[in]   enable      \b CTRUE indicate that Pull Up. \n
 *                          \b CFALSE indicate that NOT Pull Up.
 * @return	    None.
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
void 	MES_GPIO_SetPullUpEnable ( U32 BitNumber, CBOOL enable)
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );
    MES_ASSERT( (0==enable) || (1==enable) );

    MES_GPIO_SetBit( &__g_pRegister->GPIOxPUENB, BitNumber, enable );		
}

//------------------------------------------------------------------------------
/**
 * @brief       Get pull up value of GPIO Pin
 * @param[in]   BitNumber    Bit number
 * @return      GPIO pull up value
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *                                              MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
CBOOL  MES_GPIO_GetPullUpEnable ( U32 BitNumber )
{
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );
    
	return MES_GPIO_GetBit( __g_pRegister->GPIOxPUENB, BitNumber );	    
}


//--------------------------------------------------------------------------
// Pin Configuration
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief	    Set PAD Fuction
 * @param[in]   BitNumber    Bit number  
 * @param[in]   padfunc      Pad Function
 * @return	    None
 * @remarks     Each gpio pins can use to GPIO Pin or Alternate Function 0 or Alternate Function 1 or \n
 *              Alternate Function 2. So This function Sets gpio pin's function.   
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       
 *              MES_GPIO_GetPadFunction,        MES_GPIO_GetValidBit
 */
void    MES_GPIO_SetPadFunction( U32 BitNumber, MES_GPIO_PADFUNC padfunc )
{
    register    U32 bitNumber;
    
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );
    MES_ASSERT( MES_GPIO_PADFUNC_2 >= padfunc );

    bitNumber      = BitNumber;                
    
	MES_GPIO_SetBit2( &__g_pRegister->GPIOxALTFN[bitNumber/16], bitNumber%16, (U32)padfunc );    
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get PAD Fuction
 * @param[in]   BitNumber    Bit number
 * @return	    None
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *                                              MES_GPIO_GetValidBit
 */
MES_GPIO_PADFUNC    MES_GPIO_GetPadFunction( U32 BitNumber )
{
    register    U32 bitNumber;
        
    MES_ASSERT( CNULL != __g_pRegister );
    MES_ASSERT( MES_GPIO_MAX_BIT > BitNumber );

    bitNumber      = BitNumber;                
    
    return (MES_GPIO_PADFUNC)MES_GPIO_GetBit2( __g_pRegister->GPIOxALTFN[bitNumber/16], bitNumber%16 );
}

//------------------------------------------------------------------------------
/**
 * @brief	    Get GPIO's valid bit
 * @return	    GPIO's valid bit
 * @remarks     Each bit's indicate corresponding GPIO pin. If retun value is 0x0FF, then
 *              This indicate that GPIO have eight pins ( GPIO0 ~ GPIO7 ).                
 * @see         MES_GPIO_SetInterruptMode,      MES_GPIO_GetInterruptMode,
 *              MES_GPIO_SetOutputEnable,       MES_GPIO_GetOutputEnable,
 *              MES_GPIO_SetOutputValue,        MES_GPIO_GetOutputValue,
 *              MES_GPIO_GetInputValue,         MES_GPIO_SetPullUpEnable,
 *              MES_GPIO_GetPullUpEnable,       MES_GPIO_SetPadFunction,
 *              MES_GPIO_GetPadFunction        
 */
U32     MES_GPIO_GetValidBit( void )
{
    MES_ASSERT( NUMBER_OF_GPIO_MODULE > __g_CurModuleIndex );

    return __g_MES_GPIO_VALID_BIT[__g_CurModuleIndex];
}



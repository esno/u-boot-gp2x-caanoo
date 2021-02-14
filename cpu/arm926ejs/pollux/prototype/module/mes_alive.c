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
//	Module     : ALIVE
//	File       : mes_alive.c
//	Description: 
//	Author     : Firmware Team
//	History    : 
//------------------------------------------------------------------------------

#include "mes_alive.h"

/// @brief  ALIVE Module's Register List
struct  MES_ALIVE_RegisterSet
{
    volatile U32 PWRGATE;           ///< 0x00 : Alive Power Gating Register.
    volatile U32 GPIORST;           ///< 0x04 : Alive GPIO Reset Register.
    volatile U32 GPIOSET;           ///< 0x08 : Alive GPIO Set Register.
    volatile U32 GPIOREAD;          ///< 0x0C : Alive GPIO Read Register.
    volatile U32 SCRATCHRST;        ///< 0x10 : Alive Scratch Reset Register.
    volatile U32 SCRATCHSET;        ///< 0x14 : Alive Scratch Set Register.
    volatile U32 SCRATCHREAD;       ///< 0x18 : Alive Scratch Read Register.
};

static  struct
{
    struct MES_ALIVE_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_ALIVE_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_ALIVE_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE indicate that  Initialize is successed.\n
 *          \b CFALSE indicate that Initialize is failed.\n
 *  @see                                  MES_ALIVE_SelectModule,
 *          MES_ALIVE_GetCurrentModule,   MES_ALIVE_GetNumberOfModule
 */
CBOOL   MES_ALIVE_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_ALIVE_MODULE; i++ )
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
 *  @see        MES_ALIVE_Initialize,         
 *              MES_ALIVE_GetCurrentModule,   MES_ALIVE_GetNumberOfModule
 */
U32    MES_ALIVE_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_ALIVE_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;    

	return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see        MES_ALIVE_Initialize,         MES_ALIVE_SelectModule,
 *                                            MES_ALIVE_GetNumberOfModule
 */
U32     MES_ALIVE_GetCurrentModule( void )
{
    return __g_CurModuleIndex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @return     Module's number.
 *  @see        MES_ALIVE_Initialize,         MES_ALIVE_SelectModule,
 *              MES_ALIVE_GetCurrentModule   
 */
U32     MES_ALIVE_GetNumberOfModule( void )
{
    return NUMBER_OF_ALIVE_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                          MES_ALIVE_GetSizeOfRegisterSet,
 *              MES_ALIVE_SetBaseAddress,         MES_ALIVE_GetBaseAddress,
 *              MES_ALIVE_OpenModule,             MES_ALIVE_CloseModule,
 *              MES_ALIVE_CheckBusy,              MES_ALIVE_CanPowerDown
 */             
U32     MES_ALIVE_GetPhysicalAddress( void )
{               
    MES_ASSERT( NUMBER_OF_ALIVE_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_ALIVE_MODULE + (OFFSET_OF_ALIVE_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_ALIVE_GetPhysicalAddress,     
 *              MES_ALIVE_SetBaseAddress,         MES_ALIVE_GetBaseAddress,
 *              MES_ALIVE_OpenModule,             MES_ALIVE_CloseModule,
 *              MES_ALIVE_CheckBusy,              MES_ALIVE_CanPowerDown
 */
U32     MES_ALIVE_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_ALIVE_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_ALIVE_GetPhysicalAddress,     MES_ALIVE_GetSizeOfRegisterSet,
 *                                                MES_ALIVE_GetBaseAddress,
 *              MES_ALIVE_OpenModule,             MES_ALIVE_CloseModule,
 *              MES_ALIVE_CheckBusy,              MES_ALIVE_CanPowerDown
 */
void    MES_ALIVE_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_ALIVE_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_ALIVE_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_ALIVE_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_ALIVE_GetPhysicalAddress,     MES_ALIVE_GetSizeOfRegisterSet,
 *              MES_ALIVE_SetBaseAddress,         
 *              MES_ALIVE_OpenModule,             MES_ALIVE_CloseModule,
 *              MES_ALIVE_CheckBusy,              MES_ALIVE_CanPowerDown
 */
U32     MES_ALIVE_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_ALIVE_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE  indicate that Initialize is successed. \n
 *              \b CFALSE indicate that Initialize is failed.
 *  @see        MES_ALIVE_GetPhysicalAddress,     MES_ALIVE_GetSizeOfRegisterSet,
 *              MES_ALIVE_SetBaseAddress,         MES_ALIVE_GetBaseAddress,
 *                                                MES_ALIVE_CloseModule,
 *              MES_ALIVE_CheckBusy,              MES_ALIVE_CanPowerDown
 */
CBOOL   MES_ALIVE_OpenModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE indicate that  Deinitialize is successed. \n
 *              \b CFALSE indicate that Deinitialize is failed.
 *  @see        MES_ALIVE_GetPhysicalAddress,     MES_ALIVE_GetSizeOfRegisterSet,
 *              MES_ALIVE_SetBaseAddress,         MES_ALIVE_GetBaseAddress,
 *              MES_ALIVE_OpenModule,             
 *              MES_ALIVE_CheckBusy,              MES_ALIVE_CanPowerDown
 */
CBOOL   MES_ALIVE_CloseModule( void )
{
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE  indicate that Module is Busy. \n
 *              \b CFALSE indicate that Module is NOT Busy.
 *  @see        MES_ALIVE_GetPhysicalAddress,     MES_ALIVE_GetSizeOfRegisterSet,
 *              MES_ALIVE_SetBaseAddress,         MES_ALIVE_GetBaseAddress,
 *              MES_ALIVE_OpenModule,             MES_ALIVE_CloseModule,
 *                                                MES_ALIVE_CanPowerDown
 */
CBOOL   MES_ALIVE_CheckBusy( void )
{
	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE  indicate that Ready to enter power-down stage. \n
 *              \b CFALSE indicate that This module can't enter to power-down stage.
 *  @see        MES_ALIVE_GetPhysicalAddress,     MES_ALIVE_GetSizeOfRegisterSet,
 *              MES_ALIVE_SetBaseAddress,         MES_ALIVE_GetBaseAddress,
 *              MES_ALIVE_OpenModule,             MES_ALIVE_CloseModule,
 *              MES_ALIVE_CheckBusy,              
 */
CBOOL   MES_ALIVE_CanPowerDown( void )
{
    return CTRUE;
}

//------------------------------------------------------------------------------
// Alive Operation.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * @brief       Set Alive GPIO's condition ( Writable or Not )
 * @param[in]   Enable  \b CTRUE indicate that writing data to Alive GPIO is Enable. \n
 *                      \b CFALSE indicate that writing data to Alive GPIO is Disable.
 * @return      None.
 * @remarks     User should set \b MES_ALIVE_SetWriteEnable( CTRUE ) before Setting Alive GPIO.\n
 * @code
 *
 *        MES_ALIVE_SetWriteEnable( CTRUE );      // Can Alive GPIO Setting
 *        ...
 *        MES_ALIVE_SetAliveGpio( 0, CTRUE );     // Alive GPIO 0 pin set to High value
 *        MES_ALIVE_SetAliveGpio( 0, CFALSE );    // Alive GPIO 0 pin set to Low value.
 *        ...
 *        MES_ALIVE_SetWriteEnable( CFALSE );     // Now can't setting Alive GPIO pin value.
 *        ...              
 *        MES_ALIVE_SetAliveGpio( 0, CTRUE );     // Alive Gpio 0 pin still have LOW value. 
 *
 * @endcode
 * @see                                         MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
void    MES_ALIVE_SetWriteEnable( CBOOL Enable )
{
    MES_ASSERT( (0==Enable) | (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );
    
    __g_pRegister->PWRGATE  =  ( (U32)Enable & 0x01 );
}

//------------------------------------------------------------------------------
/**
 * @brief       Get status of Alive GPIO is writable or Not.
 * @return      \b CTRUE indicate that Writing data to Alive GPIO is enabled.\n
 *              \b CFALSE indicate that Writing data to Alive GPIO is disabled.
 * @see         MES_ALIVE_SetWriteEnable,       
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
CBOOL   MES_ALIVE_GetWriteEnable( void )
{
    const U32 NPOWERGATING_MASK = ( 1UL << 0 );
    
    MES_ASSERT( CNULL != __g_pRegister );
    
    return  (CBOOL)( __g_pRegister->PWRGATE & NPOWERGATING_MASK );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set VDD Power's on/off
 * @param[in]   Enable  \b CTRUE indicate that VDD Power \b ON.\n
 *                      \b CFALSE indicate that VDD Power \b OFF.
 * @return      None.
 * @remarks     User should set \b MES_ALIVE_SetWriteEnable( CTRUE ) before Setting MES_ALIVE_SetVDDPower(). \n
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *                                              MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
void    MES_ALIVE_SetVDDPower( CBOOL Enable )
{
    const U32 VDDPWRON_MASK = 1UL << 7;

    MES_ASSERT( (0==Enable) | (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );    

    __g_pRegister->GPIOSET = 0;
    __g_pRegister->GPIORST = 0;

    if( Enable )        __g_pRegister->GPIOSET = VDDPWRON_MASK;
    else                __g_pRegister->GPIORST = VDDPWRON_MASK;

    __g_pRegister->GPIOSET = 0;
    __g_pRegister->GPIORST = 0;

}

//------------------------------------------------------------------------------
/**
 * @brief       Get VDD Power's status
 * @return      Enable  \b CTRUE indicate that VDD Power is on.\n
 *                      \b CFALSE indicate that VDD Power is off.
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
CBOOL   MES_ALIVE_GetVDDPower( void )
{
    const U32 VDDPWRON_POS = 7;
    const U32 VDDPWRON_MASK = 1UL << VDDPWRON_POS;
    
    MES_ASSERT( CNULL != __g_pRegister );        
    
    return (CBOOL)((__g_pRegister->GPIOREAD & VDDPWRON_MASK ) >> VDDPWRON_POS ); 
}

//------------------------------------------------------------------------------
/**
 * @brief       Set Alive Gpio's Output value.
 * @param[in]   GpioNum Alive GPIO Number( 0 ~ 6 ).
 * @param[in]   Enable  CTRUE indicate that Alive GPIO's output value is High value. 
 *                      CFALSE indicate that Alive GPIO's output value is Low value.
 * @return      None.
 * @remarks     User should set \b MES_ALIVE_SetWriteEnable( CTRUE ) before Setting MES_ALIVE_SetAliveGpio(). \n
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *                                              MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
void    MES_ALIVE_SetAliveGpio( U32 GpioNum, CBOOL Enable )
{

    register U32 GPIO_MASK;

    MES_ASSERT( 7 > GpioNum );
    MES_ASSERT( (0==Enable) | (1==Enable) );
    MES_ASSERT( CNULL != __g_pRegister );    

    GPIO_MASK   =   ( 1UL << GpioNum );

    __g_pRegister->GPIOSET = 0;
    __g_pRegister->GPIORST = 0;

    if( Enable )        __g_pRegister->GPIOSET = GPIO_MASK; // enableÀÌ TRUE¸é set
    else                __g_pRegister->GPIORST = GPIO_MASK; //          FALSE¸é clear 

    __g_pRegister->GPIOSET = 0;
    __g_pRegister->GPIORST = 0;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get Alive Gpio's Output status.
 * @param[in]   GpioNum Alive GPIO Number( 0 ~ 6 ).
 * @return      \b CTRUE indicate that Alive GPIO's output value is High value. \n
 *              \b CFALSE indicate that Alive GPIO's output value is Low value.
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
CBOOL   MES_ALIVE_GetAliveGpio( U32 GpioNum )
{
    MES_ASSERT( 7 > GpioNum );
    MES_ASSERT( CNULL != __g_pRegister );    

    return (CBOOL) ((__g_pRegister->GPIOREAD >> GpioNum ) & 0x01 );
}

//------------------------------------------------------------------------------
/**
 * @brief       Set scratch register
 * @param[in]   Data    Data
 * @return      None.
 * @remarks     User should set \b MES_ALIVE_SetWriteEnable( CTRUE ) before Setting MES_ALIVE_SetScratchReg().\n
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *                                              MES_ALIVE_GetScratchReg,
 *              MES_ALIVE_GetVddPowerToggle 
 */
void    MES_ALIVE_SetScratchReg( U32 Data )
{
    MES_ASSERT( CNULL != __g_pRegister );    
    
    __g_pRegister->SCRATCHSET = Data;
    __g_pRegister->SCRATCHRST = ~Data;    
    __g_pRegister->SCRATCHSET = 0x0;
    __g_pRegister->SCRATCHRST = 0x0;    

}

//------------------------------------------------------------------------------
/**
 * @brief       Get data of scratch register.
 * @return      Data of scratch register.
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        
 *              MES_ALIVE_GetVddPowerToggle 
 */
U32     MES_ALIVE_GetScratchReg( void )
{
    MES_ASSERT( CNULL != __g_pRegister );        
    
    return (U32)__g_pRegister->SCRATCHREAD;
}

//------------------------------------------------------------------------------
/**
 * @brief       Get VDD power toggle status.
 * @return      \b CTRUE indicate that VDDPWRTOGGLE PAD is toggled. \n
 *              \b CFALSE indicate that VDDPWRTOGGLE PAD is \b NOT toggled. 
 * @see         MES_ALIVE_SetWriteEnable,       MES_ALIVE_GetWriteEnable,
 *              MES_ALIVE_SetVDDPower,          MES_ALIVE_GetVDDPower,
 *              MES_ALIVE_SetAliveGpio,         MES_ALIVE_GetAliveGpio,
 *              MES_ALIVE_SetScratchReg,        MES_ALIVE_GetScratchReg
 */
CBOOL   MES_ALIVE_GetVddPowerToggle( void )
{
    const U32 VDDPWRTOGGLE_POS = 8;
    const U32 VDDPWRTOGGLE_MASK = 1UL <<  VDDPWRTOGGLE_POS;
    
    MES_ASSERT( CNULL != __g_pRegister );            
    
    return (CBOOL) ((__g_pRegister->GPIOREAD & VDDPWRTOGGLE_MASK ) >> VDDPWRTOGGLE_POS );    
}




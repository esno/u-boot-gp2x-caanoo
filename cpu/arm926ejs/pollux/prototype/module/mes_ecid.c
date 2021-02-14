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
//	Module     : ECID
//	File       : mes_ecid.h
//	Description: 
//	Author     : Firmware Team
//	History    : 
//------------------------------------------------------------------------------

#include "mes_ecid.h"

/// @brief  ECID Module's Register List
struct  MES_ECID_RegisterSet
{
    volatile U32    IDREG[4];               ///< 0x00 ~ 0x0C : ID Register
    volatile U8     CHIPNAME[48];           ///< 0x10 ~ 0x3C : Chip Name Register
    volatile U32    _Reserved;              ///< 0x40        : Reserved region
    volatile U32    GUID[4];                ///< 0x44 ~ 0x50 : GUID Register
    volatile U32    EC;                     ///< 0x54        : Control Register
};

static  struct
{
    struct MES_ECID_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_ECID_MODULE] = { CNULL, };

static  U32 __g_CurModuleIndex = 0;
static  struct MES_ECID_RegisterSet *__g_pRegister = CNULL;

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *  @brief  Initialize of prototype enviroment & local variables.
 *  @return \b CTRUE indicate that   Initialize is successed.\n
 *          \b CFALSE indicate that  Initialize is failed.\n
 *  @see                                MES_ECID_SelectModule,
 *          MES_ECID_GetCurrentModule,   MES_ECID_GetNumberOfModule
 */
CBOOL   MES_ECID_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;
	
	if( CFALSE == bInit )
	{
		__g_CurModuleIndex = 0;
		
		for( i=0; i < NUMBER_OF_ECID_MODULE; i++ )
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
 *  @return     the index of previously selected module.
 *  @see        MES_ECID_Initialize,         
 *              MES_ECID_GetCurrentModule,   MES_ECID_GetNumberOfModule
 */
U32    MES_ECID_SelectModule( U32 ModuleIndex )
{
	U32 oldindex = __g_CurModuleIndex;

    MES_ASSERT( NUMBER_OF_ECID_MODULE > ModuleIndex );

    __g_CurModuleIndex = ModuleIndex;
    __g_pRegister = __g_ModuleVariables[ModuleIndex].pRegister;    
    
    return oldindex;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get index of current selected module.
 *  @return     Current module's index.
 *  @see    MES_ECID_Initialize,         MES_ECID_SelectModule,
 *                                      MES_ECID_GetNumberOfModule
 */
U32     MES_ECID_GetCurrentModule( void )
{
    return __g_CurModuleIndex;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get number of modules in the chip.
 *  @see        MES_ECID_Initialize,         MES_ECID_SelectModule,
 *              MES_ECID_GetCurrentModule   
 */
U32     MES_ECID_GetNumberOfModule( void )
{
    return NUMBER_OF_ECID_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get module's physical address.
 *  @return     Module's physical address
 *  @see                                        MES_ECID_GetSizeOfRegisterSet,
 *              MES_ECID_SetBaseAddress,         MES_ECID_GetBaseAddress,
 *              MES_ECID_OpenModule,             MES_ECID_CloseModule,
 *              MES_ECID_CheckBusy,              MES_ECID_CanPowerDown
 */
U32     MES_ECID_GetPhysicalAddress( void )
{
    MES_ASSERT( NUMBER_OF_ECID_MODULE > __g_CurModuleIndex );

    return  (U32)( PHY_BASEADDR_ECID_MODULE + (OFFSET_OF_ECID_MODULE * __g_CurModuleIndex) );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a size, in byte, of register set.
 *  @return     Size of module's register set.
 *  @see        MES_ECID_GetPhysicalAddress,     
 *              MES_ECID_SetBaseAddress,         MES_ECID_GetBaseAddress,
 *              MES_ECID_OpenModule,             MES_ECID_CloseModule,
 *              MES_ECID_CheckBusy,              MES_ECID_CanPowerDown
 */
U32     MES_ECID_GetSizeOfRegisterSet( void )
{
    return sizeof( struct MES_ECID_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *  @brief      Set a base address of register set.
 *  @param[in]  BaseAddress Module's base address
 *  @return     None.
 *  @see        MES_ECID_GetPhysicalAddress,     MES_ECID_GetSizeOfRegisterSet,
 *                                              MES_ECID_GetBaseAddress,
 *              MES_ECID_OpenModule,             MES_ECID_CloseModule,
 *              MES_ECID_CheckBusy,              MES_ECID_CanPowerDown
 */
void    MES_ECID_SetBaseAddress( U32 BaseAddress )
{
    MES_ASSERT( CNULL != BaseAddress );
    MES_ASSERT( NUMBER_OF_ECID_MODULE > __g_CurModuleIndex );

    __g_ModuleVariables[__g_CurModuleIndex].pRegister = (struct MES_ECID_RegisterSet *)BaseAddress;
    __g_pRegister = (struct MES_ECID_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get a base address of register set
 *  @return     Module's base address.
 *  @see        MES_ECID_GetPhysicalAddress,     MES_ECID_GetSizeOfRegisterSet,
 *              MES_ECID_SetBaseAddress,         
 *              MES_ECID_OpenModule,             MES_ECID_CloseModule,
 *              MES_ECID_CheckBusy,              MES_ECID_CanPowerDown
 */
U32     MES_ECID_GetBaseAddress( void )
{
    MES_ASSERT( NUMBER_OF_ECID_MODULE > __g_CurModuleIndex );

    return (U32)__g_ModuleVariables[__g_CurModuleIndex].pRegister;    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Initialize selected modules with default value.
 *  @return     \b CTRUE indicate that   Initialize is successed. \n
 *              \b CFALSE indicate that  Initialize is failed.
 *  @see        MES_ECID_GetPhysicalAddress,     MES_ECID_GetSizeOfRegisterSet,
 *              MES_ECID_SetBaseAddress,         MES_ECID_GetBaseAddress,
 *                                              MES_ECID_CloseModule,
 *              MES_ECID_CheckBusy,              MES_ECID_CanPowerDown
 */
CBOOL   MES_ECID_OpenModule( void )
{
	  return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Deinitialize selected module to the proper stage.
 *  @return     \b CTRUE indicate that   Deinitialize is successed. \n
 *              \b CFALSE indicate that  Deinitialize is failed.
 *  @see        MES_ECID_GetPhysicalAddress,     MES_ECID_GetSizeOfRegisterSet,
 *              MES_ECID_SetBaseAddress,         MES_ECID_GetBaseAddress,
 *              MES_ECID_OpenModule,             
 *              MES_ECID_CheckBusy,              MES_ECID_CanPowerDown
 */
CBOOL   MES_ECID_CloseModule( void )
{
    return CTRUE;   
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicates whether the selected modules is busy or not.
 *  @return     \b CTRUE indicate that   Module is Busy. \n
 *              \b CFALSE indicate that  Module is NOT Busy.
 *  @see        MES_ECID_GetPhysicalAddress,     MES_ECID_GetSizeOfRegisterSet,
 *              MES_ECID_SetBaseAddress,         MES_ECID_GetBaseAddress,
 *              MES_ECID_OpenModule,             MES_ECID_CloseModule,
 *                                              MES_ECID_CanPowerDown
 */
CBOOL   MES_ECID_CheckBusy( void )
{
    return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *  @brief      Indicaes whether the selected modules is ready to enter power-down stage
 *  @return     \b CTRUE indicate that   Ready to enter power-down stage. \n
 *              \b CFALSE indicate that  This module can't enter to power-down stage.
 *  @see        MES_ECID_GetPhysicalAddress,     MES_ECID_GetSizeOfRegisterSet,
 *              MES_ECID_SetBaseAddress,         MES_ECID_GetBaseAddress,
 *              MES_ECID_OpenModule,             MES_ECID_CloseModule,
 *              MES_ECID_CheckBusy              
 */
CBOOL   MES_ECID_CanPowerDown( void )
{
    if( MES_ECID_CheckBusy() )
    {
        return CFALSE;   
    }
    
    return CTRUE;
}

//------------------------------------------------------------------------------
// ECID Operation.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  @brief      Get ECID
 *  @param[out] pID Buffer to save ECID.
 *  @return     None.
 *  @remark     \e pID indicate U32 type array that size is 4. \n
 *              If buffer is small, then buffer can overflow.\n
 *  @code
 *              U32 ECID[4];
 *              ...
 *              MES_ECID_GetECID( ECID );
 *              ...
 *  @endcode
 *  @see                                MES_ECID_GetChipName, 
 *              MES_ECID_GetGUID,       MES_ECID_GetBondingID
 */
void    MES_ECID_GetECID( U32 *pID )
{
    const U32 FENB_MASK = 1 << 6;
    const U32 SET_MASK  = 1 << 5;
    const U32 SEN_MASK  = 1 << 4;

    MES_ASSERT( CNULL != pID );
    MES_ASSERT( CNULL != __g_pRegister );

    __g_pRegister->EC   = 0;
    __g_pRegister->EC   = FENB_MASK | SET_MASK;
    __g_pRegister->EC   = FENB_MASK;
    __g_pRegister->EC   = FENB_MASK | SEN_MASK;    
    __g_pRegister->EC   = FENB_MASK;    
    __g_pRegister->EC   = 0;

    *pID++ = __g_pRegister->IDREG[0];
    *pID++ = __g_pRegister->IDREG[1];
    *pID++ = __g_pRegister->IDREG[2];
    *pID++ = __g_pRegister->IDREG[3];
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get Chip Name
 *  @param[out] pName   Buffer to save Chip Name
 *  @return     None.
 *  @remark     \e pName indicate \b char type array that size is 49. \n
 *              If buffer is small, then buffer can overflow.
 *  @code
 *              char ChipName[49];
 *              ...
 *              MES_ECID_GetChipName( ChipName );
 *              ...
 *  @endcode
 *  @see        MES_ECID_GetECID,       
 *              MES_ECID_GetGUID,       MES_ECID_GetBondingID
 */
void    MES_ECID_GetChipName( char *pName )
{
    U32 i, j=0;

    MES_ASSERT( CNULL != pName );
    MES_ASSERT( CNULL != __g_pRegister );    

    for( i=0; i<48; i++)
    {
        *pName++ = __g_pRegister->CHIPNAME[j++];   
    }
    
    *pName = 0; // null
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get Group ID
 *  @param[out] pGUID   Buffer to save Group ID
 *  @return     None.
 *  @remark     \e pGUID indicate U32 type array that size is 4. \n
 *              If buffer is small, then buffer can overflow.
 *  @code
 *              U32 GUID[4];
 *              ...
 *              MES_ECID_GetGUID( GUID );
 *              ...
 *  @endcode
 *  @see        MES_ECID_GetECID,       MES_ECID_GetChipName, 
 *                                      MES_ECID_GetBondingID
 */
void    MES_ECID_GetGUID( U32 *pGUID )
{
    MES_ASSERT( CNULL != pGUID );
    MES_ASSERT( CNULL != __g_pRegister );

    *pGUID++ = __g_pRegister->GUID[0];
    *pGUID++ = __g_pRegister->GUID[1];
    *pGUID++ = __g_pRegister->GUID[2];
    *pGUID++ = __g_pRegister->GUID[3];    
}

//------------------------------------------------------------------------------
/**
 *  @brief      Get Bonding ID
 *  @return     Bonding ID.
 *  @see        MES_ECID_GetECID,       MES_ECID_GetChipName, 
 *              MES_ECID_GetGUID       
 */
U32     MES_ECID_GetBondingID( void )
{
    const U32 BONDINGID_MASK = 0x03;

    MES_ASSERT( CNULL != __g_pRegister );    

    return (__g_pRegister->EC & BONDINGID_MASK);
}



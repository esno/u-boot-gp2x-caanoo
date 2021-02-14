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
//	Author     : 
//	History    : 
//------------------------------------------------------------------------------
#ifndef __MES_ECID_H__
#define __MES_ECID_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   ECID ECID
//------------------------------------------------------------------------------
//@{
 
//------------------------------------------------------------------------------
/// @name   Module Interface
//@{
CBOOL   MES_ECID_Initialize( void );
U32     MES_ECID_SelectModule( U32 ModuleIndex );
U32     MES_ECID_GetCurrentModule( void );
U32     MES_ECID_GetNumberOfModule( void );
//@} 

//------------------------------------------------------------------------------
///  @name   Basic Interface
//@{
U32     MES_ECID_GetPhysicalAddress( void );
U32     MES_ECID_GetSizeOfRegisterSet( void );
void    MES_ECID_SetBaseAddress( U32 BaseAddress );
U32     MES_ECID_GetBaseAddress( void );
CBOOL   MES_ECID_OpenModule( void );
CBOOL   MES_ECID_CloseModule( void );
CBOOL   MES_ECID_CheckBusy( void );
CBOOL   MES_ECID_CanPowerDown( void );
//@} 

//------------------------------------------------------------------------------
///  @name  ECID Operation.
//@{
void    MES_ECID_GetECID( U32 *pID );
void    MES_ECID_GetChipName( char *pName );
void    MES_ECID_GetGUID( U32 *pGUID );
U32     MES_ECID_GetBondingID( void );
//@} 

//@} 

#ifdef  __cplusplus
}
#endif


#endif // __MES_ECID_H__

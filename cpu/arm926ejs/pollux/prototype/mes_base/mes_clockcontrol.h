//------------------------------------------------------------------------------
//
//	Copyright (C) 2007 MagicEyes Digital Co., Ltd All Rights Reserved
//	MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND	WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : mes_base
//	File       : mes_clockcontrol.h
//	Description: support clock control type.
//	Author     : Goofy
//	Export     : 
//	History    : 
//		2007.04.04	Goofy first draft.
//------------------------------------------------------------------------------
#ifndef __MES_CLOCKCONTROL_H__
#define __MES_CLOCKCONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
///	@name	Clock Control types
//------------------------------------------------------------------------------
//@{

///	@brief	type for PCLK control mode
typedef enum 
{
	MES_PCLKMODE_DYNAMIC = 0UL,	    ///< PCLK is provided only when CPU has access to registers of this module.
	MES_PCLKMODE_ALWAYS  = 1UL  	///< PCLK is always provided for this module.
} MES_PCLKMODE ;

///	@brief type for BCLK control mode
typedef enum 
{
	MES_BCLKMODE_DISABLE  = 0UL,	///< BCLK is disabled.
	MES_BCLKMODE_DYNAMIC  = 2UL,	///< BCLK is provided only when this module requests it.
	MES_BCLKMODE_ALWAYS   = 3UL		///< BCLK is always provided for this module.
} MES_BCLKMODE ;

//@}

#ifdef __cplusplus
}
#endif

#endif // __MES_CLOCKCONTROL_H__	


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
//	File       : mes_chip.h
//	Description: support chip information
//	Author     : Goofy
//	Export     : 
//	History    : 
//		2007.04.04	Goofy first draft.
//------------------------------------------------------------------------------
#ifndef __MES_CHIP_H__
#define __MES_CHIP_H__

#ifndef	MES_CHIP_ID
#define MES_CHIP_ID		MES_CHIP_LF1000
#endif

#if (MES_CHIP_ID == MES_CHIP_LF1000)
	#include "mes_chip_lf1000.h"
#else
	#error "No MES_CHIP_ID found. You have to define MES_CHIP_ID properly."
#endif

#endif // __MES_CHIP_H__


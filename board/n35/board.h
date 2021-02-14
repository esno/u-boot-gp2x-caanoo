//------------------------------------------------------------------------------
//
//	Copyright (C) 2005 MagicEyes Digital Co., Ltd All Rights Reserved
//	MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE 
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT 
//  NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR 
//  A PARTICULAR PURPOSE.
//
//	Author     : Neo Kang (neo@mesdigital.com)
//	History    :
//	   2005/08/02 Neo first implementation
//------------------------------------------------------------------------------
#ifndef _BOARD_H
#define _BOARD_H

#ifdef __cplusplus
	extern "C" {
#endif

void BOARD_Initialize(void);
int fw_check(void);
extern void boot_read_verify(void);

#ifdef __cplusplus
};
#endif

#endif // _BOARD_H

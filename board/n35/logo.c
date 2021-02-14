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
//	Author     :
//	History    :
//
//------------------------------------------------------------------------------

#include <mes_pollux.h>
#include <system.h>

#include "board.h"
#include "mes_type.h"

unsigned short Make16Pixel(unsigned char R, unsigned char G, unsigned char B)
{
	return (unsigned short)( ((R>>3)<<11) | ((G>>2)<<5) | (B>>3) );
}

void OEM_DrawLogo (U32 dwBaseAddr, U32 dwXMax, U32 dwYMax, U32 dwStride)
{
	unsigned char *FrameBase = dwBaseAddr;
	unsigned char *DrawBase = dwBaseAddr;

	U32 offset = 0;
	int y=0, x=0, region=0;

	int ScreenWidth  = dwXMax;
	int ScreenHeight = dwYMax;

	int division = (ScreenWidth/5);

	for(y=0; y<ScreenHeight; y++)
	{
		for(x=0; x<ScreenWidth; x++)
		{
			if( DISPLAY_MLC_BYTE_PER_PIXEL == 2)
			{
				region = x/division;
				switch (region)
				{
				case 0:
					*(unsigned short *)FrameBase = 
					(unsigned short)Make16Pixel(0xFF, 0xFF, 0xFF);	
					break;
				case 1:
					*(unsigned short *)FrameBase = 
					(unsigned short)Make16Pixel(0xFF, 0x00, 0x00);	
					break;
				case 2:
					*(unsigned short *)FrameBase = 
					(unsigned short)Make16Pixel(0x00, 0xFF, 0x00);	
					break;
				case 3:
					*(unsigned short *)FrameBase = 
					(unsigned short)Make16Pixel(0x00, 0x00, 0xFF);	
					break;
				case 4:
					*(unsigned short *)FrameBase = 
					(unsigned short)Make16Pixel(0x00, 0x00, 0x00);	
					break;
				}
				FrameBase += 2;
			}
			else if(DISPLAY_MLC_BYTE_PER_PIXEL == 3)
			{
				region = x/division;
				switch (region)
				{
				case 0:
					*(unsigned char *)FrameBase++ = 0xff;
					*(unsigned char *)FrameBase++ = 0xff;
					*(unsigned char *)FrameBase++ = 0xff;	break;
				case 1:
					*(unsigned char *)FrameBase++ = 0xff;
					*(unsigned char *)FrameBase++ = 0x00;
					*(unsigned char *)FrameBase++ = 0x00;	break;
				case 2:
					*(unsigned char *)FrameBase++ = 0x00;
					*(unsigned char *)FrameBase++ = 0xff;
					*(unsigned char *)FrameBase++ = 0x00;	break;
				case 3:
					*(unsigned char *)FrameBase++ = 0x00;
					*(unsigned char *)FrameBase++ = 0x00;
					*(unsigned char *)FrameBase++ = 0xff;	break;
				case 4:
					*(unsigned char *)FrameBase++ = 0x00;
					*(unsigned char *)FrameBase++ = 0x00;
					*(unsigned char *)FrameBase++ = 0x00;	break;
				}
			}
			
		}

		DrawBase += dwStride;
		FrameBase = DrawBase;
	}
}


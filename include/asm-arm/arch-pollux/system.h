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
//	Author     : hans (neo@mesdigital.com)
//	History    :
//	   2007/09/17 hans first implementation
//------------------------------------------------------------------------------
#ifndef _OEM_SYSTEM_H_
#define _OEM_SYSTEM_H_

#define ENABLE	CTRUE
#define DISABLE	CFALSE

#include <mes_type.h>
#include <mes_pollux.h>
#include <system_main.h>

//------------------------------------------------------------------------------
// Frequency
//------------------------------------------------------------------------------
//	PLL0,1=(m * Fin)/(p * 2^s)
//	PLL0	56<=MDIV<=1023,		1<=PDIV<=63,	SDIV = 0, 1, 2, 3
//	PLL1	13<=MDIV<=255,		1<=PDIV<=63,	SDIV = 0, 1, 2, 3
#define PLLFREQ( FIN, PDIV, MDIV, SDIV )	\
				((__int64)((__int64)(MDIV) * (__int64)(FIN))/((PDIV) * (1UL<<SDIV)))

/// @brief	PLL Input Frequency
enum { PLLFIN =  27000000 };
/// @brief	PLL0 clock speed (Hz)
enum { PLL0 = PLLFREQ( PLLFIN, SYSTEM_FREQUENCY_PLL0_P, SYSTEM_FREQUENCY_PLL0_M, SYSTEM_FREQUENCY_PLL0_S ) };
/// @brief	PLL1 clock speed (Hz)
enum { PLL1 = PLLFREQ( PLLFIN, SYSTEM_FREQUENCY_PLL1_P, SYSTEM_FREQUENCY_PLL1_M, SYSTEM_FREQUENCY_PLL1_S ) };
enum {
	SEL_PLL0	= 0,
	SEL_PLL1	= 1,
};
/// @brief  CPU clock speed (Hz)
enum {
	CPUSELPLL	= SYSTEM_CLOCK_CPU_SELPLL,
	CPUDIV		= SYSTEM_CLOCK_CPU_DIV,
	CPUAHBDIV	= SYSTEM_CLOCK_CPU_AHBDIV,
	CPUSRCPLL	= (CPUSELPLL==SEL_PLL0) ? PLL0 : PLL1,
	CPUCLK		= CPUSRCPLL/CPUDIV,
	CPUAHBCLK	= CPUCLK/CPUAHBDIV
};
/// @brief  Internal Bus clock speed (Hz)
/// BCLK: Max 133Mhz, PCLK: Max 66.5Mhz
enum {
	BCLKSELPLL	= SYSTEM_CLOCK_BCLK_SELPLL,
	BCLKDIV		= SYSTEM_CLOCK_BCLK_DIV,
	BCLKSRCPLL	= (BCLKSELPLL==SEL_PLL0) ? PLL0 : PLL1,
	BCLK		= BCLKSRCPLL/BCLKDIV,
	PCLK		= BCLK/2
};

/// @brief	timer clock speed (Hz)
enum {
	TIMERSELPLL	= SYSTEM_CLOCK_TIMER_SELPLL,
	TIMERDIV	= SYSTEM_CLOCK_TIMER_DIV,
	TIMERSRCPLL	= (TIMERSELPLL==SEL_PLL0) ? PLL0 : PLL1,
	TIMERSRCCLK	= TIMERSRCPLL/TIMERDIV
};
/// @brief	pwm clock speed (Hz)
enum {
	PWMSELPLL	= SYSTEM_CLOCK_PWM_SELPLL,
	PWMDIV	= SYSTEM_CLOCK_PWM_DIV,
	PWMSRCPLL	= (PWMSELPLL==SEL_PLL0) ? PLL0 : PLL1,
	PWMSRCCLK	= PWMSRCPLL/PWMDIV
};
/// @brief  USB Block
enum {
	USBSELPLL	= SYSTEM_CLOCK_USB_SELPLL,
	USBDIV		= SYSTEM_CLOCK_USB_DIV,
	USBSRCPLL	= (USBSELPLL==SEL_PLL0) ? PLL0 : PLL1,
	USBCLK		= USBSRCPLL/USBDIV
};
/// @brief	UART clock speed (Hz)
enum {
	UARTSELPLL	= SYSTEM_CLOCK_UART_SELPLL,
	UARTDIV		= SYSTEM_CLOCK_UART_DIV,
	UARTSRCPLL	= (UARTSELPLL==SEL_PLL0) ? PLL0 : PLL1,
	UARTSRCCLK	= UARTSRCPLL/UARTDIV
};

enum {
	AUDIOSELPLL	= SYSTEM_CLOCK_AUDIO_SELPLL,
	AUDIOSRCPLL	= (AUDIOSELPLL==SEL_PLL0) ? PLL0 : PLL1
};


/// @brief	SDMMC clock speed (Hz)
enum {
	SDMMCSELPLL	= SYSTEM_CLOCK_SDMMC_SELPLL,
	SDMMCDIV	= SYSTEM_CLOCK_SDMMC_DIV,
	SDMMCSRCPLL	= PLL1 / 2,
	SDMMCCLK	= 66250000/SDMMCDIV
};

enum {
	STATICWAIT_DISABLE    	= 1,
	STATICWAIT_ACTIVEHIGH 	= 2,
	STATICWAIT_ACTIVELOW  	= 3
};

enum {
	STATICBST_DISABLE		= 0,
	STATICBST_4BYTE			= 1,
	STATICBST_8BYTE			= 2,
	STATICBST_16BYTE		= 3
};

//------------------------------------------------------------------------------
// PAD Configuration
//------------------------------------------------------------------------------
enum {	nSCS2, nSCS3, nSCS4, nSCS5,
		nPCS0, nPCS1, nICS0, nICS1 };

//------------------------------------------------------------------------------
// Display Device Configuration
//------------------------------------------------------------------------------

//
//	Select Display mode
//

enum {
	DPC_VCLK_SOURCE_PLL0	= 0,
	DPC_VCLK_SOURCE_PLL1	= 1,
	DPC_VCLK_SOURCE_PLL2	= 2,
	DPC_VCLK_SOURCE_PSVCLK	= 3,
	DPC_VCLK_SOURCE_nPSVCLK	= 4,
	DPC_VCLK_SOURCE_XTI		= 5,
	DPC_VCLK_SOURCE_AVCLK	= 6,
	DPC_VCLK_SOURCE_VCLK2	= 7
};

enum {
	DPC_PADVCLK_VCLK		= 0,
	DPC_PADVCLK_VCLK2		= 1,
};

enum {
	DPC_FORMAT_RGB555		= 0,
	DPC_FORMAT_RGB565		= 1,
	DPC_FORMAT_RGB666		= 2,
	DPC_FORMAT_RGB888		= 3,
	DPC_FORMAT_MRGB555A		= 4,
	DPC_FORMAT_MRGB555B		= 5,
	DPC_FORMAT_MRGB565		= 6,
	DPC_FORMAT_MRGB666		= 7,
	DPC_FORMAT_MRGB888A		= 8,
	DPC_FORMAT_MRGB888B		= 9,
	DPC_FORMAT_CCIR656		= 10,
	DPC_FORMAT_CCIR601A		= 12,
	DPC_FORMAT_CCIR601B		= 13,
    DPC_FORMAT_4096COLOR    = 1,  ///< 4096 Color Format, STN LCD
	DPC_FORMAT_16GRAY       = 3,   ///< 16 Level Gray Format, STN LCD
};

enum {
	DPC_ORDER_CbYCrY		= 0,
	DPC_ORDER_CrYCbY		= 1,
	DPC_ORDER_YCbYCr		= 2,
	DPC_ORDER_YCrYCb		= 3
};

enum {
	DPC_VBS_NTSC_M			= 0UL,	///< NTSC-M             59.94 Hz(525)
	DPC_VBS_NTSC_N			= 1UL,	///< NTSC-N             50 Hz(625)
	DPC_VBS_NTSC_443		= 2UL,	///< NTSC-4.43          60 Hz(525)
	DPC_VBS_PAL_M			= 3UL,	///< PAL-M              59.952 Hz(525)
	DPC_VBS_PAL_N			= 4UL,	///< PAL-combination N  50 Hz(625)
	DPC_VBS_PAL_BGHI		= 5UL,	///< PAL-B,D,G,H,I,N    50 Hz(625)
	DPC_VBS_PSEUDO_PAL		= 6UL,	///< Pseudo PAL         60 Hz(525)
	DPC_VBS_PSEUDO_NTSC 	= 7UL	///< Pseudo NTSC        50 Hz (625)
};

enum LCDTYPE
{
	DPC_LCDTYPE_TFT	= 0,
	DPC_LCDTYPE_STN	= 1
};

enum 
{
   	DPC_BANDWIDTH_LOW		= 0UL,	///< Low bandwidth
   	DPC_BANDWIDTH_MEDIUM	= 1UL,	///< Medium bandwidth
   	DPC_BANDWIDTH_HIGH		= 2UL	///< High bandwidth
};

enum RGBFMT
{
    MLC_RGBFMT_R5G6B5		= 0x44320000UL,   ///< 16bpp { R5, G6, B5 }.
    MLC_RGBFMT_B5G6R5		= 0xC4320000UL,   ///< 16bpp { B5, G6, R5 }.

    MLC_RGBFMT_X1R5G5B5		= 0x43420000UL,   ///< 16bpp { X1, R5, G5, B5 }.
    MLC_RGBFMT_X1B5G5R5		= 0xC3420000UL,   ///< 16bpp { X1, B5, G5, R5 }.
    MLC_RGBFMT_X4R4G4B4		= 0x42110000UL,   ///< 16bpp { X4, R4, G4, B4 }.
    MLC_RGBFMT_X4B4G4R4		= 0xC2110000UL,   ///< 16bpp { X4, B4, G4, R4 }.
    MLC_RGBFMT_X8R3G3B2		= 0x41200000UL,   ///< 16bpp { X8, R3, G3, B2 }.
    MLC_RGBFMT_X8B3G3R2		= 0xC1200000UL,   ///< 16bpp { X8, B3, G3, R2 }.

    MLC_RGBFMT_A1R5G5B5		= 0x33420000UL,   ///< 16bpp { A1, R5, G5, B5 }.
    MLC_RGBFMT_A1B5G5R5		= 0xB3420000UL,   ///< 16bpp { A1, B5, G5, R5 }.
    MLC_RGBFMT_A4R4G4B4		= 0x22110000UL,   ///< 16bpp { A4, R4, G4, B4 }.
    MLC_RGBFMT_A4B4G4R4		= 0xA2110000UL,   ///< 16bpp { A4, B4, G4, R4 }.
    MLC_RGBFMT_A8R3G3B2		= 0x11200000UL,   ///< 16bpp { A8, R3, G3, B2 }.
    MLC_RGBFMT_A8B3G3R2		= 0x91200000UL,   ///< 16bpp { A8, B3, G3, R2 }.

    MLC_RGBFMT_G8R8_G8B8	= 0x4ED30000UL,   ///< 16bpp { G8, R8, G8, B8 }.
    MLC_RGBFMT_R8G8_B8G8	= 0x4F840000UL,   ///< 16bpp { R8, G8, B8, G8 }.
    MLC_RGBFMT_G8B8_G8R8	= 0xCED30000UL,   ///< 16bpp { G8, B8, G8, R8 }.
    MLC_RGBFMT_B8G8_R8G8	= 0xCF840000UL,   ///< 16bpp { B8, G8, R8, G8 }.

    MLC_RGBFMT_X8L8			= 0x40030000UL,   ///< 16bpp { X8, L8 }.
    MLC_RGBFMT_A8L8			= 0x10030000UL,   ///< 16bpp { A8, L8 }.
    MLC_RGBFMT_L16			= 0x45540000UL,   ///< 16bpp { L16 }.

    MLC_RGBFMT_R8G8B8		= 0x46530000UL,   ///< 24bpp { R8, G8, B8 }.
    MLC_RGBFMT_B8G8R8		= 0xC6530000UL,   ///< 24bpp { B8, G8, R8 }.

    MLC_RGBFMT_X8R8G8B8		= 0x46530000UL,   ///< 32bpp { X8, R8, G8, B8 }.
    MLC_RGBFMT_X8B8G8R8		= 0xC6530000UL,   ///< 32bpp { X8, B8, G8, R8 }.
    MLC_RGBFMT_A8R8G8B8		= 0x06530000UL,   ///< 32bpp { A8, R8, G8, B8 }.
    MLC_RGBFMT_A8B8G8R8		= 0x86530000UL,   ///< 32bpp { A8, B8, G8, R8 }.
    MLC_RGBFMT_FORCEU32		= 0x7FFFFFFFUL
};

//
//	MLC Layer Priority
//
enum {
	MLC_VID_PRIORITY_FIRST		= 0UL<<8,	// video  > Cursor > Window > 3D layer
	MLC_VID_PRIORITY_SECOND		= 1UL<<8,	// Cursor > video  > Window > 3D layer
	MLC_VID_PRIORITY_THIRD		= 2UL<<8,	// Cursor > Window > video  > 3D layer
};

#define OEM_MPEG2D3D_STRIDE		4096
#define OEM_MEM_DISPLAY_ZONE	0x20000000
#define BANK_SHIFT				20	// Fixed

enum {
	HALF_CONTROL = 1,
	FULL_CONTROL = 2
};


#define	DISP_PRI_MAIN_SCREEN 	0
#define	DISP_SEC_MAIN_SCREEN 	1
#define	DISP_ALL_MAIN_SCREEN 	2

//
//PWM
//

enum POL
{
	POL_INVERTED    = 0,   ///< Polarity: Inverse
	POL_BYPASS      = 1,   ///< Polarity: Bypass
	POL_INVALID     = 2,
	POL_FORCE32     = 0x7FFFFFFF        
};




#if 0
//------------------------------------------------------------------------------
// Wakeup Source
//------------------------------------------------------------------------------
#define OEM_USE_WAKEUP_GPIOALV0			NULL
#define OEM_USE_WAKEUP_GPIOALV3			NULL
#define OEM_USE_WAKEUP_GPIOALV4			NULL
#define OEM_USE_WAKEUP_GPIOALV5			NULL
#define OEM_USE_WAKEUP_GPIOALV6			NULL
#define OEM_USE_WAKEUP_EINT0			NULL
#define OEM_USE_WAKEUP_EINT1			NULL

//------------------------------------------------------------------------------
// Power Button
//------------------------------------------------------------------------------
#define OEM_USE_MOMENTARY_POWER_ON		CFALSE
#define OEM_PWR_SET_PWROFF_UNIT_100MS	40
#define OEM_PWR_SET_STOP_UNIT_100MS		1
#define OEM_USE_SUSPEND_MODE			CFALSE

#define OEM_USE_PWRMONITOR_ADC			CFALSE
#define OEM_PWR_MONITOR_ADC_CH_NUM		-1
#endif

enum PAD_STATUS {
	PAD_GPIOIN				= 1<<0,
	PAD_GPIOIN_PULLUP		= 1<<1,
	PAD_GPIOOUT				= 1<<2,
	PAD_GPIOOUT_PULLUP		= 1<<3,
	PAD_ALT1				= 1<<4,
	PAD_ALT1_PULLUP			= 1<<5,
	PAD_ALT2				= 1<<6,
	PAD_ALT2_PULLUP			= 1<<7,
	PAD_NOTUSED 			= 1<<8,
	PAD_NOTEXIST			= 1<<9
};

enum MES_GPIO
{
	MES_GPIO_A	= 0,
	MES_GPIO_B	= 1,
	MES_GPIO_C	= 2,
	MES_GPIO_ALV= 4,
	MES_GPIO_NOTDEFINED= -1
};

enum GPIO_DETECT{
	LOW_LEVEL		= 0,
	HIGH_LEVEL		= 1,
	FALLING_EDGE	= 2,
	RISING_EDGE		= 3
};

enum GPIO_OUT_LEVEL{
	OUT_LOW		= 0,
	OUT_HIGH	= 1
};

enum GPIO_IN_LEVEL{
	IN_LOW		= 0,
	IN_HIGH		= 1
};

typedef struct _tagGPIOIN
{
	unsigned long	GPIOID;
	unsigned char	vk;
	unsigned char	Key_Active_Det;
	unsigned char	Key_Inactive_Det;
	unsigned char	Key_Active_Level;
	long			irq;
}MES_GPIO_IN, *PMES_GPIO_IN;

#include <system_gpio.h>

// maybe will be moved to driver source
#define OEM_BUT_DIRECT_GPIO { \
	{OEM_GPIO_KEY_VOLUP, VK_RIGHT, OEM_GPIO_KEY_VOLUP_ACTIVE, OEM_GPIO_KEY_VOLUP_INACTIVE, OEM_GPIO_KEY_VOLUP_AL, OEM_GPIO_KEY_VOLUP_IRQ}, \
	{OEM_GPIO_KEY_VOLDN, VK_LEFT, OEM_GPIO_KEY_VOLDN_ACTIVE, OEM_GPIO_KEY_VOLDN_INACTIVE, OEM_GPIO_KEY_VOLDN_AL, OEM_GPIO_KEY_VOLDN_IRQ}, \
	{OEM_GPIO_KEY_CHUP, VK_UP, OEM_GPIO_KEY_CHUP_ACTIVE, OEM_GPIO_KEY_CHUP_INACTIVE, OEM_GPIO_KEY_CHUP_AL, OEM_GPIO_KEY_CHUP_IRQ}, \
	{OEM_GPIO_KEY_CHDN, VK_DOWN, OEM_GPIO_KEY_CHDN_ACTIVE, OEM_GPIO_KEY_CHDN_INACTIVE, OEM_GPIO_KEY_CHDN_AL, OEM_GPIO_KEY_CHDN_IRQ}, \
	{OEM_GPIO_KEY_CHG, VK_TAB, OEM_GPIO_KEY_CHG_ACTIVE, OEM_GPIO_KEY_CHG_INACTIVE, OEM_GPIO_KEY_CHG_AL, OEM_GPIO_KEY_CHG_IRQ}, \
	{OEM_GPIO_KEY_MENU, VK_RETURN, OEM_GPIO_KEY_MENU_ACTIVE, OEM_GPIO_KEY_MENU_INACTIVE, OEM_GPIO_KEY_MENU_AL, OEM_GPIO_KEY_MENU_IRQ}, \
	{OEM_GPIO_KEY_POWER, VK_BACK, OEM_GPIO_KEY_POWER_ACTIVE, OEM_GPIO_KEY_POWER_INACTIVE, OEM_GPIO_KEY_POWER_AL, OEM_GPIO_KEY_POWER_IRQ} \
}


#include <system_mem.h>

#endif // _OEM_SYSTEM_H_

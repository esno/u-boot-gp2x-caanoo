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
//	Author     : hans (hans@mesdigital.com)
//	History    :
//	   2007/09/10 hans first implementation
//------------------------------------------------------------------------------

#ifndef _MES_POLLUX_H
#define _MES_POLLUX_H

// for linux build
typedef	long long	__int64;

//------------------------------------------------------------------------------
// Size of IO Register
//------------------------------------------------------------------------------
#define REG_SIZE_NAND			0x0020
//------------------------------------------------------------------------------
// Physical Base Address of POLLUX
//------------------------------------------------------------------------------
#define PHY_BASEADDR_INTC			0xC0000800
                                	
#define PHY_BASEADDR_MCUS			0xC0015800
#define PHY_BASEADDR_MCUDA			0xC0014800
                                	
#define PHY_BASEADDR_TIMER0			0xC0001800
#define PHY_BASEADDR_TIMER1			0xC0001880
#define PHY_BASEADDR_TIMER2			0xC0001900
#define PHY_BASEADDR_TIMER3			0xC0001980
#define PHY_BASEADDR_TIMER4			0xC0001A00

#define PHY_BASEADDR_SYSCTRL			0xC000F000

#define PHY_BASEADDR_NAND			0xAC000000

#define PHY_BASEADDR_GPIOA			0xC000A000
#define PHY_BASEADDR_GPIOB			0xC000A040
#define PHY_BASEADDR_GPIOC			0xC000A080

#define PHY_BASEADDR_GPIOALV			0xC0019000

#define PHY_BASEADDR_I2C0			0xC000E000
#define PHY_BASEADDR_I2C1			0xC000E800

#define PHY_BASEADDR_PWM			0xC000C000

#define PHY_BASEADDR_RTC			0xC000F080

#define PHY_BASEADDR_UART0			0xC0016000
#define PHY_BASEADDR_UART1			0xC0016080
#define PHY_BASEADDR_UART2			0xC0016800
#define PHY_BASEADDR_UART3			0xC0016880

#define PHY_BASEADDR_DMA			0xC0000000

#define PHY_BASEADDR_AUDIO			0xC000D800

#define PHY_BASEADDR_SD				0xC0009800
#define PHY_BASEADDR_SD1			0xC000C800

#define PHY_BASEADDR_UHC			0xC000D000

#define PHY_BASEADDR_UDC			0xC0018000

#define PHY_BASEADDR_SPI0			0xC0007800
#define PHY_BASEADDR_SPI1			0xC0008000
#define PHY_BASEADDR_SPI2			0xC0008800

#define PHY_BASEADDR_GRP3D			0xC001A000

#define PHY_BASEADDR_ADC			0xC0005000

#define PHY_BASEADDR_MLC_P			0xC0004000
#define PHY_BASEADDR_MLC_S			0xC0004400
#define PHY_BASEADDR_DPC_P			0xC0003000
#define PHY_BASEADDR_DPC_S			0xC0003400

//------------------------------------------------------------------------------
// Virtual Base Address of POLLUX
//------------------------------------------------------------------------------
//#define NORMALIO_OFFSET			0x10000000	// 0xc0000000 -> 0xB0000000
#define NORMALIO_OFFSET				0x00000000	// 0xc0000000 -> 0xC0000000
                                	
#define VIR_BASEADDR_INTC			(PHY_BASEADDR_INTC   - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_MCUS			(PHY_BASEADDR_MCUS   - NORMALIO_OFFSET)
#define VIR_BASEADDR_MCUDA			(PHY_BASEADDR_MCUDA   - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_TIMER0			(PHY_BASEADDR_TIMER0  - NORMALIO_OFFSET)
#define VIR_BASEADDR_TIMER1			(PHY_BASEADDR_TIMER1  - NORMALIO_OFFSET)
#define VIR_BASEADDR_TIMER2			(PHY_BASEADDR_TIMER2  - NORMALIO_OFFSET)
#define VIR_BASEADDR_TIMER3			(PHY_BASEADDR_TIMER3  - NORMALIO_OFFSET)
#define VIR_BASEADDR_TIMER4			(PHY_BASEADDR_TIMER4  - NORMALIO_OFFSET)

#define VIR_BASEADDR_SYSCTRL		(PHY_BASEADDR_SYSCTRL- NORMALIO_OFFSET)

#define VIR_BASEADDR_NAND			(PHY_BASEADDR_NAND	 - NORMALIO_OFFSET)

#define VIR_BASEADDR_GPIOA			(PHY_BASEADDR_GPIOA	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_GPIOB			(PHY_BASEADDR_GPIOB	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_GPIOC			(PHY_BASEADDR_GPIOC	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_GPIOALV		(PHY_BASEADDR_GPIOALV	 - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_I2C0			(PHY_BASEADDR_I2C0	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_I2C1			(PHY_BASEADDR_I2C1	 - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_PWM			(PHY_BASEADDR_PWM	 - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_RTC			(PHY_BASEADDR_RTC	 - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_UART0			(PHY_BASEADDR_UART0	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_UART1			(PHY_BASEADDR_UART1	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_UART2			(PHY_BASEADDR_UART2	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_UART3			(PHY_BASEADDR_UART3	 - NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_DMA			(PHY_BASEADDR_DMA	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_AUDIO			(PHY_BASEADDR_AUDIO	 - NORMALIO_OFFSET)

#define VIR_BASEADDR_SD				(PHY_BASEADDR_SD	 - NORMALIO_OFFSET)
#define VIR_BASEADDR_SD1			(PHY_BASEADDR_SD1	 - NORMALIO_OFFSET)

#define VIR_BASEADDR_UHC			(PHY_BASEADDR_UHC	- NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_UDC			(PHY_BASEADDR_UDC	- NORMALIO_OFFSET)
                                	
#define VIR_BASEADDR_SPI0			(PHY_BASEADDR_SPI0	- NORMALIO_OFFSET)
#define VIR_BASEADDR_SPI1			(PHY_BASEADDR_SPI1	- NORMALIO_OFFSET)
#define VIR_BASEADDR_SPI2			(PHY_BASEADDR_SPI2	- NORMALIO_OFFSET)

#define VIR_BASEADDR_GRP3D			(PHY_BASEADDR_GRP3D	- NORMALIO_OFFSET)

#define VIR_BASEADDR_ADC			(PHY_BASEADDR_ADC	- NORMALIO_OFFSET)

#define VIR_BASEADDR_MLC_P			(PHY_BASEADDR_MLC_P	- NORMALIO_OFFSET)
#define VIR_BASEADDR_MLC_S			(PHY_BASEADDR_MLC_S	- NORMALIO_OFFSET)
#define VIR_BASEADDR_DPC_P			(PHY_BASEADDR_DPC_P - NORMALIO_OFFSET)
#define VIR_BASEADDR_DPC_S			(PHY_BASEADDR_DPC_S - NORMALIO_OFFSET)

//------------------------------------------------------------------------------
// Interrupt Number
//------------------------------------------------------------------------------
#include "system_interrupt.h"

//------------------------------------------------------------------------------
// DMA Peripheral Index
//------------------------------------------------------------------------------
#define DMANUM_UART0TX			0
#define DMANUM_UART0RX			1
#define DMANUM_UART1TX			2
#define DMANUM_UART1RX			3
#define DMANUM_UART2TX			4
#define DMANUM_UART2RX			5
#define DMANUM_UART3TX			6
#define DMANUM_UART3RX			7
#define DMANUM_USBDEP1			12
#define DMANUM_USBDEP2			13
#define DMANUM_SD_RW			16
#define DMANUM_SPI_0_WRITE		18
#define DMANUM_SPI_0_READ		19
#define DMANUM_SPI_1_WRITE		20
#define DMANUM_SPI_1_READ		21
#define DMANUM_AUDIO_PLAY		24
#define DMANUM_AUDIO_RECORD		26
#define DMANUM_SD1_RW			30
#define DMANUM_MCUSTATIC0		31
#define DMANUM_I2C_CH0			33
#define DMANUM_I2C_CH1			34
#define DMANUM_RESERVED			0xffffffff

//------------------------------------------------------------------------------
// DMA Register Offset
//------------------------------------------------------------------------------
#define DMA_CHANNEL_OFFSET		0x80

//------------------------------------------------------------------------------
// GPIO
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// prototype header
//------------------------------------------------------------------------------
#include "mes_alive.h"
#include "mes_gpio.h"
#include "mes_clkpwr.h"
#include "mes_mcud.h"
#include "mes_mcus.h"
#include "mes_uart.h"
#include "mes_intc.h"
#include "mes_timer.h"
#include "mes_rtc.h"
#include "mes_dpc.h"
#include "mes_mlc.h"

#endif // _MES_MP2530F_H

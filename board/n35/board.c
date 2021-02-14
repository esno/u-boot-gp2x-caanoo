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
#include <common.h>
#include <mes_pollux.h>
#include <system.h>

#include "board.h"
#include "mes_type.h"

#ifdef CONFIG_WIZ_LOGO
    #ifndef CONFIG_LCD_NOINIT
        #include "logo_Wiz.h"
    #endif /* CONFIG_LCD_NOINIT */
    
    #include "img_firmware.h"
#endif /* CONFIG_WIZ_LOGO */


void MES_SetOutputValueBit ( U32 PinID, CBOOL bFlag );
void InitializeProtoType(void);
void ClockAndPower(void);
void InitializeGPIO(void);
void InitMemoryInterface(void);
void InitializeDisplay(void);
void InitializeLCD(void);
void InitializeDPC(void);
void InitializeMLC(void);
void InitializeSDHC(void);
void CountDelay(int count);

void OEM_DrawLogo ( U32 dwBaseAddr, U32 dwXMax, U32 dwYMax, U32 dwStride);

#define CS_LOW	( MES_SetOutputValueBit(OEM_GPIO_LCD_CS ,CFALSE))
#define CS_HIGH	( MES_SetOutputValueBit(OEM_GPIO_LCD_CS ,CTRUE))
#define SCL_LOW ( MES_SetOutputValueBit(OEM_GPIO_LCD_SCL ,CFALSE))
#define SCL_HIGH ( MES_SetOutputValueBit(OEM_GPIO_LCD_SCL ,CTRUE))
#define SDI_LOW ( MES_SetOutputValueBit(OEM_GPIO_LCD_SDI ,CFALSE))
#define SDI_HIGH ( MES_SetOutputValueBit(OEM_GPIO_LCD_SDI ,CTRUE))

int board_rev = 0;

void BOARD_Initialize(void)
{
    
    *(volatile unsigned short *)(VIR_BASEADDR_UDC + 0xC0) = 0;  /* usb device clock disable */
	
	//--------------------------------------------------------------------------
	// Initialize mes_prototype
	//--------------------------------------------------------------------------
	InitializeProtoType();
	InitMemoryInterface();	// don't change function call order below ClockAndPower;
	ClockAndPower();
	InitializeGPIO();

#ifndef CONFIG_LCD_NOINIT	

#ifdef CONFIG_WIZ_LOGO
{
    int x = 0;
	int y = 0;
    int img_x = (320-166)/2;
	int img_y = (240-72)/2;

    unsigned char *fbaddr = (unsigned char*)OEM_MEM_PHY_FRAMEBUFFER_START;
    unsigned char r,g,b;

    unsigned short *pSrc = wiz_logo;

    for(y=0; y<240; y++){
		for(x=0; x<320; x++) {
            if((img_x<x && x<=img_x+166) && (img_y<y && y<=img_y+72)) {   
                r = (*pSrc & 0xf800) >> 11;
                g = (*pSrc & 0x7E0) >> 5;
                b = (*pSrc & 0x1f);
                *fbaddr++ = b <<= 3;
                *fbaddr++ = g <<= 2;
                *fbaddr++ = r <<= 3;
                *pSrc ++;
			}
			else{
			    *fbaddr++ = 0;
			    *fbaddr++ = 0; 
			    *fbaddr++ = 0;  
		    }  
		}
	}
}
#endif /* CONFIG_WIZ_LOGO */

	InitializeDisplay();
#endif /* CONFIG_LCD_NOINIT */

}

void InitializeProtoType(void)
{
	U32 index=0;

	// alive gpio initialize
	MES_ALIVE_Initialize();
	MES_ALIVE_SelectModule( 0 );
	MES_ALIVE_SetBaseAddress( VIR_BASEADDR_GPIOALV);//MES_ALIVE_GetPhysicalAddress() );
	MES_ALIVE_OpenModule();

	// gpio initialize
	MES_GPIO_Initialize();
	for( index=0 ; index < MES_GPIO_GetNumberOfModule() ; index++ )
	{
		MES_GPIO_SelectModule( index );
		MES_GPIO_SetBaseAddress( VIR_BASEADDR_GPIOA + 0x40*index);//MES_GPIO_GetPhysicalAddress() );
		MES_GPIO_OpenModule();
	}

	MES_CLKPWR_Initialize();
	MES_CLKPWR_SelectModule( 0 );
	MES_CLKPWR_SetBaseAddress( VIR_BASEADDR_SYSCTRL);//MES_CLKPWR_GetPhysicalAddress() );
	MES_CLKPWR_OpenModule();

	MES_MCUD_Initialize();
	MES_MCUD_SelectModule( 0 );
	MES_MCUD_SetBaseAddress( VIR_BASEADDR_MCUDA);	//MES_MCUD_GetPhysicalAddress() );
	MES_MCUD_OpenModule();

	MES_MCUS_Initialize();
	MES_MCUS_SelectModule( 0 );
	MES_MCUS_SetBaseAddress( VIR_BASEADDR_MCUS);	//MES_MCUS_GetPhysicalAddress() );
	MES_MCUS_OpenModule();

	MES_INTC_Initialize();
	MES_INTC_SelectModule( 0 );
	MES_INTC_SetBaseAddress( VIR_BASEADDR_INTC);	//MES_INTC_GetPhysicalAddress() );
	MES_INTC_OpenModule();

	MES_TIMER_Initialize();
	MES_TIMER_SelectModule( 0 );
	MES_TIMER_SetBaseAddress( VIR_BASEADDR_TIMER0);	//MES_TIMER_GetPhysicalAddress() );
	MES_TIMER_OpenModule();

	MES_RTC_Initialize();
	MES_RTC_SelectModule( 0 );
    MES_RTC_SetBaseAddress( VIR_BASEADDR_RTC);//MES_RTC_GetPhysicalAddress() );
    MES_RTC_OpenModule();

    MES_DPC_Initialize();
    MES_DPC_SelectModule( 0 );
    MES_DPC_SetBaseAddress( VIR_BASEADDR_DPC_P );
	MES_DPC_OpenModule();

	MES_MLC_Initialize();
    MES_MLC_SelectModule( 0 );
    MES_MLC_SetBaseAddress( VIR_BASEADDR_MLC_P );
	MES_MLC_OpenModule();

    MES_PWM_Initialize();
	MES_PWM_SelectModule( 0);
	MES_PWM_SetBaseAddress(VIR_BASEADDR_PWM);
	MES_PWM_OpenModule();
	
	#if (DISPLAY_SEC_DISPLAY_ENABLE==CTRUE)
	{
    	MES_DPC_SelectModule( 1 );
    	MES_DPC_SetBaseAddress( VIR_BASEADDR_DPC_S );
		MES_DPC_OpenModule();

    	MES_MLC_SelectModule( 1 );
		MES_MLC_SetBaseAddress( VIR_BASEADDR_MLC_S );
		MES_MLC_OpenModule();
	}
	#endif

}

void ClockAndPower(void)
{
	MES_CLKPWR_SetClockCPU( CPUSELPLL, CPUDIV, CPUAHBDIV);

	// initialize system(Bus/Peri clock) clock source and divider
	MES_CLKPWR_SetClockBCLK( BCLKSELPLL, BCLKDIV);
	
	//--------------------------------------------------------------------------
	// PLL setting
	//--------------------------------------------------------------------------
	// initialize PLL0  
	MES_CLKPWR_SetPLLPMS( 0, SYSTEM_FREQUENCY_PLL0_P, SYSTEM_FREQUENCY_PLL0_M, SYSTEM_FREQUENCY_PLL0_S);
	
	// initialize PLL1
	MES_CLKPWR_SetPLLPowerOn( CTRUE );
	MES_CLKPWR_SetPLLPMS( 1, SYSTEM_FREQUENCY_PLL1_P, SYSTEM_FREQUENCY_PLL1_M, SYSTEM_FREQUENCY_PLL1_S);	

	// PLL change and wait PLL stable state
	MES_CLKPWR_DoPLLChange();              				// Change PLL
	while( CFALSE == MES_CLKPWR_IsPLLStable() );  		// wait until PLL is stable.

	// some delay
	{
		volatile U32 x;
		for( x=0 ; x<0x01FF ; x++ );
	}
}

void InitializeGPIO(void)
{
	int PortID=0;
	int BitNum=0;

	static const U32 IOMode[3][32] = {
		// GPIO group A
	{
		PAD_GPIOA0,  PAD_GPIOA1,  PAD_GPIOA2,  PAD_GPIOA3,  PAD_GPIOA4,  PAD_GPIOA5,  PAD_GPIOA6,  PAD_GPIOA7,  PAD_GPIOA8,  PAD_GPIOA9,
		PAD_GPIOA10, PAD_GPIOA11, PAD_GPIOA12, PAD_GPIOA13, PAD_GPIOA14, PAD_GPIOA15, PAD_GPIOA16, PAD_GPIOA17, PAD_GPIOA18, PAD_GPIOA19,
		PAD_GPIOA20, PAD_GPIOA21, PAD_GPIOA22, PAD_GPIOA23, PAD_GPIOA24, PAD_GPIOA25, PAD_GPIOA26, PAD_GPIOA27, PAD_GPIOA28, PAD_GPIOA29,
		PAD_GPIOA30, PAD_GPIOA31
	},
		// GPIO group B
	{
		PAD_GPIOB0,  PAD_GPIOB1,  PAD_GPIOB2,  PAD_GPIOB3,  PAD_GPIOB4,  PAD_GPIOB5,  PAD_GPIOB6,  PAD_GPIOB7,  PAD_GPIOB8,  PAD_GPIOB9,
		PAD_GPIOB10, PAD_GPIOB11, PAD_GPIOB12, PAD_GPIOB13, PAD_GPIOB14, PAD_GPIOB15, PAD_GPIOB16, PAD_GPIOB17, PAD_GPIOB18, PAD_GPIOB19,
		PAD_GPIOB20, PAD_GPIOB21, PAD_GPIOB22, PAD_GPIOB23, PAD_GPIOB24, PAD_GPIOB25, PAD_GPIOB26, PAD_GPIOB27, PAD_GPIOB28, PAD_GPIOB29,
		PAD_GPIOB30, PAD_GPIOB31
	},
		// GPIO group C
	{
		PAD_GPIOC0,  PAD_GPIOC1,  PAD_GPIOC2,  PAD_GPIOC3,  PAD_GPIOC4,  PAD_GPIOC5,  PAD_GPIOC6,  PAD_GPIOC7,  PAD_GPIOC8,  PAD_GPIOC9,
		PAD_GPIOC10, PAD_GPIOC11, PAD_GPIOC12, PAD_GPIOC13, PAD_GPIOC14, PAD_GPIOC15, PAD_GPIOC16, PAD_GPIOC17, PAD_GPIOC18, PAD_GPIOC19,
		PAD_GPIOC20, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST, PAD_NOTEXIST,
		PAD_NOTEXIST, PAD_NOTEXIST
	}};

	for (PortID=0; PortID<3; PortID++)
	{
		MES_GPIO_SelectModule( PortID );
		for (BitNum=0; BitNum<32; BitNum++)
		{
			if(IOMode[PortID][BitNum] != PAD_NOTEXIST)
				switch(IOMode[PortID][BitNum])
				{
				case PAD_GPIOIN:
					MES_GPIO_SetPullUpEnable( BitNum, CFALSE );
					MES_GPIO_SetOutputEnable( BitNum, CFALSE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_GPIO );
					break;
				case PAD_GPIOIN_PULLUP:
					MES_GPIO_SetPullUpEnable( BitNum, CTRUE );
					MES_GPIO_SetOutputEnable( BitNum, CFALSE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_GPIO );
					break;
				case PAD_GPIOOUT:
					MES_GPIO_SetPullUpEnable( BitNum, CFALSE );
					MES_GPIO_SetOutputEnable( BitNum, CTRUE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_GPIO );
					break;
				case PAD_GPIOOUT_PULLUP:
					MES_GPIO_SetPullUpEnable( BitNum, CTRUE );
					MES_GPIO_SetOutputEnable( BitNum, CTRUE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_GPIO );
					break;
				case PAD_ALT1:
					MES_GPIO_SetPullUpEnable( BitNum, CFALSE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_1 );
					break;
				case PAD_ALT1_PULLUP:
					MES_GPIO_SetPullUpEnable( BitNum, CTRUE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_1 );
					break;
				case PAD_ALT2:
					MES_GPIO_SetPullUpEnable( BitNum, CFALSE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_2 );
					break;
				case PAD_ALT2_PULLUP:
					MES_GPIO_SetPullUpEnable( BitNum, CTRUE );
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_2 );
					break;
				case PAD_NOTUSED:
				default:
					MES_GPIO_SetPullUpEnable( BitNum, CFALSE );// not pullup
					MES_GPIO_SetOutputEnable( BitNum, CTRUE );// output
					MES_GPIO_SetPadFunction( BitNum, MES_GPIO_PADFUNC_GPIO );// low
					break;
				}//switch(IOMode[PortID][BitNum])
		}//for (int BitNum=0; BitNum<(int)32; BitNum++)
	}//for (int PortID=0; PortID<GPIO_MAX; PortID++)
//	MES_ALIVE_GetAliveGpio(U32 GpioNum)

	MES_ALIVE_SetWriteEnable( CTRUE );
	MES_ALIVE_SetAliveGpio( NAND_WP, CTRUE ); 
	MES_ALIVE_SetWriteEnable( CFALSE );

    //led_on_green
    MES_ALIVE_SetWriteEnable( CTRUE );
	MES_ALIVE_SetAliveGpio( POWER_ON_LED, CTRUE ); 
	MES_ALIVE_SetWriteEnable( CFALSE );
	    
    MES_SetOutputValueBit(OEM_GPIO_USB5V_CTRL, CFALSE);   		/* vbus low */    
	MES_SetOutputValueBit(OEM_GPIO_USBH2_CURRENT, CFALSE);     /* usb host2.0 vbus current disable */
	MES_SetOutputValueBit(OEM_GPIO_USBH2_EN, CFALSE);     	   /* usb host2.0 power disable */
	
    if( GetInputValue(OEM_GPIO_BD_NUM_LSB) && (!GetInputValue(OEM_GPIO_BD_NUM_MSB)) )
        board_rev = 1;
    else if( (!GetInputValue(OEM_GPIO_BD_NUM_LSB)) && GetInputValue(OEM_GPIO_BD_NUM_MSB) )
        board_rev = 2;
    else if( GetInputValue(OEM_GPIO_BD_NUM_LSB) && GetInputValue(OEM_GPIO_BD_NUM_MSB) )
        board_rev = 3;
    
    MES_SetOutputValueBit(OEM_GPIO_SPK_ENB, CFALSE);
    MES_SetOutputValueBit(OEM_GPIO_HMUTE_CNTL, CFALSE);    
}

void InitMemoryInterface(void)
{

	// DDR2 SDRAM
	MES_MCUD_SetDIC			( (MES_MCUD_DIC) CFG_SYS_BANKA_DIC );	
	MES_MCUD_SetDLLEnable	( (CBOOL) CFG_SYS_BANKA_USEDLL );	

	MES_MCUD_SetCASLatency	( (MES_MCUD_LATENCY) CFG_SYS_BANKA_CASLAT  );
	MES_MCUD_SetReadLatency	( (MES_MCUD_LATENCY) CFG_SYS_BANKA_READLAT );

	MES_MCUD_SetMRD	( CFG_SYS_BANKA_TMRD );
	MES_MCUD_SetRP	( CFG_SYS_BANKA_TRP  );
	MES_MCUD_SetRC	( CFG_SYS_BANKA_TRC	 );
	MES_MCUD_SetRCD	( CFG_SYS_BANKA_TRCD );
	MES_MCUD_SetRAS	( CFG_SYS_BANKA_TRAS );
	MES_MCUD_SetWR	( CFG_SYS_BANKA_TWR  );
	
	MES_MCUD_SetClockDelay	( (MES_MCUD_DELAY) CFG_SYS_BANKA_CLKDLY );
	MES_MCUD_SetDQSDelay	( (MES_MCUD_DELAY) CFG_SYS_BANKA_DQS0OUTDLY,
				  (MES_MCUD_DELAY) CFG_SYS_BANKA_DQS1OUTDLY,
				  (MES_MCUD_DELAY) CFG_SYS_BANKA_DQS0INDLY ,
				  (MES_MCUD_DELAY) CFG_SYS_BANKA_DQS1INDLY );
	
	MES_MCUD_SetRefreshPeriod( (U16) CFG_SYS_BANKA_REFPRED );

	MES_MCUD_SetDisplayBlockMode( CTRUE );
	
	// BANK A register update
	MES_MCUD_ApplyModeSetting();

	do	
	{
		volatile U32 x;
		for( x=0 ; x<0x01FF ; x++ );
	}
	while( MES_MCUD_IsBusyModeSetting() );

#define STATIC_BUS_CONFIGUTATION( _n_ )						\
	MES_MCUS_SetStaticBUSConfig						\
	( 									\
		MES_MCUS_SBUSID_STATIC ## _n_, 					\
		CFG_SYS_STATIC ## _n_ ## _BW, 					\
		CFG_SYS_STATIC ## _n_ ## _TACS, 				\
		CFG_SYS_STATIC ## _n_ ## _TCAH, 				\
		CFG_SYS_STATIC ## _n_ ## _TCOS, 				\
		CFG_SYS_STATIC ## _n_ ## _TCOH, 				\
		CFG_SYS_STATIC ## _n_ ## _TACC, 				\
		CFG_SYS_STATIC ## _n_ ## _TSACC,				\
		(MES_MCUS_WAITMODE ) CFG_SYS_STATIC ## _n_ ## _WAITMODE, 	\
		(MES_MCUS_BURSTMODE) CFG_SYS_STATIC ## _n_ ## _RBURST, 		\
		(MES_MCUS_BURSTMODE) CFG_SYS_STATIC ## _n_ ## _WBURST		\
	);

	STATIC_BUS_CONFIGUTATION( 0 );
	STATIC_BUS_CONFIGUTATION( 1 );
	STATIC_BUS_CONFIGUTATION( 2 );
	STATIC_BUS_CONFIGUTATION( 3 );
	STATIC_BUS_CONFIGUTATION( 4 );
	STATIC_BUS_CONFIGUTATION( 5 );
	STATIC_BUS_CONFIGUTATION( 6 );
	STATIC_BUS_CONFIGUTATION( 7 );
	STATIC_BUS_CONFIGUTATION( 8 );
	STATIC_BUS_CONFIGUTATION( 9 );

	// NAND Bus config
	MES_MCUS_SetStaticBUSConfig
	( 
		MES_MCUS_SBUSID_NAND,	// bus type 		: NAND
		0,						// bit width 		: Not used
		CFG_SYS_NAND_TACS,		// tACS  ( 0 ~ 3  ) 
		CFG_SYS_NAND_TCAH,		// tCAH  ( 0 ~ 3  )
		CFG_SYS_NAND_TCOS,		// tCOS  ( 0 ~ 3  )
		CFG_SYS_NAND_TCOH,		// tCOH  ( 0 ~ 3  )
		CFG_SYS_NAND_TACC,		// tACC  ( 1 ~ 16 )
		0,						// tSACC ( 1 ~ 16 ) : Not used
		(MES_MCUS_WAITMODE)0,	// Wait mode		: Not used
		(MES_MCUS_BURSTMODE)0,	// Read  burst mode	: Not used
		(MES_MCUS_BURSTMODE)0	// Write burst mode : Not used
	);
}

void InitializeDisplay(void)
{
#ifndef CONFIG_LCD_NOINIT
	InitializeDPC();
	InitializeMLC();
	InitializeLCD();
	LcdTurnOn();
#endif /* CONFIG_LCD_NOINIT */
}

#define DEVICEID_LGPHILIPS		0x70

#if 0
//bnjang [2009.04.06]
unsigned short valueREG[][2] = {
			     {0x03, 0x0117},
			     {0xff, 100},
			     {0x0D, 0x0030},
 			     {0xff, 100}, 
			     {0x0E, 0x2a00}, //2800
 			     {0xff, 100}, 
			     {0x1E, 0x00c1}, //c1
 			     {0xff, 1000},   // 30ms

			     {0x01, 0x6300}, 
			     {0x02, 0x200},
			     {0x04, 0x04C7},
 			     {0x05, 0xFFC0},
			     {0x0A, 0x4008},
			     {0x0B, 0x0000},
			     {0x0F, 0x0000},

			     {0x16, 0x9F80},
			     {0x17, 0x2212}, //0a0f
 			     
 			     
 			     {0x30, 0x0300}, 
 			     {0x31, 0x0007}, 
 			     {0x32, 0x0000}, 
 			     {0x33, 0x0000}, 
 			     {0x34, 0x0707}, 
 			     {0x35, 0x0004}, 
 			     {0x36, 0x0302}, 
 			     {0x37, 0x0202},
 			     {0x3A, 0x0a0d},
 			     {0x3B, 0x0806},
 			     
 			     {0xff, 1000}};
#else

//bnjang [2009.04.06]
//edited by smhong [2010.06.10]  for LG LCD (LB035Q04-TD07)
unsigned short valueREG[][2] = {
			     {0x01, 0x6300}, 
			     {0x02, 0x0200},
			     {0x03, 0x041C}, //{0x03, 0x0117},
			     {0xff, 100},
			     {0x04, 0x04C7},
				 {0x05, 0xFC70}, //{0x05, 0xFFC0},
				 {0x06, 0xE806},
			     {0x0A, 0x4008},
			     {0x0B, 0x0000},
			     {0x0D, 0x0024}, //{0x0D, 0x0030},
 			     {0xff, 100}, 
			     {0x0E, 0x29C0}, //{0x0E, 0x2800},
 			     {0xff, 100}, 
			     {0x0F, 0x0000},
				 //porch
			     {0x16, 0x9F80},
			     {0x17, 0x0A0F}, //{0x17, 0x2212}, //0a0f
			     {0x1E, 0x00DD}, //{0x1E, 0x00DD}, //{0x1E, 0x00c1}, //c1
 			     {0xff, 1000},   // 30ms
				 //gamma collection
			     {0x30, 0x0700},
			     {0x31, 0x0207},
			     {0x32, 0x0000},
			     {0x33, 0x0105},
			     {0x34, 0x0007},
			     {0x35, 0x0101},
			     {0x36, 0x0707},
			     {0x37, 0x0504},
			     {0x3A, 0x140F},
			     {0x3B, 0x0509},
 			     {0xff, 1000}};

#endif


void CountDelay(int count)
{	
	volatile unsigned int delay = 0;
 	for(delay = 0; delay<count*100; delay++); 
}

void InitializeLCD(void)
{
    unsigned int i;
    MES_SetOutputValueBit(OEM_GPIO_LCD_ENB, CFALSE);//l
    CountDelay(100);
    MES_SetOutputValueBit(OEM_GPIO_LCD_ENB, CTRUE); //h

	if( board_rev){   /* lg */
		CS_HIGH;
    	SCL_HIGH;
    	SDI_HIGH;
    	
    	for(i=0; i < sizeof(valueREG)/sizeof(valueREG[0]) ; i++){
    		if (valueREG[i][0] == 0xff)
    		{
    			CountDelay(valueREG[i][1]);
    			continue;
    		}
        	lcdSetWrite(DEVICEID_LGPHILIPS, valueREG[i][0], valueREG[i][1]);	
    	}
	}else {				/* board_rev = 0 (TCL) */	
		CS_HIGH;
    	SCL_HIGH;
    	SDI_LOW;
		
		lcd_spi_write(0x04, 0x01); //sw reset
		/* power set */
		lcd_spi_write(0x05, 0x02);
		lcd_spi_write(0x07, 0xef);
		lcd_spi_write(0x0C, 0x44);
		lcd_spi_write(0x0d, 0x0a);
		lcd_spi_write(0x30, 0x09);
	
		/* vcom set */
		//lcd_spi_write(0x01, 0x12);
		//lcd_spi_write(0x02, 0x3d);
		//lcd_spi_write(0x03, 0x0e);

    	lcd_spi_write(0x0E, 0x08); // contrast 
    	lcd_spi_write(0x0F, 0x84); // brightness (0x80)
    
    	/* lcd set */
		lcd_spi_write(0x06, 0x5f); 
		lcd_spi_write(0x08, 0x11);
		lcd_spi_write(0x09, 0x80); 	
		lcd_spi_write(0x0A, 0x49); 
		//lcd_spi_write(0x0A, 0x48);
		lcd_spi_write(0x0B, 0x04); 

		/* gamma set */
		//lcd_spi_write(0x10, 0x77); 
		//lcd_spi_write(0x11, 0x77); 
		//lcd_spi_write(0x12, 0x77); 
		//lcd_spi_write(0x13, 0x77); 
		//lcd_spi_write(0x14, 0x77); 
		//lcd_spi_write(0x15, 0x77); 
		//lcd_spi_write(0x16, 0x77); 
		//lcd_spi_write(0x17, 0x77); 

		lcd_spi_write(0x42, 0x00);  // otp disable 		
    	CountDelay(2000);
	}
}

void InitializeDPC(void)
{
	//--------------------------------------------------------------------------
	//	SECONDARY Display Controller
	//--------------------------------------------------------------------------
	#if (DISPLAY_SEC_DISPLAY_ENABLE==CTRUE)
	{
	    MES_DPC_SelectModule( 1 );
		MES_DPC_SetClockPClkMode(MES_PCLKMODE_DYNAMIC);
	
		MES_DPC_SetClockOutEnb( 0, CFALSE );
		MES_DPC_SetDPCEnable( CFALSE );
		MES_DPC_SetClockDivisorEnable(CFALSE);
		
		#if ( DISPLAY_SEC_SCALE_UP_ENABLE == CTRUE)
			if(DISPLAY_SEC_MAX_X_RESOLUTION > DISPLAY_PRI_MAX_X_RESOLUTION)
				MES_DPC_SetHorizontalUpScaler( CTRUE, DISPLAY_PRI_MAX_X_RESOLUTION, DISPLAY_SEC_MAX_X_RESOLUTION );
			else
				MES_DPC_SetHorizontalUpScaler( CFALSE, 2, 2 );
		#else
			MES_DPC_SetHorizontalUpScaler( CFALSE, 2, 2 );
		#endif

		//----------------------------------------------------------------------
	    // Internal/External Encoder Mode
	    //----------------------------------------------------------------------
    	// VCLK2 : CLKGEN0
    	MES_DPC_SetClockSource  (0, DISPLAY_DPC_SEC_VCLK_SOURCE);	// CLKSRCSEL
    	MES_DPC_SetClockDivisor (0, DISPLAY_DPC_SEC_VCLK_DIV);		// CLKDIV
    	MES_DPC_SetClockOutDelay(0, 0); 							// OUTCLKDELAY
    
    	// VCLK : CLKGEN1
    	MES_DPC_SetClockSource  (1, DISPLAY_DPC_SEC_VCLK2_SOURCE);	// CLKSRCSEL  : CLKGEN0's out
    	MES_DPC_SetClockDivisor (1, DISPLAY_DPC_SEC_VCLK2_DIV);		// CLKDIV
    	MES_DPC_SetClockOutDelay(1, 0); 							// OUTCLKDELAY

    	MES_DPC_SetMode( (MES_DPC_FORMAT)DISPLAY_DPC_SEC_OUTPUT_FORMAT,	// FORMAT
    	                 DISPLAY_DPC_SEC_SCAN_INTERLACE,			// SCANMODE
    	             	 DISPLAY_DPC_SEC_POLFIELD_INVERT,			// POLFIELD
    	             	 CFALSE, 									// RGBMODE
    	             	 CFALSE,       								// SWAPRB
    	             	 (MES_DPC_YCORDER)DISPLAY_DPC_SEC_OUTORDER,	// YCORDER
    	             	 CTRUE,										// YCCLIP
    	             	 CFALSE,  									// Embedded sync
    	             	 (MES_DPC_PADCLK)DISPLAY_DPC_SEC_PAD_VCLK,	// PADCLKSEL
    	             	 DISPLAY_DPC_SEC_CLOCK_INVERT				// PADCLKINV
    				   );

    	MES_DPC_SetHSync( DISPLAY_SEC_MAX_X_RESOLUTION,
    					  DISPLAY_DPC_SEC_HSYNC_SWIDTH,
    					  DISPLAY_DPC_SEC_HSYNC_FRONT_PORCH,
    					  DISPLAY_DPC_SEC_HSYNC_BACK_PORCH,
    					  DISPLAY_DPC_SEC_HSYNC_ACTIVEHIGH );

    	MES_DPC_SetVSync( DISPLAY_SEC_MAX_Y_RESOLUTION/2,
    					  DISPLAY_DPC_SEC_VSYNC_SWIDTH,
    					  DISPLAY_DPC_SEC_VSYNC_FRONT_PORCH,
    					  DISPLAY_DPC_SEC_VSYNC_BACK_PORCH,
    					  DISPLAY_DPC_SEC_VSYNC_ACTIVEHIGH,
    					  DISPLAY_SEC_MAX_Y_RESOLUTION/2,
    					  3,
    					  4,
    					  16 );

    	MES_DPC_SetVSyncOffset( 0, 0, 0, 0 );

		switch(DISPLAY_DPC_SEC_OUTPUT_FORMAT)
		{
			case DPC_FORMAT_RGB555:
			case DPC_FORMAT_MRGB555A:
			case DPC_FORMAT_MRGB555B:
				MES_DPC_SetDither(MES_DPC_DITHER_5BIT,		// DPCCTRL1: RDITHER-6bit
							      MES_DPC_DITHER_5BIT,		// DPCCTRL1: GDITHER-6bit
							  	  MES_DPC_DITHER_5BIT); 	// DPCCTRL1: BDITHER-6bit
				break;
			case DPC_FORMAT_RGB565:
			case DPC_FORMAT_MRGB565:
				MES_DPC_SetDither(MES_DPC_DITHER_5BIT,		// DPCCTRL1: RDITHER-6bit
							      MES_DPC_DITHER_6BIT,		// DPCCTRL1: GDITHER-6bit
							      MES_DPC_DITHER_5BIT);		// DPCCTRL1: BDITHER-6bit
				break;
			case DPC_FORMAT_RGB666:
			case DPC_FORMAT_MRGB666:
				MES_DPC_SetDither(MES_DPC_DITHER_6BIT,		// DPCCTRL1: RDITHER-6bit
							      MES_DPC_DITHER_6BIT,		// DPCCTRL1: GDITHER-6bit
							      MES_DPC_DITHER_6BIT);		// DPCCTRL1: BDITHER-6bit
				break;
			default:
				MES_DPC_SetDither(MES_DPC_DITHER_BYPASS,	// DPCCTRL1: RDITHER-6bit
							      MES_DPC_DITHER_BYPASS,	// DPCCTRL1: GDITHER-6bit
							  	  MES_DPC_DITHER_BYPASS);	// DPCCTRL1: BDITHER-6bit
				break;
		}

		MES_DPC_SetDelay( 0,							// DELAYRGB_PVD
	 	             	  DISPLAY_DPC_SEC_SYNC_DELAY,	// DELAYHS_CP1
		             	  DISPLAY_DPC_SEC_SYNC_DELAY,	// DELAYVS_FRAM
		             	  DISPLAY_DPC_SEC_SYNC_DELAY );	// DELAYDE_CP2

		#if (DISPLAY_DPC_SEC_ENCODER_ON == CTRUE)
		{
			MES_DPC_SetENCEnable( CTRUE );
			CountDelay( 100 );
			MES_DPC_SetClockDivisorEnable(CTRUE);
			CountDelay( 100 );
			MES_DPC_SetENCEnable( CFALSE );
			CountDelay( 100 );
			MES_DPC_SetClockDivisorEnable(CFALSE);
			CountDelay( 100 );
			MES_DPC_SetENCEnable( CTRUE );
				
			MES_DPC_SetVideoEncoderPowerDown( CTRUE );
			MES_DPC_SetVideoEncoderMode( (MES_DPC_VBS)DISPLAY_DPC_SEC_ENCODER_FORMAT, CTRUE );
			MES_DPC_SetVideoEncoderFSCAdjust( 0 );
			MES_DPC_SetVideoEncoderBandwidth( MES_DPC_BANDWIDTH_LOW, MES_DPC_BANDWIDTH_LOW);
			MES_DPC_SetVideoEncoderColorControl( 0, 0, 0, 0, 0 );
			MES_DPC_SetVideoEncoderTiming( 64, 1716, 0, 3 );
			MES_DPC_SetVideoEncoderPowerDown( CFALSE );
		}
		#else
		{
			MES_DPC_SetENCEnable( CFALSE );
		}
		#endif
		
		MES_DPC_SetDPCEnable( CTRUE );
		MES_DPC_SetClockDivisorEnable(CTRUE);	// CLKENB : Provides internal operating clock.
		MES_DPC_SetClockOutEnb( 0, CFALSE );	// OUTCLKENB : Enable
	}
	#endif	// SECONDARY DPC

	//--------------------------------------------------------------------------
	//	PRIMARY Display Controller
	//--------------------------------------------------------------------------
	#if (DISPLAY_PRI_DISPLAY_ENABLE==CTRUE)
	{
	    #if (DISPLAY_PRI_INITIAL_LOGO_ON == CTRUE)
		//OEM_DrawLogo(PRI_MLC_FRAME_BASE, DISPLAY_PRI_MAX_X_RESOLUTION, DISPLAY_PRI_MAX_Y_RESOLUTION, X_STRIDE);
		#endif
		
	    MES_DPC_SelectModule( 0 );
		MES_DPC_SetClockPClkMode(MES_PCLKMODE_ALWAYS);

		MES_DPC_SetClockOutEnb( 0, CFALSE );
		MES_DPC_SetDPCEnable( CFALSE );
		MES_DPC_SetClockDivisorEnable(CFALSE);
		MES_DPC_SetHorizontalUpScaler( CFALSE, 2, 2 );


		// VCLK2 : CLKGEN0
	    MES_DPC_SetClockSource  (0, DISPLAY_DPC_PRI_VCLK_SOURCE);	// CLKSRCSEL
	    //MES_DPC_SetClockDivisor (0, DISPLAY_DPC_PRI_VCLK_DIV); // CLKDIV
	    MES_DPC_SetClockDivisor (0, (board_rev == 0 ) ?
    						DISPLAY_DPC_PRI_VCLK_DIV_TCL : DISPLAY_DPC_PRI_VCLK_DIV); // CLKDIV
	    MES_DPC_SetClockOutDelay(0, 0); 							// OUTCLKDELAY
	    MES_DPC_SetClockOutEnb( 0, CTRUE );							// OUTCLKENB : Enable
	    	
	    // VCLK : CLKGEN1
	    MES_DPC_SetClockSource  (1, DISPLAY_DPC_PRI_VCLK2_SOURCE);	// CLKSRCSEL  : CLKGEN0's out
	    MES_DPC_SetClockDivisor (1, DISPLAY_DPC_PRI_VCLK2_DIV);		// CLKDIV
	    MES_DPC_SetClockOutDelay(1, 0); 							// OUTCLKDELAY
	
		MES_DPC_SetClockDivisorEnable(CTRUE);	// CLKENB : Provides internal operating clock.

		//----------------------------------------------------------------------
	    // TFT LCD or Internal/External Encoder Mode
	    //----------------------------------------------------------------------
		MES_DPC_SetMode( (	MES_DPC_FORMAT)DISPLAY_DPC_PRI_OUTPUT_FORMAT,	// FORMAT
	    	              	DISPLAY_DPC_PRI_TFT_SCAN_INTERLACE,				// SCANMODE
	    	            	DISPLAY_DPC_PRI_TFT_POLFIELD_INVERT,			// POLFIELD
	    	             	CTRUE, 											// RGBMODE
	    	             	CFALSE,       									// SWAPRB
	    	             	(MES_DPC_YCORDER)DISPLAY_DPC_PRI_TFT_OUTORDER,	// YCORDER
	    	             	CFALSE,											// YCCLIP
	    	             	CFALSE,  										// Embedded sync
	    	             	(MES_DPC_PADCLK)DISPLAY_DPC_PRI_PAD_VCLK,		// PADCLKSEL
	    	             	DISPLAY_DPC_PRI_CLOCK_INVERT					// PADCLKINV
	    					);
			
		switch(DISPLAY_DPC_PRI_OUTPUT_FORMAT)
		{
			case DPC_FORMAT_RGB555:
			case DPC_FORMAT_MRGB555A:
			case DPC_FORMAT_MRGB555B:
				MES_DPC_SetDither(MES_DPC_DITHER_5BIT,		// DPCCTRL1: RDITHER-6bit
								  MES_DPC_DITHER_5BIT,		// DPCCTRL1: GDITHER-6bit
								  MES_DPC_DITHER_5BIT); 	// DPCCTRL1: BDITHER-6bit
				break;
			case DPC_FORMAT_RGB565:
			case DPC_FORMAT_MRGB565:
				MES_DPC_SetDither(MES_DPC_DITHER_5BIT,		// DPCCTRL1: RDITHER-6bit
								  MES_DPC_DITHER_6BIT,		// DPCCTRL1: GDITHER-6bit
								  MES_DPC_DITHER_5BIT);		// DPCCTRL1: BDITHER-6bit
				break;
			case DPC_FORMAT_RGB666:
			case DPC_FORMAT_MRGB666:
				MES_DPC_SetDither(MES_DPC_DITHER_6BIT,		// DPCCTRL1: RDITHER-6bit
								  MES_DPC_DITHER_6BIT,		// DPCCTRL1: GDITHER-6bit
								  MES_DPC_DITHER_6BIT);		// DPCCTRL1: BDITHER-6bit
				break;
			default:
				MES_DPC_SetDither(MES_DPC_DITHER_BYPASS,	// DPCCTRL1: RDITHER-6bit
								  MES_DPC_DITHER_BYPASS,	// DPCCTRL1: GDITHER-6bit
								  MES_DPC_DITHER_BYPASS);	// DPCCTRL1: BDITHER-6bit
				break;
		}



	    MES_DPC_SetHSync( DISPLAY_PRI_MAX_X_RESOLUTION,
	    				  DISPLAY_DPC_PRI_TFT_HSYNC_SWIDTH,
	    			      DISPLAY_DPC_PRI_TFT_HSYNC_FRONT_PORCH,
	    				  DISPLAY_DPC_PRI_TFT_HSYNC_BACK_PORCH,
	    				  DISPLAY_DPC_PRI_TFT_HSYNC_ACTIVEHIGH );


	    MES_DPC_SetVSync( DISPLAY_PRI_MAX_Y_RESOLUTION,
	    				  DISPLAY_DPC_PRI_TFT_VSYNC_SWIDTH,
	    				  DISPLAY_DPC_PRI_TFT_VSYNC_FRONT_PORCH,
	    				  DISPLAY_DPC_PRI_TFT_VSYNC_BACK_PORCH,
	    				  DISPLAY_DPC_PRI_TFT_VSYNC_ACTIVEHIGH,
	    					  1,
	    					  1,
	    					  1,
	    					  1 );

	
	    //MES_DPC_SetVSyncOffset( 1, 1, 1, 1 );
		MES_DPC_SetVSyncOffset( 0, 0, 0, 0 );
		MES_DPC_SetDelay( 0,							// DELAYRGB_PVD
	 	             	  DISPLAY_DPC_PRI_SYNC_DELAY,	// DELAYHS_CP1
		             	  DISPLAY_DPC_PRI_SYNC_DELAY,	// DELAYVS_FRAM
		             	  DISPLAY_DPC_PRI_SYNC_DELAY );	// DELAYDE_CP2

		MES_DPC_SetSecondaryDPCSync( CFALSE );
		MES_DPC_SetDPCEnable( CTRUE );
		
		
	}	
	#endif	// PRIMARY DPC
}

void InitializeMLC(void)
{
	U32 X_STRIDE = DISPLAY_MLC_BYTE_PER_PIXEL * DISPLAY_PRI_MAX_X_RESOLUTION;

	//--------------------------------------------------------------------------
	//	SECONDARY Multi Layer Contoller
	//--------------------------------------------------------------------------
	#if (DISPLAY_SEC_DISPLAY_ENABLE == CTRUE)
	{
		U32 SEC_MLC_FRAME_BASE;
		U32 SEC_X_RESOLUTION = DISPLAY_SEC_MAX_X_RESOLUTION;
		U32 SEC_Y_RESOLUTION = DISPLAY_SEC_MAX_Y_RESOLUTION;

		#if ( DISPLAY_SEC_SCALE_UP_ENABLE == CTRUE)
			if( DISPLAY_SEC_MAX_X_RESOLUTION > DISPLAY_PRI_MAX_X_RESOLUTION )
				SEC_X_RESOLUTION = DISPLAY_PRI_MAX_X_RESOLUTION;

			if( DISPLAY_SEC_MAX_Y_RESOLUTION > DISPLAY_PRI_MAX_Y_RESOLUTION )
				SEC_Y_RESOLUTION = DISPLAY_PRI_MAX_Y_RESOLUTION;
		#endif

		#if (DISPLAY_PRI_DISPLAY_ENABLE == CTRUE)
		{	
			#if ( DISPLAY_MAIN_DISPLAY_SCREEN == DISP_ALL_MAIN_SCREEN )
				X_STRIDE = DISPLAY_MLC_BYTE_PER_PIXEL * ( (DISPLAY_PRI_MAX_X_RESOLUTION > SEC_X_RESOLUTION) ?
						   DISPLAY_PRI_MAX_X_RESOLUTION : SEC_X_RESOLUTION ) ;
			#else
				X_STRIDE = DISPLAY_MLC_BYTE_PER_PIXEL * (DISPLAY_PRI_MAX_X_RESOLUTION + SEC_X_RESOLUTION);
			#endif
				
			#if (DISPLAY_MAIN_DISPLAY_SCREEN == DISP_PRI_MAIN_SCREEN)
				SEC_MLC_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START + (DISPLAY_PRI_MAX_X_RESOLUTION * DISPLAY_MLC_BYTE_PER_PIXEL);
			#else 
				SEC_MLC_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START;
			#endif	
		}
		#else
		{
			X_STRIDE = DISPLAY_VID_BYTE_PER_PIXEL * SEC_X_RESOLUTION;
			SEC_VID_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START;
		}		
		#endif

		#if (DISPLAY_SEC_INITIAL_LOGO_ON == CTRUE)
		{
			OEM_DrawLogo(SEC_MLC_FRAME_BASE, DISPLAY_PRI_MAX_X_RESOLUTION, DISPLAY_PRI_MAX_Y_RESOLUTION, X_STRIDE);
		}
		#endif

	    MES_MLC_SelectModule( 1 );
		MES_MLC_SetClockPClkMode( MES_PCLKMODE_DYNAMIC );
		MES_MLC_SetClockBClkMode( MES_BCLKMODE_DYNAMIC );

		//----------------------------------------------------------------------
		// SECONDARY RGB Layer TOP Field
		//----------------------------------------------------------------------
	    MES_MLC_SetLayerPriority( (MES_MLC_PRIORITY)DISPLAY_MLC_VIDEO_LAYER_PRIORITY );
	    MES_MLC_SetTopPowerMode ( CTRUE );
	    MES_MLC_SetTopSleepMode ( CFALSE );
	
		#if ( DISPLAY_SEC_SCALE_UP_ENABLE == CTRUE)
			if(DISPLAY_SEC_MAX_Y_RESOLUTION > DISPLAY_PRI_MAX_Y_RESOLUTION)
			MES_MLC_SetFieldEnable( CFALSE );
			else
			MES_MLC_SetFieldEnable( CTRUE );
		#else
			MES_MLC_SetFieldEnable( CTRUE );
		#endif

		MES_MLC_SetScreenSize( SEC_X_RESOLUTION, SEC_Y_RESOLUTION );		
		MES_MLC_SetBackground(DISPLAY_MLC_BACKGROUND_COLOR);
	
		MES_MLC_SetMLCEnable( CTRUE );
		MES_MLC_SetTopDirtyFlag();

		//----------------------------------------------------------------------
		// SECONDARY RGB Layer SCREEN Field
		//----------------------------------------------------------------------
		MES_MLC_SetAlphaBlending  ( LAYER_DISPLAY_SCREEN_RGB, CFALSE, 15 );
		MES_MLC_SetTransparency   ( LAYER_DISPLAY_SCREEN_RGB, CFALSE,  0 );
		MES_MLC_SetColorInversion ( LAYER_DISPLAY_SCREEN_RGB, CFALSE,  0 );
		MES_MLC_SetLockSize       ( LAYER_DISPLAY_SCREEN_RGB, 4 );
		MES_MLC_Set3DEnb          ( LAYER_DISPLAY_SCREEN_RGB, CFALSE );
	
	    MES_MLC_SetLayerPowerMode ( LAYER_DISPLAY_SCREEN_RGB, CTRUE );
		MES_MLC_SetLayerSleepMode ( LAYER_DISPLAY_SCREEN_RGB, CFALSE );
		
		MES_MLC_SetFormat         ( LAYER_DISPLAY_SCREEN_RGB, (MES_MLC_RGBFMT)DISPLAY_MLC_RGB_FORMAT );
		MES_MLC_SetPosition       ( LAYER_DISPLAY_SCREEN_RGB, 0, 0, SEC_X_RESOLUTION-1, SEC_Y_RESOLUTION-1 );

		MES_MLC_SetRGBLayerStride ( LAYER_DISPLAY_SCREEN_RGB, DISPLAY_MLC_BYTE_PER_PIXEL, X_STRIDE );
		MES_MLC_SetRGBLayerAddress( LAYER_DISPLAY_SCREEN_RGB, SEC_MLC_FRAME_BASE );
		
		MES_MLC_SetRGBLayerInvalidPosition( LAYER_DISPLAY_SCREEN_RGB, 0, 0, 0, 0, 0, CFALSE );
		MES_MLC_SetRGBLayerInvalidPosition( LAYER_DISPLAY_SCREEN_RGB, 1, 0, 0, 0, 0, CFALSE );
		
		MES_MLC_SetLayerEnable( LAYER_DISPLAY_SCREEN_RGB, CTRUE );
		MES_MLC_SetDirtyFlag( LAYER_DISPLAY_SCREEN_RGB );
	}
	#endif // SECONDARY MLC

	//--------------------------------------------------------------------------
	//	PRIMARY Multi Layer Contoller
	//--------------------------------------------------------------------------
	#if (DISPLAY_PRI_DISPLAY_ENABLE == CTRUE)
	{

		U32 PRI_MLC_FRAME_BASE;
		#if (DISPLAY_SEC_DISPLAY_ENABLE == CTRUE && DISPLAY_MAIN_DISPLAY_SCREEN == DISP_SEC_MAIN_SCREEN)
			#if ( DISPLAY_SEC_SCALE_UP_ENABLE == CTRUE)
				if( DISPLAY_SEC_MAX_X_RESOLUTION > DISPLAY_PRI_MAX_X_RESOLUTION )
				PRI_MLC_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START + (DISPLAY_PRI_MAX_X_RESOLUTION*DISPLAY_MLC_BYTE_PER_PIXEL);
				else
				PRI_MLC_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START + (DISPLAY_SEC_MAX_X_RESOLUTION*DISPLAY_MLC_BYTE_PER_PIXEL);
			#else
			PRI_MLC_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START + (DISPLAY_SEC_MAX_X_RESOLUTION*DISPLAY_MLC_BYTE_PER_PIXEL);
			#endif
		#else
			PRI_MLC_FRAME_BASE = OEM_MEM_PHY_FRAMEBUFFER_START;
		#endif	

		MES_MLC_SelectModule( 0 );
		MES_MLC_SetClockPClkMode( MES_PCLKMODE_ALWAYS );
		MES_MLC_SetClockBClkMode( MES_BCLKMODE_ALWAYS );
	
		//----------------------------------------------------------------------
		// PRIMARY RGB Layer TOP Field
		//----------------------------------------------------------------------
	    MES_MLC_SetTop3DAddrChangeSync( MES_MLC_3DSYNC_PRIMARY );  
		MES_MLC_SetLayerPriority( (MES_MLC_PRIORITY)DISPLAY_MLC_VIDEO_LAYER_PRIORITY );
	    MES_MLC_SetTopPowerMode ( CTRUE );
	    MES_MLC_SetTopSleepMode ( CFALSE );
	
		MES_MLC_SetFieldEnable( CFALSE );
		MES_MLC_SetScreenSize( DISPLAY_PRI_MAX_X_RESOLUTION, DISPLAY_PRI_MAX_Y_RESOLUTION );
		//MES_MLC_SetBackground(DISPLAY_MLC_BACKGROUND_COLOR);
		MES_MLC_SetBackground(0x00000000);
		MES_MLC_SetMLCEnable( CTRUE );
		MES_MLC_SetTopDirtyFlag();
		
		//----------------------------------------------------------------------
		// PRIMARY RGB Layer SCREEN Field
		//----------------------------------------------------------------------
		MES_MLC_SetAlphaBlending  ( LAYER_DISPLAY_SCREEN_RGB, CFALSE,  0 );  //15
		MES_MLC_SetTransparency   ( LAYER_DISPLAY_SCREEN_RGB, CFALSE,  0 );
		MES_MLC_SetColorInversion ( LAYER_DISPLAY_SCREEN_RGB, CFALSE,  0 );
		MES_MLC_SetLockSize       ( LAYER_DISPLAY_SCREEN_RGB, 8 );         //4
		MES_MLC_Set3DEnb          ( LAYER_DISPLAY_SCREEN_RGB, CFALSE );
	
	    MES_MLC_SetLayerPowerMode ( LAYER_DISPLAY_SCREEN_RGB, CTRUE );
		MES_MLC_SetLayerSleepMode ( LAYER_DISPLAY_SCREEN_RGB, CFALSE );
		
		MES_MLC_SetFormat         ( LAYER_DISPLAY_SCREEN_RGB, (MES_MLC_RGBFMT)DISPLAY_MLC_RGB_FORMAT );
		MES_MLC_SetPosition       ( LAYER_DISPLAY_SCREEN_RGB, 0, 0, DISPLAY_PRI_MAX_X_RESOLUTION-1, DISPLAY_PRI_MAX_Y_RESOLUTION-1 );
		MES_MLC_SetRGBLayerStride ( LAYER_DISPLAY_SCREEN_RGB, DISPLAY_MLC_BYTE_PER_PIXEL, X_STRIDE );
		MES_MLC_SetRGBLayerAddress( LAYER_DISPLAY_SCREEN_RGB, PRI_MLC_FRAME_BASE );
		
		MES_MLC_SetRGBLayerInvalidPosition( LAYER_DISPLAY_SCREEN_RGB, 0, 0, 0, 0, 0, CFALSE );
		MES_MLC_SetRGBLayerInvalidPosition( LAYER_DISPLAY_SCREEN_RGB, 1, 0, 0, 0, 0, CFALSE );
		
		MES_MLC_SetLayerEnable( LAYER_DISPLAY_SCREEN_RGB, CTRUE );
		MES_MLC_SetDirtyFlag( LAYER_DISPLAY_SCREEN_RGB );
	}
	#endif // PRIMARY MLC
}

///////////////////////////////////////////////////////////////////////////////////////////////
void LcdTurnOn(void)
{
    MES_PWM_SetClockSource (0, PWMSELPLL);
	MES_PWM_SetClockDivisor(0, PWMDIV);		                            //MAX Output Freq 4MHz
	MES_PWM_SetClockPClkMode(MES_PCLKMODE_ALWAYS);
	MES_PWM_SetClockDivisorEnable(CTRUE);		                        // CLKGENENB: Enable

	MES_PWM_SetPreScale(PWM_DISPLAY_LCD_PRI_BRIGHTNESS ,2);				// 4MHz / 2 = 2MHz
	MES_PWM_SetPolarity (PWM_DISPLAY_LCD_PRI_BRIGHTNESS,
							(DISPLAY_LCD_PRI_BIRGHTNESS_ACTIVE) ?
							(enum POL) POL_BYPASS : (enum POL) POL_INVERTED );
	
	MES_PWM_SetPeriod   (PWM_DISPLAY_LCD_PRI_BRIGHTNESS, 50);	        //2MHz /50 = 40KHz
	MES_PWM_SetDutyCycle(PWM_DISPLAY_LCD_PRI_BRIGHTNESS, 35);            //70%, dutycycle value must be smaller then period value
}

#define SET_DELAY_CNT       10
void lcdSetWrite(unsigned char id, unsigned short reg, unsigned short val)
{
	int i;
	
	// START INDEX  
	CS_LOW;
	CountDelay(SET_DELAY_CNT);
	
	for(i=7 ; i>=0 ; i--){
		if( id & (1<<i) )	SDI_HIGH;
		else SDI_LOW;
 		
 		SCL_LOW;
		CountDelay(SET_DELAY_CNT);
		SCL_HIGH;
		CountDelay(SET_DELAY_CNT);
 	}
	
	for(i=15 ; i>=0 ; i--){
		if( reg & (1<<i) )	SDI_HIGH;
		else SDI_LOW;
 			
		SCL_LOW;
		CountDelay(SET_DELAY_CNT);
		SCL_HIGH;
		CountDelay(SET_DELAY_CNT);
	}
	
	// END INDEX 
	SDI_HIGH;
	CountDelay(SET_DELAY_CNT);
	CS_HIGH;
	CountDelay(SET_DELAY_CNT);
	CountDelay(SET_DELAY_CNT);
	CountDelay(SET_DELAY_CNT);

	// START INSTRUCTION
	CS_LOW;
	CountDelay(SET_DELAY_CNT);
	
	id |= 0x02;
	for(i=7 ; i>=0 ; i--)
	{
		if( id & (1<<i) )	SDI_HIGH;
		else SDI_LOW;
 		
 		SCL_LOW;
		CountDelay(SET_DELAY_CNT);
		SCL_HIGH;
		CountDelay(SET_DELAY_CNT);
	}
	
	for(i=15 ; i>=0 ; i--)
	{
		if( val & (1<<i) )	SDI_HIGH;
		else SDI_LOW;
 	
 	    SCL_LOW;
		CountDelay(SET_DELAY_CNT);
		SCL_HIGH;
		CountDelay(SET_DELAY_CNT);
	}
	
	//END INSTRUCTION 
	SDI_HIGH;
	CountDelay(SET_DELAY_CNT);
	CS_HIGH;
	CountDelay(SET_DELAY_CNT);
}

void lcd_spi_write(unsigned char addr, unsigned char data)
{
	int i;
	
	// START addr
	CS_LOW;
	CountDelay(SET_DELAY_CNT);
	
	for(i=7 ; i>=0 ; i--)
	{
		if( addr & (1<<i) )	SDI_HIGH;
		else SDI_LOW;
 		
 		SCL_LOW;
		CountDelay(SET_DELAY_CNT);
		SCL_HIGH;
		CountDelay(SET_DELAY_CNT);
	}
	
	for(i=7 ; i>=0 ; i--)
	{
		if( data & (1<<i) )	SDI_HIGH;
		else SDI_LOW;
 	
 	    SCL_LOW;
		CountDelay(SET_DELAY_CNT);
		SCL_HIGH;
		CountDelay(SET_DELAY_CNT);
	}
	
	//END INSTRUCTION 
	SDI_LOW;
	CountDelay(SET_DELAY_CNT);
	CS_HIGH;
	CountDelay(SET_DELAY_CNT);			
}

///////////////////////////////////////////////////////////////////////////////////////////////
void MES_SetOutputValueBit	 ( U32 PinID, CBOOL bFlag )
{
	if((PinID>>5)!=3){
		U32 oldindex = MES_GPIO_SelectModule(PinID>>5);
		MES_GPIO_SetOutputValue(PinID&0x1F, bFlag);
		MES_GPIO_SelectModule(oldindex);
	}else{
		MES_ALIVE_SetWriteEnable( CTRUE );
		MES_ALIVE_SetAliveGpio( PinID & 0x1F, bFlag );
		MES_ALIVE_SetWriteEnable( CFALSE );
	}
}

int GetInputValue( U32 PinID)
{	
	MES_GPIO_SelectModule(PinID>>5);
	return (int) MES_GPIO_GetInputValue(PinID&0x1F);
}

int fw_check(void)
{
	if(GetInputValue(OEM_GPIO_TEST_KEY) ) return 0;
	else {
		//MES_SetOutputValueBit(OEM_GPIO_SD_ENABLE, CFALSE);
		return 1;
	}
}

void show_Firmware(void)
{
	int x = 0;
	int y = 0;
	
#ifdef CONFIG_LCD_NOINIT	
    InitializeDisplay();	
#endif  /* CONFIG_LCD_NOINIT */

    int img_x = (320-157)/2;
	int img_y = (240-58)/2;
    
    unsigned short *pSrc = imgFirmware;
    unsigned char *fbaddr = (unsigned char*)OEM_MEM_PHY_FRAMEBUFFER_START;
    unsigned char r,g,b;
        
	for(y=0; y<240; y++){
		for(x=0; x<320; x++) {
            if((img_x<x && x<=img_x+157) && (img_y<y && y<=img_y+58)){    
                r = (*pSrc & 0xf800) >> 11;
                g = (*pSrc & 0x7E0) >> 5;
                b = (*pSrc & 0x1f);
                *fbaddr++ = b <<= 3;
                *fbaddr++ = g <<= 2;
                *fbaddr++ = r <<= 3;
                *pSrc ++;
			}
			else{
			    *fbaddr++ = 0;
			    *fbaddr++ = 0; 
			    *fbaddr++ = 0;  
			}
		}
	}
    
#ifdef CONFIG_LCD_NOINIT    
    InitializeLCD();
#endif

}

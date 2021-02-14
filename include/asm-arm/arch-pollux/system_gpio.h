#ifndef __OEM_SYSTEM_GPIO_H__
#define __OEM_SYSTEM_GPIO_H__

//////////////////////////////////////////////////////////////////////////
	///// GROUPA /////
#define PAD_GPIOA0		PAD_ALT1		// Driver : DISPLAY	, Use : PVD0
#define PAD_GPIOA1		PAD_ALT1		// Driver : DISPLAY	, Use : PVD1
#define PAD_GPIOA2		PAD_ALT1		// Driver : DISPLAY	, Use : PVD2
#define PAD_GPIOA3		PAD_ALT1		// Driver : DISPLAY	, Use : PVD3
#define PAD_GPIOA4		PAD_ALT1		// Driver : DISPLAY	, Use : PVD4
#define PAD_GPIOA5		PAD_ALT1		// Driver : DISPLAY	, Use : PVD5
#define PAD_GPIOA6		PAD_ALT1		// Driver : DISPLAY	, Use : PVD6
#define PAD_GPIOA7		PAD_ALT1		// Driver : DISPLAY	, Use : PVD7
#define PAD_GPIOA8		PAD_ALT1		// Driver : UART0	, Use : TX0
#define PAD_GPIOA9		PAD_GPIOOUT		// Driver : MOTOR/SOUND	, Use : HEN/TV_SPK(AMP_EN)
#define PAD_GPIOA10		PAD_GPIOIN		// Driver : POWER	, Use : HOLD_KEY
#define PAD_GPIOA11		PAD_GPIOOUT		// Driver : SOUND	, Use : H/W MUTE CNTL
//#define PAD_GPIOA11		PAD_GPIOIN		// Driver : SOUND	, Use : EARPHONE_DET
#define PAD_GPIOA12		PAD_GPIOIN		// Driver : GPIO   	, Use : TACT_ON 
#define PAD_GPIOA13		PAD_GPIOOUT		// Driver : GPIO	, Use : USB_HOST2_RST 
#define PAD_GPIOA14		PAD_GPIOIN		// Driver : GPIO	, Use : USB_HOST2_INT
#define PAD_GPIOA15		PAD_ALT1		// Driver : UART1	, Use : TX1
#define PAD_GPIOA16		PAD_ALT1		// Driver : UART1	, Use : RX1
#define PAD_GPIOA17		PAD_GPIOIN		// Driver : TOUCH_INT	, Use : PENDOWN DECT
#define PAD_GPIOA18		PAD_GPIOIN		// Driver : POWER	, Use : MAIN_POWER_CTRL
#define PAD_GPIOA19		PAD_GPIOIN		// Driver : SDMMC	, Use : SD_CD
#define PAD_GPIOA20		PAD_GPIOIN		// Driver : SDMMC	, Use : SD_WP
#define PAD_GPIOA21		PAD_ALT1		// Driver : I2S		, Use : I2SDOUT
#define PAD_GPIOA22		PAD_ALT1		// Driver : I2S		, Use : I2SBCLK
#define PAD_GPIOA23		PAD_ALT1		// Driver : I2S		, Use : I2SDIN
#define PAD_GPIOA24		PAD_ALT1		// Driver : I2S		, Use : I2SSYNC
#define PAD_GPIOA25		PAD_ALT1		// Driver : I2S		, Use : I2SMCLK
#define PAD_GPIOA26		PAD_ALT1		// Driver : I2C		, Use : SCL0
#define PAD_GPIOA27		PAD_ALT1		// Driver : I2C		, Use : SDA0
#define PAD_GPIOA28		PAD_ALT1		// Driver : EXT_RTC	, Use : SCL1 
#define PAD_GPIOA29		PAD_ALT1		// Driver : EXT_RTC	, Use : SDA1 
#define PAD_GPIOA30		PAD_ALT1		// Driver : PWM		, Use : PWMOUT0 (LCD BACK)
#define PAD_GPIOA31		PAD_ALT1		// Driver : PWM		, Use : MOTOR 
    
///// GROUPB /////
#define PAD_GPIOB0		PAD_ALT1		// Driver : SDMMC		, Use : SDCLK0, SSPCLK2
#define PAD_GPIOB1		PAD_ALT1		// Driver : SDMMC		, Use : SDCMD0, SSPRXD2
#define PAD_GPIOB2		PAD_ALT1		// Driver : SDMMC		, Use : SDDAT0, SSPTXD2
#define PAD_GPIOB3		PAD_ALT1		// Driver : SDMMC		, Use : SDDAT1
#define PAD_GPIOB4		PAD_ALT1		// Driver : SDMMC		, Use : SDDAT2
#define PAD_GPIOB5		PAD_ALT1		// Driver : SDMMC		, Use : SDDAT3, SSPFRM2
#define PAD_GPIOB6		PAD_GPIOIN		// Driver : POWER	    , Use : POWER_CHK 
#define PAD_GPIOB7		PAD_GPIOOUT		// Driver : GPIO	    , Use : USBHOST2_HI_CNT
#define PAD_GPIOB8		PAD_GPIOOUT		// Driver : DISPLAY	    , Use : LCD_SDI
#define PAD_GPIOB9		PAD_GPIOOUT		// Driver : DISPLAY	    , Use : LCD_SCL
#define PAD_GPIOB10		PAD_GPIOOUT		// Driver : DISPLAY	    , Use : LCD_CS
#define PAD_GPIOB11		PAD_GPIOIN		// Driver : GPIO	    , Use : SELECT
#define PAD_GPIOB12		PAD_GPIOOUT		// Driver : USB         , Use : USB5V_CTRL
#define PAD_GPIOB13		PAD_GPIOIN		// Driver : NOT			, Use : H/W RESET 
#define PAD_GPIOB14		PAD_GPIOIN		// Driver : G_SENSOR	, Use : INTERRUPT
#define PAD_GPIOB15		PAD_GPIOOUT		// Driver : DISPLAY		, Use : LCD_RST
#define PAD_GPIOB16		PAD_ALT1		// Driver : DISPLAY		, Use : PVD8
#define PAD_GPIOB17		PAD_ALT1		// Driver : DISPLAY		, Use : PVD9
#define PAD_GPIOB18		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD10
#define PAD_GPIOB19		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD11
#define PAD_GPIOB20		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD12
#define PAD_GPIOB21		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD13
#define PAD_GPIOB22		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD14
#define PAD_GPIOB23		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD15
#define PAD_GPIOB24		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD16
#define PAD_GPIOB25		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD17
#define PAD_GPIOB26		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD18
#define PAD_GPIOB27		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD19
#define PAD_GPIOB28		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD20
#define PAD_GPIOB29		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD21
#define PAD_GPIOB30		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD22
#define PAD_GPIOB31		PAD_ALT1		// Driver : DISPLAY	    , Use : PVD23

	///// GROUPC /////
#define PAD_GPIOC0		PAD_GPIOIN		// Driver : GPIO		, Use : HOME
#define PAD_GPIOC1		PAD_GPIOIN		// Driver : GPIO		, Use : SELECT1
#define PAD_GPIOC2		PAD_GPIOIN		// Driver : GPIO		, Use : L_SHLDR
#define PAD_GPIOC3		PAD_GPIOIN		// Driver : GPIO		, Use : R_SHLDR
#define PAD_GPIOC4		PAD_GPIOIN		// Driver : GPIO		, Use : A_BTN
#define PAD_GPIOC5		PAD_GPIOIN		// Driver : GPIO		, Use : B_BTN
#define PAD_GPIOC6		PAD_GPIOIN		// Driver : GPIO		, Use : X_BTN
#define PAD_GPIOC7		PAD_GPIOIN      // Driver : GPIO		, Use : Y_BTN
#define PAD_GPIOC8		PAD_GPIOIN      // Driver : NOT			, Use : 
#define PAD_GPIOC9		PAD_GPIOIN      // Driver : NOT			, Use :
#define PAD_GPIOC10		PAD_GPIOIN      // Driver : NOT			, Use : 
#define PAD_GPIOC11		PAD_GPIOOUT     // Driver : TOUCH		, Use : YMON
#define PAD_GPIOC12		PAD_GPIOOUT     // Driver : TOUCH		, Use : NYPON
#define PAD_GPIOC13		PAD_GPIOOUT     // Driver : TOUCH		, Use : XMON
#define PAD_GPIOC14		PAD_GPIOOUT     // Driver : TOUCH		, Use : NXPON
#define PAD_GPIOC15		PAD_GPIOIN		// Driver : USB_DEVICE	, Use : USB_INSERT_CHECK
#define PAD_GPIOC16		PAD_GPIOOUT		// Driver : USB_HOST2.0 , Use : USB_HOST_POWER_ENABLE 
#define PAD_GPIOC17		PAD_GPIOIN		// Driver : VERSION[1]	, Use : BOARD_VERSION(MSB)
#define PAD_GPIOC18		PAD_GPIOIN		// Driver : VERSION[0]	, Use : BOARD_VERSION(LSB)
#define PAD_GPIOC19		PAD_GPIOOUT		// Driver : TOUCH		, Use : PENDOWN_CON
#define PAD_GPIOC20		PAD_ALT1		// Driver : POWER		, Use : VDDPWRTOGGLE
#define PAD_GPIOALV0	PAD_GPIOOUT		// Driver : 			, Use : 
#define PAD_GPIOALV1	PAD_GPIOOUT		// Driver : NAND		, Use : NAND_MLC_WP
#define PAD_GPIOALV2	PAD_GPIOIN		// Driver :         	, Use : 
#define PAD_GPIOALV3	PAD_GPIOOUT		// Driver : MOTOR		, Use : LEN
#define PAD_GPIOALV4	PAD_GPIOOUT		// Driver : SLEEP		, Use : SLEEP_EN 
#define PAD_GPIOALV5	PAD_GPIOOUT		// Driver : LED			, Use : TURN_ON_LED
#define PAD_GPIOALV6	PAD_GPIOOUT		// Driver : LED			, Use : BATT_LIMIT_LED

//GPIO A~C
#define OEM_GPIO_SPK_ENB                        (32*0+9)
#define OEM_GPIO_HMUTE_CNTL                     (32*0+11)
#define OEM_GPIO_MPWR_CTRL						(32*0+18)
#define OEM_GPIO_USB2_RST						(32*0+13)


#define OEM_GPIO_USBH2_CURRENT					(32*1+7)
#define OEM_GPIO_LCD_SDI						(32*1+8)
#define OEM_GPIO_LCD_SCL						(32*1+9)
#define OEM_GPIO_LCD_CS							(32*1+10)
#define OEM_GPIO_USB5V_CTRL						(32*1+12)
#define OEM_GPIO_LCD_ENB              			(32*1+15)

#define OEM_GPIO_TEST_KEY						(32*2+3)
#define OEM_GPIO_USBH2_EN						(32*2+16)
#define OEM_GPIO_BD_NUM_MSB                     (32*2+17)
#define OEM_GPIO_BD_NUM_LSB                     (32*2+18)
        
//GPIOALIVE BIT
#define NAND_WP                                 1
#define SLEEP_EN								4	
#define MOTOR_LEN								3	
#define POWER_ON_LED                            5 
#define BATT_LIMIT_LED                          6


#endif  //__OEM_SYSTEM_GPIO_H__

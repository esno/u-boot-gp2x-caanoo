#ifndef __OEM_SYSTEM_INTERRUPT_H__
#define __OEM_SYSTEM_INTERRUPT_H__

/*************** INTERRUPT LIST ***************/
// Physical interrupt
#define IRQ_PHY_PDISPLAY				0
#define IRQ_PHY_SDISPLAY				1

#define IRQ_PHY_DMA         			3
#define IRQ_PHY_SYSTIMER0   			4
#define IRQ_PHY_SYSCTRL     			5

#define IRQ_PHY_UART0       			10
#define IRQ_PHY_SYSTIMER1   			11
#define IRQ_PHY_SSPSPI0     			12
#define IRQ_PHY_GPIO        			13
#define IRQ_PHY_SDMMC       			14
#define IRQ_PHY_SYSTIMER2   			15

#define IRQ_PHY_UDC         			20
#define IRQ_PHY_SYSTIMER3   			21

#define IRQ_PHY_AUDIOIF					24
#define IRQ_PHY_ADC         			25
#define IRQ_PHY_MCUSTATIC   			26
#define IRQ_PHY_GRP3D       			27
#define IRQ_PHY_UHC         			28

#define IRQ_PHY_RTC         			31
#define IRQ_PHY_I2C0        			32
#define IRQ_PHY_I2C1        			33
#define IRQ_PHY_UART1       			34
#define IRQ_PHY_UART2       			35
#define IRQ_PHY_UART3       			36

#define IRQ_PHY_SSPSPI1     			39

#define IRQ_PHY_CSC         			41
#define IRQ_PHY_SDMMC1       			42
#define IRQ_PHY_SYSTIMER4   			43

// Virtual interrupt
#define IRQ_VIR0            			2

#define IRQ_VIR1            			6
#define IRQ_VIR2            			7
#define IRQ_VIR3            			8
#define IRQ_VIR4            			9

#define IRQ_VIR5            			16
#define IRQ_VIR6            			17
#define IRQ_VIR7            			18
#define IRQ_VIR8            			19

#define IRQ_VIR9            			22
#define IRQ_VIR10            			23

#define IRQ_VIR11            			29
#define IRQ_VIR12            			30

#define IRQ_VIR13            			37
#define IRQ_VIR14            			38

#define IRQ_VIR15            			40

#define IRQ_VIR16            			44
#define IRQ_VIR17            			45
#define IRQ_VIR18           			46
#define IRQ_VIR19           			47
#define IRQ_VIR20           			48
#define IRQ_VIR21           			49
#define IRQ_VIR22           			50
#define IRQ_VIR23           			51
#define IRQ_VIR24           			52
#define IRQ_VIR25           			53
#define IRQ_VIR26           			54
#define IRQ_VIR27           			55
#define IRQ_VIR28           			56
#define IRQ_VIR29           			57
#define IRQ_VIR30           			58
#define IRQ_VIR31           			59
#define IRQ_VIR32           			60
#define IRQ_VIR33           			61
#define IRQ_VIR34           			62
#define IRQ_VIR35           			63
#endif //__OEM_SYSTEM_INTERRUPT_H__

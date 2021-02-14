//  Copyright (C) 2007 MagicEyes Digital Co., All Rights Reserved
//  MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : SD Host Controller
//	File       : mes_sspspi.h
//	Description:
//	Author     :
//	History    :
//------------------------------------------------------------------------------
#ifndef __MES_SDHC_H__
#define __MES_SDHC_H__

#include "../mes_base/mes_prototype.h"

#ifdef  __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
/// @defgroup   SDHC SD Host Controller
//------------------------------------------------------------------------------
//@{

//------------------------------------------------------------------------------
// To remove following waring on RVDS compiler
// 		Warning : #66-D: enumeration value is out of "int" range
//------------------------------------------------------------------------------
#ifdef __arm	
#pragma diag_remark 66		// disable #66 warining
#endif


#define SD_REMOVED  0
#define SD_INSERTED 1

#define WRITE_ENABLED   0
#define WRITE_PROTECTED 1

enum pollux_waitfor {
        COMPLETION_NONE,
        COMPLETION_CMDDONE,
        COMPLETION_XFERFINISH,
        COMPLETION_XFERFINISH_RSPFIN,
};



//------------------------------------------------------------------------------
#define	MES_SDCARD_STATUS_NOERROR			0
#define	MES_SDCARD_STATUS_ERROR				(1UL<<31)
#define	MES_SDCARD_STATUS_CMDBUSY			(MES_SDCARD_STATUS_ERROR | (1UL<<0))
#define	MES_SDCARD_STATUS_CMDTOUT			(MES_SDCARD_STATUS_ERROR | (1UL<<1))
#define	MES_SDCARD_STATUS_RESCRCFAIL		(MES_SDCARD_STATUS_ERROR | (1UL<<2))
#define	MES_SDCARD_STATUS_RESERROR			(MES_SDCARD_STATUS_ERROR | (1UL<<3))
#define	MES_SDCARD_STATUS_RESTOUT			(MES_SDCARD_STATUS_ERROR | (1UL<<4))
#define MES_SDCARD_STATUS_UNKNOWNCMD		(MES_SDCARD_STATUS_ERROR | (1UL<<5))

//------------------------------------------------------------------------------
// Command
#define GO_IDLE_STATE			(0)
#define ALL_SEND_CID			(2)
#define SEND_RELATIVE_ADDR		(3)
#define IO_SEND_OP_COND			(5)
#define SELECT_CARD				(7)
#define SEND_IF_COND			(8)
#define SEND_CSD				(9)
#define SEND_CID				(10)
#define STOP_TRANSMISSION		(12)
#define SEND_STATUS				(13)
#define	SWITCH_FUNC				(6)
#define SET_BLOCKLEN			(16)
#define READ_SINGLE_BLOCK		(17)
#define READ_MULTIPLE_BLOCK		(18)
#define WRITE_BLOCK				(24)
#define WRITE_MULTIPLE_BLOCK	(25)
#define APP_CMD					(55)

// Application Command
#define SET_BUS_WIDTH			((APP_CMD<<8) | 6)
#define SD_STATUS				((APP_CMD<<8) | 13)
#define	SD_SEND_OP_COND			((APP_CMD<<8) | 41)
#define	SET_CLR_CARD_DETECT		((APP_CMD<<8) | 42)
#define SEND_SCR				((APP_CMD<<8) | 51)


//------------------------------------------------------------------------------
/// @brief	SDHC Interrupts for interrupt interfaces
enum	MES_SDHC_INT
{
	MES_SDHC_INT_SDIO	= 16,	///< SDIO Interrupt.\n
								///		Interrupt from SDIO card.
	MES_SDHC_INT_EBE	= 15,	///< End Bit Error for reading, Write no CRC for wrting.\n
								///	 	Error in end-bit during read operations, or no data CRC or negative CRC
								///		received during write operation.\n\n
								///		\b Note> \n
								///		For MMC CMD19, there may be no CRC status returned by the card. 
								///		Hence, MES_SDHC_INT_EBE is set for CMD19. The application should not treat
								///	 	this as an error.
	MES_SDHC_INT_ACD	= 14,	///< Auto Command Done.\n
								///		Stop/abort commands automatically sent by card unit and not initiated by
								///		host; similar to Command Done (MES_SDHC_INT_CD) interrupt.
	MES_SDHC_INT_SBE	= 13,	///< Start Bit Error.\n
								///		Error in data start bit when data is read from a card. In 4-bit mode, if all
								///		data bits do not have start bit, then this error is set.
	MES_SDHC_INT_HLE	= 12,	///< Hardware Locked Error.\n
								///		This interrupt is genearted when the SDHC module cannot load a command
								///	 	issued by the user. When the user sets a command with MES_SDHC_CMDFLAG_STARTCMD
								///		flag, the SDHC module tries to load the command. If the command buffer is
								///		already filled with a command, this error is raised.
								///		And also if the user try to modify any of settings while a command loading is in
								///		progress, then the modification is ignored and the SDHC module generates this
								///		error.
	MES_SDHC_INT_FRUN	= 11,	///< FIFO underrun/overrun Error.\n
								///		Host tried to push data when FIFO was full, or host tried to read data
								///		when FIFO was empty. Typically this should not happen, except due to
								///		error in software.\n
								///		SDHC module never pushes data into FIFO when FIFO is full, and pop data
								///		when FIFO is empty.
	MES_SDHC_INT_HTO	= 10,	///< Data Startvation by Host Timeout.\n
								///		To avoid data loss, SDCLK is stopped if FIFO is empty
								///		when writing to card, or FIFO is full when reading from card. Whenever
								///		SDCLK is stopped to avoid data loss, data-starvation timeout counter
								///		is started with data-timeout value. This interrupt is set if host does not fill
								///		data into FIFO during write to card, or does not read from FIFO during
								///		read from card before timeout period.\n
								///		Even after timeout, SDCLK stays in stopped state with SDHC module waiting.
								///		It is responsibility of host to push or pop data into FIFO upon interrupt, 
								///		which automatically restarts SDCLK and SDHC module.\n
								///		Even if host wants to send stop/abort command, it still needs to ensure it
								///		has to push or pop FIFO so that clock starts in order for stop/abort
								///		command to send on cmd signal along with data that is sent or received
								///		on data line.
	MES_SDHC_INT_DRTO	=  9,	///< Data Read Timeout.\n
								///		Data timeout occurred. Data Transfer Over (MES_SDHC_INT_DTO) also set if data
								///		timeout occurs.
	MES_SDHC_INT_RTO	=  8,	///< Response Timeout.\n
								///		Response timeout occurred. Command Done (MES_SDHC_INT_CD) also set if response
								///		timeout occurs. If command involves data transfer and when response
								///		times out, no data transfer is attempted by SDHC module.
	MES_SDHC_INT_DCRC	=  7,	///< Data CRC Error.\n	
								///		Received Data CRC does not match with locally-generated CRC in SDHC module.
	MES_SDHC_INT_RCRC	=  6,	///< Response CRC Error.\n
								///		Response CRC does not match with locally-generated CRC in SDHC module.
	MES_SDHC_INT_RXDR	=  5,	///< Receive FIFO Data Request.\n
								///		Interrupt set during read operation from card when FIFO level is greater
								///		than Receive-Threshold level.\n\n
								///		\b Recommendation> In DMA modes, this interrupt should not be enabled.\n\n
								///		ISR, in non-DMA mode: \n
								///	@code
								///	pop RX_WMark + 1 data from FIFO
								///	@endcode
	MES_SDHC_INT_TXDR	=  4,	///< Transmit FIFO Data Request.\n
								///		Interrupt set during write operation to card when FIFO level reaches less
								///		than or equal to Transmit-Threshold level.\n\n
								///		\b Recommendation> In DMA modes, this interrupt should not be enabled.\n\n
								///		ISR in non-DMA mode: \n
								///	@code
								///	if (pending_bytes > (16 - TX_WMark))
								///		push (16 - TX_WMark) data into FIFO 
								///	else
								///		push pending_bytes data into FIFO
								///	@endcode
	MES_SDHC_INT_DTO	=  3,	///< Data Transfer Over.\n
								///		Data transfer completed, even if there is Start Bit Error or CRC error.
								///		This bit is also set when "read data-timeout" occurs.\n\n
								///		\b Recommendation> \n
								///		In non-DMA mode, when data is read from card, on seeing interrupt,
								///		host should read any pending data from FIFO.\n
								///		In DMA mode, DMA controllers guarantee FIFO is flushed before interrupt.\n\n
								///		\b Note> \n
								///		MES_SDHC_INT_DTO is set at the end of the last data block, even if
								///		the device asserts MMC busy after the last data block.
	MES_SDHC_INT_CD		=  2,	///< Command Done.\n
								///		Command sent to card and got response from card, even if Response
								///		Error or CRC error occurs. Also set when response timeout occurs.
	MES_SDHC_INT_RE		=  1	///< Response Error.\n
								///		Error in received response set if one of following occurs:
								///		- Transmission bit != 0
								///		- Command index mismatch
								///		- End-bit != 1
};

//------------------------------------------------------------------------------
/// @brief	Command Flags for MES_SDHC_SetCommand()
enum	MES_SDHC_CMDFLAG
{
	MES_SDHC_CMDFLAG_STARTCMD		= 1UL<<31,	///< Start Command. 
												///		Set a command with this flag to update modified settings to the
												///		SDHC module. If this flag is set, you have not to modify any
												///		any settings until the command is taken by the SDHC module.
	MES_SDHC_CMDFLAG_UPDATECLKONLY	= 1UL<<21,	///< Just update clock settings for SDCLK into card clock domain.\n
												///		Changes card clocks (change frequency, truncate off or on, and
												///		set low-frequency mode); provided in order to change clock
												///		frequency or stop clock without having to send command to
												///		cards.\n
												///		Even though this flag is set with MES_SDHC_CMDFLAG_STARTCMD flag,
												///		SDHC module does not issue MES_SDHC_INT_CD signal upon command completion.
	MES_SDHC_CMDFLAG_SENDINIT		= 1UL<<15,	///< Send initialization sequence before sending this command.\n
												///		After power on, 80 clocks must be sent to card for initialization
												///		before sending any commands to card. This flag should be set while
												///		sending first command to card so that controller will initialize
												///		clocks before sending command to card. 
	MES_SDHC_CMDFLAG_STOPABORT		= 1UL<<14,	///< Stop or abort command intended to stop current data transfer in progress.\n
												///		When open-ended or predefined data transfer is in progress, and
												///		host issues stop or abort command to stop data transfer, this flag should
												///		be set so that command/data state-machines of SDHC module can return
												///		correctly to idle state.
	MES_SDHC_CMDFLAG_WAITPRVDAT		= 1UL<<13,	///< Wait for previous data transfer completion before sending command.\n
												///		Command without this flag typically used to query 
												///		status of card during data transfer or to stop current data transfer.
	MES_SDHC_CMDFLAG_SENDAUTOSTOP	= 1UL<<12,	///< Send stop command at end of data transfer.\n
												///		When set, SDHC module sends stop command to card at end of data transfer. \n
												///		* when this flag should be set, since some data
												///		transfers do not need explicit stop commands.\n
												///		* open-ended transfers that software should explicitly send to
												///		stop command.\n
												///		Additionally, when "resume" is sent to resume . suspended
												///		memory access of SD-Combo card . this falg should be set correctly if
												///		suspended data transfer needs send_auto_stop.\n
												///		Don't care if no data expected from card.
	MES_SDHC_CMDFLAG_BLOCK			= 0UL<<11,	///< Block data transfer command.\n Don't care if no data expected from card.
	MES_SDHC_CMDFLAG_STREAM			= 1UL<<11,	///< Stream data transfer command.\n Don't care if no data expected from card.
	MES_SDHC_CMDFLAG_TXDATA			= 3UL<< 9,	///< Write to card.\n Do not set this flag if no data expected from card.
	MES_SDHC_CMDFLAG_RXDATA			= 1UL<< 9,	///< Read form card.\n Do not set this flag if no data expected from card.
	MES_SDHC_CMDFLAG_CHKRSPCRC		= 1UL<< 8,	///< Check response CRC.\n
												///		Some of command responses do not return valid CRC bits.
												///		Software should disable CRC checks for those commands in
												///		order to disable CRC checking by SDHC module.
	MES_SDHC_CMDFLAG_SHORTRSP		= 1UL<< 6,	///< Short response expected from card.\n Do not set this flag if no response is expected from card.
	MES_SDHC_CMDFLAG_LONGRSP		= 3UL<< 6	///< Long response expected from card.\n Do not set this flag if no response is expected from card.
};

//------------------------------------------------------------------------------
/// @brief	Command FSM state for MES_SDHC_GetCommandFSM().
typedef enum
{
	MES_SDHC_CMDFSM_IDLE			=  0,	///< Idle.
	MES_SDHC_CMDFSM_SENDINIT		=  1,	///< Send init sequence.
	MES_SDHC_CMDFSM_TXCMDSTART		=  2,	///< Tx command start bit
	MES_SDHC_CMDFSM_TXCMDTX			=  3,	///< Tx command tx bit.
	MES_SDHC_CMDFSM_TXCMDINDEXARG	=  4,	///< Tx command index + arg.
	MES_SDHC_CMDFSM_TXCMDCRC7		=  5,	///< Tx command CRC7.
	MES_SDHC_CMDFSM_TXCMDEND		=  6,	///< Tx command end bit.
	MES_SDHC_CMDFSM_RXRSPSTART		=  7,	///< Rx response start bit.
	MES_SDHC_CMDFSM_RXRSPIRQ		=  8,	///< Rx response IRQ response.
	MES_SDHC_CMDFSM_RXRSPTX			=  9,	///< Rx response tx bit.
	MES_SDHC_CMDFSM_TXRSPCMDIDX		= 10,	///< Rx response command index.
	MES_SDHC_CMDFSM_RXRSPDATA		= 11,	///< Rx response data.
	MES_SDHC_CMDFSM_RXRSPCRC7		= 12,	///< Rx response CRC7.
	MES_SDHC_CMDFSM_RXRSPEND		= 13,	///< Rx response end bit.
	MES_SDHC_CMDFSM_CMDWAITNCC		= 14,	///< Command path wait NCC.
	MES_SDHC_CMDFSM_WAIT			= 15	///< Wait; Comamnd to Response turnaround.
} MES_SDHC_CMDFSM;	

//------------------------------------------------------------------------------
/// @brief	SDHC Source Clock for Clock Generator.
enum	MES_SDHC_CLKSRC
{
	MES_SDHC_CLKSRC_PCLK	= 0,	///< PCLK.
	MES_SDHC_CLKSRC_PLL0	= 1,	///< PLL0.
	MES_SDHC_CLKSRC_PLL1	= 2		///< PLL1.
};


//------------------------------------------------------------------------------
/// @name	Module Interface
//------------------------------------------------------------------------------
//@{
CBOOL	MES_SDHC_Initialize( void );
U32		MES_SDHC_SelectModule( U32 ModuleIndex );
U32		MES_SDHC_GetCurrentModule( void );
U32		MES_SDHC_GetNumberOfModule( void );
//@}

//------------------------------------------------------------------------------
///  @name	Basic Interface
//------------------------------------------------------------------------------
//@{
U32		MES_SDHC_GetPhysicalAddress( void );
U32		MES_SDHC_GetSizeOfRegisterSet( void );

void	MES_SDHC_SetBaseAddress( U32 BaseAddress );
U32		MES_SDHC_GetBaseAddress( void );

CBOOL	MES_SDHC_OpenModule( void );
CBOOL	MES_SDHC_CloseModule( void );

CBOOL	MES_SDHC_CheckBusy( void );
CBOOL	MES_SDHC_CanPowerDown( void );
//@}

//------------------------------------------------------------------------------
///  @name	Interrupt Interface
//------------------------------------------------------------------------------
S32		MES_SDHC_GetInterruptNumber( void );

void	MES_SDHC_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL	MES_SDHC_GetInterruptEnable( S32 IntNum );
CBOOL	MES_SDHC_GetInterruptPending( S32 IntNum );
void	MES_SDHC_ClearInterruptPending( S32 IntNum );

void	MES_SDHC_SetInterruptEnableAll( CBOOL Enable );
CBOOL	MES_SDHC_GetInterruptEnableAll( void );
CBOOL	MES_SDHC_GetInterruptPendingAll( void );
void	MES_SDHC_ClearInterruptPendingAll( void );

void	MES_SDHC_SetInterruptEnable32( U32 EnableFlag );
U32		MES_SDHC_GetInterruptEnable32( void );
U32 	MES_SDHC_GetInterruptPending32( void );
void   	MES_SDHC_ClearInterruptPending32( U32 PendingFlag );

S32		MES_SDHC_GetInterruptPendingNumber( void );
//@}

//------------------------------------------------------------------------------
///  @name	DMA Interface
//------------------------------------------------------------------------------
//@{
U32		MES_SDHC_GetDMAIndex( void );
U32		MES_SDHC_GetDMABusWidth( void );
//@}

//------------------------------------------------------------------------------
///  @name	Clock Control Interface
//------------------------------------------------------------------------------
//@{
void			MES_SDHC_SetClockPClkMode( MES_PCLKMODE mode );
MES_PCLKMODE	MES_SDHC_GetClockPClkMode( void );

void	MES_SDHC_SetClockSource( U32 Index, U32 ClkSrc );
U32		MES_SDHC_GetClockSource( U32 Index );
void	MES_SDHC_SetClockDivisor( U32 Index, U32 Divisor );
U32		MES_SDHC_GetClockDivisor( U32 Index );
void	MES_SDHC_SetClockDivisorEnable( CBOOL Enable );
CBOOL	MES_SDHC_GetClockDivisorEnable( void );
//@}

//------------------------------------------------------------------------------
///	@name	Module customized operations
//------------------------------------------------------------------------------
//@{
void	MES_SDHC_AbortReadData( void );
void	MES_SDHC_SendIRQResponse( void );
void	MES_SDHC_SetReadWait( CBOOL bAssert );

void	MES_SDHC_SetDMAMode( CBOOL bEnb );

void	MES_SDHC_ResetDMA( void );
CBOOL	MES_SDHC_IsResetDMA( void );
void	MES_SDHC_ResetFIFO( void );
CBOOL	MES_SDHC_IsResetFIFO( void );
void	MES_SDHC_ResetController( void );
CBOOL	MES_SDHC_IsResetController( void );

void	MES_SDHC_SetOutputClockDivider( U32 divider );
U32		MES_SDHC_GetOutputClockDivider( void );
void	MES_SDHC_SetLowPowerClockMode( CBOOL bEnb );
CBOOL	MES_SDHC_GetLowPowerClockMode( void );
void	MES_SDHC_SetOutputClockEnable( CBOOL bEnb );
CBOOL	MES_SDHC_GetOutputClockEnable( void );

void	MES_SDHC_SetDataTimeOut( U32 dwTimeOut );
U32		MES_SDHC_GetDataTimeOut( void );
void	MES_SDHC_SetResponseTimeOut( U32 dwTimeOut );
U32		MES_SDHC_GetResponseTimeOut( void );

void	MES_SDHC_SetDataBusWidth( U32 width );
U32		MES_SDHC_GetDataBusWidth( void );

void	MES_SDHC_SetBlockSize( U32 SizeInByte );
U32		MES_SDHC_GetBlockSize( void );
void	MES_SDHC_SetByteCount( U32 SizeInByte );
U32		MES_SDHC_GetByteCount( void );

void	MES_SDHC_SetCommand( U32 Cmd, U32 flag );
void	MES_SDHC_StartCommand( void );
CBOOL	MES_SDHC_IsCommandBusy( void );
void	MES_SDHC_SetCommandArgument( U32 argument );

U32		MES_SDHC_GetShortResponse( void );
void	MES_SDHC_GetLongResponse( U32 *pLongResponse );
U32		MES_SDHC_GetAutoStopResponse( void );
U32		MES_SDHC_GetResponseIndex( void );

void	MES_SDHC_SetFIFORxThreshold( U32 Threshold );
U32		MES_SDHC_GetFIFORxThreshold( void );
void	MES_SDHC_SetFIFOTxThreshold( U32 Threshold );
U32		MES_SDHC_GetFIFOTxThreshold( void );

U32		MES_SDHC_GetFIFOCount( void );
CBOOL	MES_SDHC_IsFIFOFull( void );
CBOOL	MES_SDHC_IsFIFOEmpty( void );
CBOOL	MES_SDHC_IsFIFOTxThreshold( void );
CBOOL	MES_SDHC_IsFIFORxThreshold( void );

U32		MES_SDHC_GetDataTransferSize( void );
U32		MES_SDHC_GetFIFOTransferSize( void );

void	MES_SDHC_SetData( U32 dwData );
U32		MES_SDHC_GetData( void );
void	MES_SDHC_SetData32( const U32 *pdwData );
void	MES_SDHC_GetData32( U32 *pdwData );
volatile U32*	MES_SDHC_GetDataPointer( void );

CBOOL	MES_SDHC_IsDMAReq( void );
CBOOL	MES_SDHC_IsDMAAck( void );

CBOOL	MES_SDHC_IsDataTransferBusy( void );
CBOOL	MES_SDHC_IsCardDataBusy( void );
CBOOL	MES_SDHC_IsCardPresent( void );

MES_SDHC_CMDFSM	MES_SDHC_GetCommandFSM( void );
//@}


//------------------------------------------------------------------------------
#ifdef __arm	// for RVDS
#pragma diag_default 66		// return to default setting for #66 warning
#endif

//@}
// End of SDHC module
//------------------------------------------------------------------------------

#ifdef  __cplusplus
}
#endif

#endif // __MES_SDHC_H__


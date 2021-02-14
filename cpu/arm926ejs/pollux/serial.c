
#include <common.h>
#include <mes_uart.h>
#include <system.h>

#ifdef CONFIG_DRIVER_POLLUX_SERIAL

void uart_tx_byte(unsigned char c);
int  uart_rx_byte(void);
int  uart_rx_count(void);

void pollux_uart_init(int port)
{
	switch(port)
	{	
		case 0:
			MES_UART_SelectModule( SYSTEM_DEBUG_UART_PORT);	// debug port may be fixed to uart0 :-)
			MES_UART_SetBaseAddress( VIR_BASEADDR_UART0);//MES_UART_GetPhysicalAddress() );
			break;
		case 1:
			MES_UART_SelectModule( 1);	// debug port may be fixed to uart0 :-)
			MES_UART_SetBaseAddress( VIR_BASEADDR_UART1);//MES_UART_GetPhysicalAddress() );
			break;
		case 2:
			MES_UART_SelectModule( 2);	// debug port may be fixed to uart0 :-)
			MES_UART_SetBaseAddress( VIR_BASEADDR_UART2);//MES_UART_GetPhysicalAddress() );
			break;
		case 3:
			MES_UART_SelectModule( 3);	// debug port may be fixed to uart0 :-)
			MES_UART_SetBaseAddress( VIR_BASEADDR_UART3);//MES_UART_GetPhysicalAddress() );
			break;
		default:
	                MES_UART_SelectModule( SYSTEM_DEBUG_UART_PORT);	// debug port may be fixed to uart0 :-)
	                MES_UART_SetBaseAddress( VIR_BASEADDR_UART0);//MES_UART_GetPhysicalAddress() );
			break;
	}
			

	MES_UART_SetClockDivisorEnable( CFALSE );
	MES_UART_SetClockPClkMode( MES_PCLKMODE_ALWAYS );

	MES_UART_OpenModule();

	// UART Mode : Tx, Rx Only
	MES_UART_SetSIRMode( CFALSE );                
	MES_UART_SetLoopBackMode( CFALSE );
	MES_UART_SetAutoFlowControl( CFALSE );
	MES_UART_SetHalfChannelEnable( CTRUE );	// Full or Half

	MES_UART_SetSCTxEnb( CFALSE );
	MES_UART_SetSCRxEnb( CTRUE );

	// Frame Configuration : Data 8 - Parity 0 - Stop 1
	MES_UART_SetFrameConfiguration( MES_UART_PARITYMODE_NONE, 8, 1 );                 

	// Tx Rx Operation : Polling
	MES_UART_SetInterruptEnableAll( CFALSE );
	MES_UART_ClearInterruptPendingAll();
	MES_UART_SetTxIRQType( MES_UART_IRQTYPE_PULSE );
	MES_UART_SetTxOperationMode( MES_UART_OPERMODE_NORMAL );

	MES_UART_SetRxIRQType( MES_UART_IRQTYPE_PULSE );
	MES_UART_SetRxOperationMode( MES_UART_OPERMODE_NORMAL );
	MES_UART_SetRxTimeOutEnb( CFALSE );

	MES_UART_SetSYNCPendClear();

	// FIFO Control
	MES_UART_SetFIFOEnb( CTRUE );
	MES_UART_ResetTxFIFO();
	MES_UART_ResetRxFIFO();

	// UART clock
	MES_UART_SetClockSource(0, UARTSELPLL);
	MES_UART_SetClockDivisor(0, UARTDIV);
	MES_UART_SetBRD( MES_UART_MakeBRD(SYSTEM_DEBUG_UART_BAUD, UARTSRCCLK) );   
	MES_UART_SetClockDivisorEnable( CTRUE );

}

int serial_init(void)
{
	MES_UART_Initialize();
	pollux_uart_init(0);
	pollux_uart_init(1);

	return 0;
}

void serial_putc(const char c)
{
	if ( c == '\r' ) return;

        /* If \n, also do \r */
        if (c == '\n') {
               uart_tx_byte('\r');
	}
	uart_tx_byte(c);
}

int serial_tstc(void)
{
	return uart_rx_count();
}

int serial_getc(void)
{
	return uart_rx_byte();
}

void serial_puts(const char *s)
{
	while(*s) {
		serial_putc(*s++);
	}
}

void serial_setbrg(void)
{
	return;
}

void uart_tx_byte(unsigned char ch)
{
	U32 oldindex;
	
	oldindex = MES_UART_SelectModule( SYSTEM_DEBUG_UART_PORT );

	while( MES_UART_GetFIFOStatus() & MES_UART_FIFOSTAT_TXFULL ) {}

	MES_UART_SendByte( (U8)ch );
	MES_UART_SelectModule( oldindex );
}

int uart_rx_byte(void)
{
	U8  Data = 0;
	U32 oldindex;
	
	oldindex = MES_UART_SelectModule( SYSTEM_DEBUG_UART_PORT );

	if( MES_UART_GetRxFIFOCount() )
	{
		Data = (char)MES_UART_GetByte();
	}
	
	MES_UART_SelectModule( oldindex );
	return (int)Data;
}

int uart_rx_count(void)
{
	U32 rxcount;
	U32 oldindex;
	
	oldindex = MES_UART_SelectModule( SYSTEM_DEBUG_UART_PORT );
	rxcount  = MES_UART_GetRxFIFOCount();

	MES_UART_SelectModule( oldindex );

	return (int)rxcount;
}



void uart1_tx_byte(unsigned char ch)
{
	U32 oldindex;
	
	oldindex = MES_UART_SelectModule( 1 );

	while( MES_UART_GetFIFOStatus() & MES_UART_FIFOSTAT_TXFULL ) {}

	MES_UART_SendByte( (U8)ch );
	MES_UART_SelectModule( oldindex );
}


void serial1_putc(const char c)
{
	if ( c == '\r' ) return;

        /* If \n, also do \r */
        if (c == '\n') {
               uart1_tx_byte('\r');
	}
	uart1_tx_byte(c);
}

void serial1_puts(const char *s)
{
	while(*s) {
		serial1_putc(*s++);
	}
}


#endif

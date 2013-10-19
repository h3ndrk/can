/********************************************\
* !!!!! THIS FILE IS NOT USED ANY MORE !!!!! *
*         NO GUARANTEE FOR ERRORS            *
\********************************************/

int usart_putChar(char c,FILE *stream)
{
	/* NEW LINE CHARACTER */
	if(c == '\n')
	{
		while(!(UCSRA & (1 << UDRE)));
		UDR = '\r';
	}
	/* CHARACTER */
	while(!(UCSRA & (1 << UDRE)));
	UDR = c;
	return 0;
}

int usart_getChar(FILE *stream)
{
	while(!(UCSRA & (1 << RXC)));
	return (int)UDR;
}

#define BAUDRATE 2400UL

void usart_init(void)
{
	uint16_t ubrr = (uint16_t)((uint32_t)F_CPU / (16 * BAUDRATE) - 1);
    UBRRH = (uint8_t)(ubrr>>8);
    UBRRL = (uint8_t)(ubrr);
    
	// enable receiver and trasmitter
	UCSRB = (1<<RXEN) | (1<<TXEN);
	// set frame format: 1stop bit, 8data
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
	
	while(UCSRA & (1 << RXC))
	{
		UDR;
	}
}

/*FILE usartOutput = FDEV_SETUP_STREAM(usart_putChar,NULL,_FDEV_SETUP_WRITE);
FILE usartInput = FDEV_SETUP_STREAM(NULL,usart_getChar,_FDEV_SETUP_READ);*/

/**
 * Read a byte from data bus
 * @return the byte read from data bus
**/
char dataBusRead(void)
{
	char byte = 0;
	for(int i = 0; i < 8; i++)
	{
		// wait for clock
		while(bit_is_clear(PINB,4));
		// save bit
		if(bit_is_set(PINB,0))
		{
			byte |= (1<<i);
		}
		else
		{
			byte &= ~(1<<i);
		}
		// wait for clock
		while(bit_is_set(PINB,4));
	}
	return byte;
}

/**
 * Send a byte to data bus
 * @param byte			the byte to be sended
**/
void dataBusSend(char byte)
{
	_delay_ms(5);
	for(int i = 0; i < 8; i++)
	{
		// set output
		if(byte & (1<<i))
		{
			PORTB |= (1<<2);
		}
		else
		{
			PORTB &= ~(1<<2);
		}
		// clock
		PORTB |= (1<<3);
		_delay_us(500);
		PORTB &= ~(1<<3);
		_delay_us(500);
	}
}

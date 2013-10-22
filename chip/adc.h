void adcInit(void)
{
	uint16_t result;
	
	// enable adc
	ADMUX = (1<<REFS0);					// avcc as reference
	ADCSRA =  (1<<ADPS2) | (1<<ADPS1);	// prescaler to 64
	ADCSRA |= (1<<ADEN);				// enable
	
	// dummy readout
	ADCSRA |= (1<<ADSC);				// start
	while(ADCSRA & (1<<ADSC));			// wait for value
	result = ADCW;						// perform read
}

uint16_t adcRead(uint8_t channel)
{
	uint8_t i;
	uint32_t result = 0;

	// set channel + set avcc as reference
	ADMUX = channel | (1<<REFS0);
	
	// dummy readout
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	// average of 10 times
	for(i = 0; i < 10; i++)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		result += ADCW;
	}
	
	// calculate average
	result /= 100;
	
	return result;
}

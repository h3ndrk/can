/**
 * Perform beep with mikrocontroller
 * @param frequency		frequency for beep
 * @param duration		duration for beep
**/
void beep(uint32_t frequency,uint32_t duration)
{
	for(int i = 0; i < frequency * ((float)duration / 1000.0); i++)
	{
		PORTD |= (1<<5);
		for(int j = 0; j < 220000 / frequency; j++)
			_delay_us(1);
		PORTD &= ~(1<<5);
		for(int j = 0; j < 220000 / frequency; j++)
			_delay_us(1);
	}
}

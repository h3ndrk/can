#define MOTOR_PORT PORTD
#define MOTOR_ENABLE 0
#define MOTOR_DIR_LEFT 1
#define MOTOR_DIR_RIGHT 2
#define MOTOR_CLOCK_LEFT 3
#define MOTOR_CLOCK_RIGHT 4

#include "./include.h"

int main(void)
{
	// data bus input output declaration
	DDRB = 0xFF;		// 1	FAN
						// 2	DATA	A->R
						// 3	CLOCK	A->R
						// 6	XTAL1
						// 7	XTAL2
	
	DDRB &= ~(1<<0);	// 0	DATA	R->A
	DDRB &= ~(1<<4);	// 4	CLOCK	R->A
	DDRB &= ~(1<<5);	// 5	EXTRA	R->A
	
	// motor input output declaration
	DDRD = 0xFF;		// 0	ENABLE
						// 1	DIR		LEFT
						// 2	DIR		RIGHT
						// 3	CLOCK	LEFT
						// 4	CLOCK	RIGHT
						// 5	PIEZO
						// 6	Not Connected
						// 7	Not Connected
	
	// enable fan pwm
	TCCR1A = (1<<COM1A1) | (1<<WGM10) | (1<<WGM11);
	TCCR1B = (1<<WGM12) | (1<<CS12);
	OCR1A = 255;
	
	// disable motors by default
	PORTD |= (1<<MOTOR_ENABLE);
	
	// indicate startup finish with beep
	beep(1500,50);
	
	// initialize adc
	adcInit();
	
	while(1)
	{
		char byte = dataBusRead();
		switch(byte)
		{
			case 1:
			{
				int failure = 0;
				for(int i = 0; i < 100; i++)
				{
					if(dataBusRead() != i)
						failure = 1;
				}
				for(int i = 0; i < 100; i++)
				{
					dataBusSend(i);
				}
				_delay_ms(50);
				if(failure == 0)
					dataBusSend(1);
				else
					dataBusSend(0);
				break;
			}
			case 3:
			{
				OCR1A = (int)(((float)dataBusRead() / 100.0) * 1023.0);
				dataBusSend(1);
				break;
			}
			case 4:
			{
				byte = dataBusRead();
				if(byte == 0)
				{
					PORTD |= (1<<0);
					dataBusSend(0);
				}
				else
				{
					PORTD &= ~(1<<0);
					dataBusSend(1);
				}
				break;
			}
			case 5:
			{
				uint32_t steps = 0;
				char leftDir = 0;
				char rightDir = 0;
				steps = dataBusRead();
				steps += (dataBusRead()<<8);
				steps += (dataBusRead()<<16);
				steps += (dataBusRead()<<24);
				leftDir = dataBusRead();
				rightDir = dataBusRead();
				drive(steps,leftDir,rightDir);
				dataBusSend(1);
				break;
			}
			case 6:
			{
				uint32_t frequency = 0;
				uint32_t duration = 0;
				frequency = dataBusRead();
				frequency += (dataBusRead()<<8);
				frequency += (dataBusRead()<<16);
				frequency += (dataBusRead()<<24);
				duration = dataBusRead();
				duration += (dataBusRead()<<8);
				duration += (dataBusRead()<<16);
				duration += (dataBusRead()<<24);
				beep(frequency,duration);
				dataBusSend(1);
				break;
			}
			case 13:
			{
				unsigned int value = 0;
				value = adcRead(dataBusRead());
				dataBusSend((char)value);
				dataBusSend((char)(value>>8));
				break;
			}
		}
	}
}

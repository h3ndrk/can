/**
 * Drive an amount of steps with the stepper motors
 * @param steps			amount of steps(1000 steps = 1 rotation)
 * @param leftDir		direction of left motor: 0 = reverse, 1 = forward
 * @param rightDir		direction of right motor: 0 = reverse, 1 = forward
**/
void drive(uint32_t steps, char leftDir, char rightDir)
{
	// set direction outputs
	if(leftDir == 0)
		MOTOR_PORT &= ~(1<<MOTOR_DIR_LEFT);
	else
		MOTOR_PORT |= (1<<MOTOR_DIR_LEFT);
	
	if(rightDir == 0)
		MOTOR_PORT |= (1<<MOTOR_DIR_RIGHT);
	else
		MOTOR_PORT &= ~(1<<MOTOR_DIR_RIGHT);
	
	// if steps <= 3000 then no constant speed between speed ramp else speed
	// ramp with constant speed between the ramp
	if(steps <= 3000)
	{
		// speed ramp accelerate
		for(int i = 1; i <= (steps / 2); i+=1)
		{
			MOTOR_PORT |= (1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT);
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
			MOTOR_PORT &= ~((1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT));
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
		}
		// speed ramp brake
		for(int i = (steps / 2); i >= 1; i-=1)
		{
			MOTOR_PORT |= (1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT);
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
			MOTOR_PORT &= ~((1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT));
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
		}
	}
	else
	{
		// speed ramp accelerate
		for(int i = 1; i <= 1500; i+=1)
		{
			MOTOR_PORT |= (1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT);
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
			MOTOR_PORT &= ~((1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT));
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
		}
		// constant speed
		for(int i = 0; i <= (steps - 3000); i+=1)
		{
			MOTOR_PORT |= (1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT);
			_delay_us(160);
			MOTOR_PORT &= ~((1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT));
			_delay_us(160);
		}
		// speed ramp brake
		for(int i = 1500; i >= 1; i-=1)
		{
			MOTOR_PORT |= (1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT);
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
			MOTOR_PORT &= ~((1<<MOTOR_CLOCK_LEFT) | (1<<MOTOR_CLOCK_RIGHT));
			for(int j = 0; j < 1000*(sqrt(i+1)-sqrt(i)); j++)
				_delay_us(1);
		}
	}
}

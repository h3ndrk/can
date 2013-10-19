/**
 * Generate new cartesian point
 * @param x			X-Position
 * @param y			Y-Position
 * @return point as "point_t" with polar coords
**/
point_t generateCartPoint(float x,float y)
{
	point_t point;
	point.x = x;
	point.y = y;
	
	point.angle = 0;
	
	// angle calculation from: http://en.wikipedia.org/wiki/Polar_coordinate_system#Converting_between_polar_and_Cartesian_coordinates
	if(x > 0 && y >= 0)
	{
		point.angle += (180.0 / M_PI) * (atan(y / x));
	}
	else if(x > 0 && y < 0)
	{
		point.angle += (180.0 / M_PI) * (atan(y / x) + 2 * M_PI);
	}
	else if(x < 0)
	{
		point.angle += (180.0 / M_PI) * (atan(y / x) + M_PI);
	}
	else if(x == 0 && y > 0)
	{
		point.angle += (180.0 / M_PI) * (M_PI / 2.0);
	}
	else if(x == 0 && y < 0)
	{
		point.angle += (180.0 / M_PI) * ((3.0 * M_PI) / 2.0);
	}
	
	// radius calculation
	point.radius = sqrt((double)(pow(x,2.0)+pow(y,2.0)));
	
	// angle calculation from: http://en.wikipedia.org/wiki/Polar_coordinate_system#Converting_between_polar_and_Cartesian_coordinates
	if(y >= 0)
	{
		point.angle += (180.0 / M_PI) * acos(x / point.radius);
	}
	else
	{
		point.angle += (180.0 / M_PI) * ((2 * M_PI) - acos(x / point.radius));
	}
	
	// get the average
	point.angle /= 2.0;
	
	return point;
}

/**
 * Generate new polar point
 * @param angle			angle
 * @param radius		radius
 * @return point as "point_t" with cartesian coords
**/
point_t generatePolarPoint(float angle,float radius)
{
	point_t point;
	point.angle = angle;
	point.radius = radius;
	
	// cartesian coord calculation
	point.x = cos((M_PI / 180.0) * angle) * radius;
	point.y = sin((M_PI / 180.0) * angle) * radius;
	
	return point;
}

/**
 * Convert from MM to STEPS (for driving in one direction)
 * @param mm
 * @return converted steps
**/
int convertMmToSteps(float mm)
{
	return mm * (STEPS_PER_ROTATION / (2 * M_PI * WHEEL_RADIUS));
}

/**
 * Convert from STEPS to MM (for driving in one direction)
 * @param steps
 * @return converted mm
**/
float convertStepsToMm(int steps)
{
	return (steps * 2 * M_PI * WHEEL_RADIUS) / STEPS_PER_ROTATION;
}

/**
 * Convert from ANGLE to STEPS (for turning at center point of robot)
 * @param angle
 * @return converted steps
**/
int convertAngleToSteps(float angle)
{
	return convertMmToSteps(M_PI * (angle / 360.0) * WHEEL_DISTANCE);
}

/**
 * Convert from STEPS to ANGLE (for turning at center point of robot)
 * @param steps
 * @return converted angle
**/
float convertStepsToAngle(int steps)
{
	return (360 * convertStepsToMm(steps)) / (M_PI * WHEEL_DISTANCE);
}

/**
 * Save the given position in file
 * @param position	robot-position
**/
void savePosition(position_t position)
{
	FILE *file;
	file = fopen("/usr/share/nginx/html/bin/values/position","w");
	fprintf(file,"%f %f %f",position.x,position.y,position.angle);
	fclose(file);
	if(DEBUG)
		printf("Position saved: ( X: %f | Y: %f | ANGLE: %f )\n",position.x,position.y,position.angle);
}

/**
 * Load the position in file
 * @return	robot-position
**/
position_t loadPosition(void)
{
	position_t position;
	FILE *file;
	file = fopen("/usr/share/nginx/html/bin/values/position","r");
	fscanf(file,"%f %f %f",&position.x,&position.y,&position.angle);
	fclose(file);
	if(DEBUG)
		printf("Position loaded: ( X: %f | Y: %f | ANGLE: %f )\n",position.x,position.y,position.angle);
	return position;
}

/**
 * Reset the position by writing zeros in the file
**/
void resetPosition(void)
{
	position_t position;
	position.x = 0.0;
	position.y = 0.0;
	position.angle = 90.0;
	savePosition(position);
}

/**
 * Calculates the shortest angle for TURN
 * @param curAngle	current robot angle
 * @param newAngle	new robot angle to be driven
 * @return the shortest angle for TURN
**/
float shortestWayForTurn(float curAngle,float newAngle)
{
	return (((newAngle - curAngle) <= 180.0) ? (newAngle - curAngle) : (360 - (newAngle - curAngle)) * (-1));
}

/**
 * Drive to position
 * @param x			X-Position of new coordinate
 * @param y			Y-Position of new coordinate
**/
void driveToPosition(float x, float y)
{
	position_t currentPosition, newPosition;
	float xDelta = 0.0;
	float yDelta = 0.0;
	char commandParamBuffer[255];
	
	if(DEBUG)
		printf("drive to position: ( X: %f | Y: %f )\n",x,y);
	
	// read position
	currentPosition = loadPosition();
	
	// delta between current position and new point
	xDelta = (x - currentPosition.x);
	yDelta = (y - currentPosition.y);
	
	// calculate real driven position
	point_t newPoint = generateCartPoint(xDelta,yDelta);
	newPoint = generatePolarPoint(convertStepsToAngle(convertAngleToSteps(newPoint.angle)),convertStepsToMm(convertMmToSteps(newPoint.radius)));
	
	// save new position
	newPosition.x = newPoint.x + currentPosition.x;
	newPosition.y = newPoint.y + currentPosition.y;
	newPosition.angle = newPoint.angle;
	savePosition(newPosition);
	
	// drive to position
	if(DEBUG)
		printf("TURN: %f degree, %i direction\n",fabs(shortestWayForTurn(currentPosition.angle,newPoint.angle)),((shortestWayForTurn(currentPosition.angle,newPoint.angle) >= 0) ? 1 : 0));
	snprintf(commandParamBuffer,255,"%f %i",fabs(shortestWayForTurn(currentPosition.angle,newPoint.angle)),((shortestWayForTurn(currentPosition.angle,newPoint.angle) >= 0) ? 1 : 0));
	execCommands(9,commandParamBuffer);
	if(DEBUG)
		printf("DRIVE: %f mm\n",newPoint.radius);
	snprintf(commandParamBuffer,255,"%f %i",newPoint.radius,1);
	execCommands(8,commandParamBuffer);
}

/**
 * Drive to angle
 * @param angle		the angle to be driven
**/
void driveToAngle(float angle)
{
	position_t position;
	char commandParamBuffer[255];
	float driveAngle = 0.0;
	
	if(DEBUG)
		printf("turn to angle: ( ANGLE: %f )\n",angle);
	
	// read position
	position = loadPosition();
	
	// calculate driven angle
	driveAngle = convertStepsToAngle(convertAngleToSteps(shortestWayForTurn(position.angle,angle)));
	
	// save new position
	position.angle += driveAngle;
	savePosition(position);
	
	// drive to angle
	if(DEBUG)
		printf("TURN: %f degree, %i direction\n",fabs(driveAngle),(driveAngle >= 0) ? 1 : 0);
	snprintf(commandParamBuffer,255,"%f %i",fabs(driveAngle),(driveAngle >= 0) ? 1 : 0);
	execCommands(9,commandParamBuffer);
}

// This is the header file of "handlerDriving.h" which must be called before
// "handlerCommands.h" to avoid "implicit declaration"-warnings

#define STEPS_PER_ROTATION 1600
#define WHEEL_RADIUS 33.5
#define WHEEL_DISTANCE 138.5

typedef struct point {
	// X-Position of the cartesian coord
	float x;
	// Y-Position of the cartesian coord
	float y;
	// angle of the polar coord
	float angle;
	// radius of the polar coord
	float radius;
} point_t;

// struct of robot-position
typedef struct position {
	// X-Position
	float x;
	// Y-Position
	float y;
	// angle relativ to start
	float angle;
} position_t;

point_t generateCartPoint(float x,float y);

point_t generatePolarPoint(float angle,float radius);

int convertMmToSteps(float mm);

float convertStepsToMm(int steps);

int convertAngleToSteps(float angle);

float convertStepsToAngle(int steps);

void savePosition(position_t position);

position_t loadPosition(void);

void resetPosition(void);

float shortestWayForTurn(float curAngle,float newAngle);

void driveToPosition(float x, float y);

void driveToAngle(float angle);

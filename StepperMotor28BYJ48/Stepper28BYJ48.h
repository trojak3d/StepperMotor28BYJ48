// Stepper28BYJ48.h

#ifndef _Stepper28BYJ48_h
#define _Stepper28BYJ48_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Stepper28BYJ48 {
public:
	Stepper28BYJ48(int pin1, int pin2, int pin3, int pin4);

	void setPins();

	void step(bool isForward);

	void setStepDelay(int value = minStepDelay);

	int getStepDelay();

private:
	// stepper config
	static const int stepsPerStep = 64;
	static const int stepsPerRotation = 64;
	const int totalStepsPerRotation = stepsPerStep * stepsPerRotation;
	const int pole1[8] = {0, 0, 0, 0, 0, 1, 1, 1};//pole1, 8 step values
	const int pole2[8] = {0, 0, 0, 1, 1, 1, 0, 0};//pole2, 8 step values
	const int pole3[8] = {0, 1, 1, 1, 0, 0, 0, 0};//pole3, 8 step values
	const int pole4[8] = {1, 1, 0, 0, 0, 0, 0, 1};//pole4, 8 step values

	int pin1;
	int pin2;
	int pin3;
	int pin4;

	int poleStep = 0;

	static const int minStepDelay = 2;

	int stepDelay = minStepDelay;

	void driveStepper(int c);
};

#endif


#include "Stepper28BYJ48.h"

Stepper28BYJ48::Stepper28BYJ48(int pin1, int pin2, int pin3, int pin4) {
	Serial.print("Creating Stepper");
	this->pin1 = pin1;
	this->pin2 = pin2;
	this->pin3 = pin3;
	this->pin4 = pin4;
	Serial.println("Created Stepper - Pin Sequence (" + String(pin1) + ", " + String(pin2) + ", " + String(pin3) + ", " + String(pin4) + ")");
}

void Stepper28BYJ48::setPins() {
	pinMode(pin1, OUTPUT);//define pin for ULN2003 in1 
	pinMode(pin2, OUTPUT);//define pin for ULN2003 in2   
	pinMode(pin3, OUTPUT);//define pin for ULN2003 in3   
	pinMode(pin4, OUTPUT);//define pin for ULN2003 in4   
}

void Stepper28BYJ48::driveStepper(int c) {
	digitalWrite(pin1, pole1[c]);
	digitalWrite(pin2, pole2[c]);
	digitalWrite(pin3, pole3[c]);
	digitalWrite(pin4, pole4[c]);
}

void Stepper28BYJ48::step(bool isForward) {
	if (isForward) {
		poleStep++;
	} else {
		poleStep--;
	}

	if (poleStep > 7) {
		poleStep = 0;
	}

	if (poleStep < 0) {
		poleStep = 7;
	}

	driveStepper(poleStep);
	delay(stepDelay);
}

void Stepper28BYJ48::setStepDelay(int value) {
	if (stepDelay != value) {
		int oldStepDelay = stepDelay;
		stepDelay = value < minStepDelay ? minStepDelay : value;

		Serial.println("Step Delay: " + String(oldStepDelay) + " -> " + String(stepDelay));
	}
}

int Stepper28BYJ48::getStepDelay() {
	return stepDelay;
}
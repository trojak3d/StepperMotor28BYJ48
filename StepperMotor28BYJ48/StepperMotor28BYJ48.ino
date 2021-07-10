#include "HtmlIndex.h"
#include "Stepper28BYJ48.h"
#include "HtmlIndex.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Stepper28BYJ48.h"

#ifndef STASSID
#define STASSID "StepperMotor"
#define STAPSK  "3dscanner"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

Stepper28BYJ48 stepper = Stepper28BYJ48(D3, D2, D1, D0);

// Default values
float stepAngleDf = 5.625;
int stepAngleStepsDf = 64;
float pauseIntervalDf = 0;
int pauseIntervalMsDf = 0;
int totalAngleDf = 360;
int totalAngleStepsDf = 4096;
int rotationsDf = 1;
int totalStepCountDf = 0;
int intervalStepCountDf = 0;
bool isForwardDf = true;
float startDelayDf = 1;
float startDelayMsDf = 1000;

// Current values
float stepAngle = stepAngleDf;
int stepAngleSteps = stepAngleStepsDf;
float pauseInterval = pauseIntervalDf;
int pauseIntervalMs = pauseIntervalMsDf;
int totalAngle = totalAngleDf;
int totalAngleSteps = totalAngleStepsDf;
int rotations = rotationsDf;
bool isForward = isForwardDf;
float startDelay = startDelayDf;
float startDelayMs = startDelayMsDf;

int totalStepCount = totalStepCountDf;
int intervalStepCount = intervalStepCountDf;

float startDelayMsCountdown = 1000;

bool isRunning = false;

bool isSingleStep = false;
bool isSingleStepDirectionForward = true;
int singleStepSteps = 0;

#pragma region Set Variables

void parameterChanged(String name, String oldValue, String newValue) {
	Serial.println(name + ": " + oldValue + " -> " + newValue);
}

String stepAngleStr() {
	return String(stepAngle) + " (" + String(stepAngleSteps) + " steps)";
}

void setStepAngle(float newStepAngle = stepAngleDf) {
	if (stepAngle != newStepAngle) {
		String oldStepAngle = stepAngleStr();
		stepAngle = newStepAngle;
		stepAngleSteps = (stepAngle / stepAngleDf) * stepAngleStepsDf;
		parameterChanged("Step Angle", oldStepAngle, stepAngleStr());
	}
}

String pauseIntervalStr() {
	return String(pauseInterval) + " (" + String(pauseIntervalMs) + "ms)";
}

void setPauseInterval(float newPauseInterval = pauseIntervalDf) {
	if (pauseInterval != newPauseInterval) {
		String oldPauseInterval = pauseIntervalStr();
		pauseInterval = newPauseInterval;
		pauseIntervalMs = pauseInterval * 1000;
		parameterChanged("Pause Interval", oldPauseInterval, pauseIntervalStr());
	}
}

String totalAngleStr() {
	return String(totalAngle) + " (" + String(totalAngleSteps) + " steps)";
}

void setTotalAngle(int newTotalAngle = totalAngleDf) {
	if (totalAngle != newTotalAngle) {
		String oldTotalAngle = totalAngleStr();
		totalAngle = newTotalAngle;
		totalAngleSteps = (totalAngle / stepAngleDf) * stepAngleStepsDf;
		parameterChanged("Total Angle", oldTotalAngle, totalAngleStr());
	}
}

String rotationsStr() {
	return String(totalAngle) + " (" + String(totalAngleSteps) + " steps)";
}

void setRotations(int newRotations = rotationsDf) {
	if (rotations != newRotations) {
		String oldRotations = rotationsStr();
		rotations = newRotations;
		parameterChanged("Rotations", oldRotations, rotationsStr());
	}
}

String directionStr() {
	return isForward ? "forward" : "backward";
}

void setDirection(bool newForward = isForwardDf) {
	if (isForward != newForward) {
		String oldDirection = directionStr();
		isForward = newForward;
		parameterChanged("Direction", oldDirection, directionStr());
	}
}

String startDelayStr() {
	return String(startDelay) + " (" + String(startDelayMs) + "ms)";
}

void setStartDelay(float newStartDelay = startDelayDf) {
	String oldStartDelay = startDelayStr();
	startDelay = newStartDelay;
	startDelayMs = newStartDelay * 1000;
	startDelayMsCountdown = startDelayMs;
	parameterChanged("Start Delay", oldStartDelay, startDelayStr());
}

void resetDefaults() {
	Serial.println("Reset Defaults:");

	stepper.setStepDelay();
	setStepAngle();
	setPauseInterval();
	setTotalAngle();
	setRotations();
	setDirection();

	Serial.println("Reset Defaults Finished:");
}

void resetStepCount() {
	totalStepCount = totalStepCountDf;
	intervalStepCount = intervalStepCountDf;
}

#pragma endregion

#pragma region Handle http

void handleRoot() {
	String html = GetHtml(
		stepper.getStepDelay(),
		stepAngle,
		pauseInterval,
		totalAngle,
		rotations,
		startDelay,
		isForward
	);

	server.send(200, "text/html", html); //Send web page
}

void start() {
	Serial.println("Start:");
	isRunning = true;
}

void stop() {
	Serial.println("Stop:");
	isRunning = false;
}

void handleForm() {
	bool wasRunning = isRunning;
	stop();

	int newStepDelay = server.arg("step-delay").toInt();
	if (newStepDelay < 2) {
		newStepDelay = 2;
	}

	float newStepAngle = server.arg("step-angle").toFloat();
	float newPauseInterval = server.arg("pause-interval").toFloat();
	int newTotalAngle = server.arg("total-angle").toInt();
	int newRotations = server.arg("rotations").toInt();
	bool newForward = server.arg("forward") == "true";

	if (stepAngle != newStepAngle || totalAngle != newTotalAngle || rotations != newRotations || isForward != newForward) {
		resetStepCount();
	}

	stepper.setStepDelay(newStepDelay);
	setStepAngle(newStepAngle);
	setPauseInterval(newPauseInterval);
	setTotalAngle(newTotalAngle);
	setRotations(newRotations);
	setDirection(newForward);

	if (wasRunning) {
		start();
	}

	handleRoot();
}

void handleStart() {
	float newDelay = server.arg("delay").toFloat();
	setStartDelay(newDelay);

	start();

	handleRoot();
}

void handleReset() {
	bool wasRunning = isRunning && !isSingleStep;
	stop();

	resetDefaults();
	resetStepCount();
	isSingleStep = false;

	if (wasRunning) {
		start();
	}

	handleRoot();
}

void handlePause() {
	stop();

	handleRoot();
}

void handleStop() {
	stop();
	resetStepCount();
	isSingleStep = false;

	handleRoot();
}

void handleStepBackward() {
	stop();
	isSingleStep = true;
	isSingleStepDirectionForward = false;

	singleStepSteps = intervalStepCount > 0 ? intervalStepCount : stepAngleSteps;

	start();

	handleRoot();
}

void handleStepForward() {
	stop();
	isSingleStep = true;
	isSingleStepDirectionForward = true;

	int remainingSteps = stepAngleSteps - intervalStepCount;

	singleStepSteps = remainingSteps > 0 ? remainingSteps : stepAngleSteps;

	start();

	handleRoot();
}

#pragma endregion

void setup(void) {
	Serial.begin(9600);

	WiFi.begin(ssid, password);     //Connect to your WiFi router
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	//If connection successful show IP address in serial monitor
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println("WiFi");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());  //IP address assigned to your ESP

	server.on("/", handleRoot);      //Which routine to handle at root location
	server.on("/motor", handleForm); //form action is handled here
	server.on("/start", handleStart); //form action is handled here
	server.on("/pause", handlePause); //form action is handled here
	server.on("/stop", handleStop); //form action is handled here
	server.on("/reset", handleReset); //form action is handled here
	server.on("/step-backward", handleStepBackward); //form action is handled here
	server.on("/step-forward", handleStepForward); //form action is handled here

	server.begin();                  //Start server
	Serial.println("HTTP server started");

	stepper.setPins();
}

void bootstrap() {
	File file = SPIFFS.open("/bootstrap.min.css.gz", "r");
	size_t sent = server.streamFile(file, "text/css");
}
void popper() {
	File file = SPIFFS.open("/popper.min.js.gz", "r");
	size_t sent = server.streamFile(file, "application/javascript");
}
void bootstrapmin() {
	File file = SPIFFS.open("/bootstrap.min.js.gz", "r");
	size_t sent = server.streamFile(file, "application/javascript");
}

void loop(void) {
	server.handleClient();
	MDNS.update();

	if (!isRunning) {
		delay(1);
		return;
	}

	bool isCurrentDirectionForward = isForward;

	if (isSingleStep) {
		if (singleStepSteps > 0) {
			isCurrentDirectionForward = isSingleStepDirectionForward;
			singleStepSteps--;

			if (isSingleStepDirectionForward) {
				totalStepCount++;
				intervalStepCount++;
			} else {
				totalStepCount--;
				intervalStepCount--;
			}
		} else {
			stop();
			isSingleStep = false;
			intervalStepCount = intervalStepCountDf;
			singleStepSteps = 0;
		}
	} else {
		if (startDelayMsCountdown > 0) {
			startDelayMsCountdown--;
			delay(1);
			return;
		}
		if (totalStepCount > (totalAngleSteps * rotations)) {
			isRunning = false;
			resetStepCount();
			return;
		}

		if (intervalStepCount > stepAngleSteps) {
			intervalStepCount = intervalStepCountDf;
			delay(pauseIntervalMs);
		}

		totalStepCount++;
		intervalStepCount++;
	}

	stepper.step(isCurrentDirectionForward);
}

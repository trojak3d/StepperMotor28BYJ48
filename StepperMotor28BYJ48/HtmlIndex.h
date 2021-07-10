// HtmlIndex.h

#ifndef _HTMLINDEX_h
#define _HTMLINDEX_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

String GetHtml(int stepDelay, float stepAngle, float pauseInterval, int totalAngle, int rotations, int startDelay, bool isForward);

#endif


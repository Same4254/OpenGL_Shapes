#include "Utils/Utils.h"

float Utils_RadiansToDegrees(float radians) {
	return radians * (180.0f / PI);
}

float Utils_DegreesToRadians(float degrees) {
	return degrees * (PI / 180.0f);
}

float Utils_CorrectATan(float x, float y) {
	float temp = 0;
	if (x != 0) {
		temp = atan2f(y, x);
		//if (x < 0)
		//	temp += PI;
	}

	return temp;
}
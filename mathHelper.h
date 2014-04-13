#ifndef MATHHELPER
#define MATHHELPER

#include "vector3.h"
#include "f00quat.h"

namespace MathHelper
{
	inline float Clamp( float value, float minVal, float maxVal )
	{
		if( value < minVal )
		{
			value = minVal;
		}
		else if(value > maxVal )
		{
			value = maxVal;
		}
		return value;
	}

	float rFloatRange(float a, float b);
	inline float Lerp( float fromValue, float toValue, float lerpFactor )
	{
		return fromValue + (toValue - fromValue)*lerpFactor;
	}

	float rFloatRange(float a, float b);
	inline double frac( double val );
	inline float frac( float val );
	inline float minval( float val1, float val2 ) { return val1 < val2 ? val1 : val2; }
	inline float maxval( float val1, float val2 ) { return val1 > val2 ? val1 : val2; }
	inline float Damp( float oldValue, float newValue, float newFactor );	// a big newFactor means less damping
	bool IsPowerOf2( unsigned int i );
	inline float DegToRad( float deg ) { return deg * 3.14159f / 360.0f; }
}

#endif
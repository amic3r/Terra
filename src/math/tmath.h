
#ifndef _included_terra_math_h
#define _included_terra_math_h

#include <math.h>

// Angle conversion

static inline float TMathDegToRad(float degrees)
{ 
	return (float)((double)degrees * (M_PI / 180.0));
}

static inline float TMathRadToDeg(float radian)
{ 
	return (float)((double)radian * (180.0 / M_PI));
}

#include "ttuple.h"
#include "tmatrix.h"

#ifdef TERRA_TRANSFORM
#include "ttransform.h"
#endif

#ifdef TERRA_LINE
#include "tline.h"
#endif

#ifdef TERRA_RECTANGLE
#include "trectangle.h"
#endif

#ifdef TERRA_CUBOID
#include "tcuboid.h"
#endif

#endif

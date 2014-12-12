#include "utility.h"
#include <cmake_config.h>

/* Public variables */

NAMESPACE_INTERNAL_BEGIN

const char * lowerDigits = "0123456789abcdefghijklmnopqrstuvwxyz";
const char * upperDigits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const wchar * wLowerDigits = L"0123456789abcdefghijklmnopqrstuvwxyz";
const wchar * wUpperDigits = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

NAMESPACE_INTERNAL_END


/* Public functions */

#ifndef HAVE_MODFL

//
// Extracts signed integral and fractional values from floating-point number
//
long double modfl(long double x, long double * exp)
{
	static const long double epsilon = 2.2204460492503131e-16L;
	long double z;

	if (x < 0.0L)
	{
		z = 1.0L / epsilon - x - 1.0 / epsilon;
		if (z + x > 0.0L)
			z = z - 1.0L;

		return (*exp = -z) + x;
	}
	else
	{
		z = 1.0L / epsilon + x - 1.0 / epsilon;
		if (z > x)
			z = z - 1.0L;

		return x - (*exp = z);
	}
}

#endif // HAVE_MODFL

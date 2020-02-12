#include "dmkafx.h"
#include "DMK_Math.h"

namespace Dynamik {
	namespace utils {
		// to the (=)pow(base, exponent);
		I64 pow(int base, I16 exp) {
			I64 res = 1;

			for (I16 a = 0; a <= exp; a++)
				res *= base;

			return res;
		}
	}
}
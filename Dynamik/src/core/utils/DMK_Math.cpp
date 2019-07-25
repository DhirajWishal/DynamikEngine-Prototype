#include "dmkafx.h"
#include "DMK_Math.h"

namespace Dynamik {
	namespace utils {
		// to the (=)pow(base, exponent);
		int64 pow(int32 base, int16 exp) {
			int64 res = 1;

			for (int16 a = 0; a <= exp; a++)
				res *= base;

			return res;
		}
	}
}

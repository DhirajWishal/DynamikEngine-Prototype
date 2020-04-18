#include "dmkafx.h"
#include "Sigmoid.h"

namespace Dynamik {
	namespace Math {
		/* SIGMOID FUNCTION */
		F32 Sigmoid(F32 value) {
			return 1 / (1 + exp(value * -1));
		}
	}
}
#pragma once

namespace XEON {
	
	template<typename T>
	T wrap(T val, T lower, T upper) {
		const T range = upper - lower;
		const T dividend = std::floor(((lower - val) / range) + 1);
		return val + range * dividend;
	}

}

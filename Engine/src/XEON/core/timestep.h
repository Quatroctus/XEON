#pragma once

#include <memory>

namespace XEON {

	class Timestep {
	public:
		Timestep(float time = 0.0F) : time(time) {}
	
		operator float() const { return time; }

		float getSeconds() const { return time; }
		float getMilliseconds() const { return time * 1000.0F; }

	private:
		float time;

	};
}

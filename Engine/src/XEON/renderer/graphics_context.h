#pragma once
#include "XEON/core/core.h"


namespace XEON {

	class XEON_API GraphicsContext {
	public:
		virtual ~GraphicsContext() {}

		virtual void init() = 0;
		virtual void swapBuffers() = 0;

		//static GraphicsContext* CreateContext();

	};

}

#pragma once
#include "XEON/core/core.h"
#include "XEON/renderer/graphics_context.h"

struct GLFWwindow;

namespace XEON {	

	class XEON_API OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* window;

	};
	
}

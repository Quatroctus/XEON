#include "xeonpch.h"

#include "XEON/platform/opengl/opengl_context.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace XEON {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : window(window) { }

	void OpenGLContext::init() {
		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		XEON_ASSERT(status, "Failed to initlaize Glad!");
		XEON_INFO("OpenGL Info:");
		XEON_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		XEON_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		XEON_INFO("  Version: {0}", glGetString(GL_VERSION));
	#ifdef XEON_ENABLE_ASSERTS
		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		XEON_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "XEON requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(window);
	}

}

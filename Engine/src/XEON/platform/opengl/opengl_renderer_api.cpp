#include "xeonpch.h"

#include "XEON/platform/opengl/opengl_renderer_api.h"

#include "glad/glad.h"

namespace XEON {

	void OpenGLRendererAPI::init() {
		XEON_PROFILE_FN();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::shutdown() {
		XEON_PROFILE_FN();
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColor(glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
		if (!indexCount) indexCount = vertexArray->getIndexBuffer()->getCount();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

}

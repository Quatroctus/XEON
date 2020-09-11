#pragma once

#include "XEON/renderer/renderer_api.h"

namespace XEON {

	class OpenGLRendererAPI : public RendererAPI {
	public:

		virtual void init() override;
		virtual void shutdown() override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void setClearColor(glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}

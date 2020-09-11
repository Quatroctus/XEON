#pragma once

#include "XEON/renderer/renderer_api.h"

namespace XEON {

	class RenderCommand {
	public:
		inline static void Init() { rendererAPI->init(); }
		inline static void Shutdown() { rendererAPI->shutdown(); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { rendererAPI->setViewport(x, y, width, height); }

		// inline static void RenderCommand::SetClearColor(float r, float g, float b, float a) { rendererAPI->setClearColor(r, g, b, a); }
		// inline static void RenderCommand::SetClearColor(glm::vec3& color) { rendererAPI->setClearColor(color); }
		inline static void RenderCommand::SetClearColor(glm::vec4& color) { rendererAPI->setClearColor(color); }

		inline static void RenderCommand::Clear() { rendererAPI->clear(); }
		// inline static void RenderCommand::Clear(float r, float g, float b, float a) { rendererAPI->clear(r, g, b, a); }
		// inline static void RenderCommand::Clear(glm::vec3& color) { rendererAPI->clear(color); }
		// inline static void RenderCommand::Clear(glm::vec4& color) { rendererAPI->clear(color); }

		inline static void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray) { rendererAPI->drawIndexed(vertexArray); }

	private:
		static Scope<RendererAPI> rendererAPI;

	};

}
#pragma once

#include <memory>

#include "XEON/renderer/vertex_array.h"

#include <glm/glm.hpp>

namespace XEON {

	class RendererAPI {
	public:
		enum class API { NONE = 0, OPENGL = 1, VULKAN = 2, DIRECTX = 3 };

		// inline virtual void setClearColor(float r, float g, float b, float a = 1.0F) { setClearColor(glm::vec4(r, g, b, a)); }

		// inline virtual void setClearColor(const glm::vec3& color) { setClearColor(glm::vec4{ color.r, color.g, color.b, 1.0 }); };

		// virtual void clear(float r, float g, float b, float a = 1.0F) {
		// 	setClearColor(r, g, b, a);
		// 	clear();
		// }

		// virtual void clear(glm::vec3& color) {
		// 	setClearColor(color);
		// 	clear();
		// }

		// virtual void clear(glm::vec4& color) {
		// 	setClearColor(color);
		// 	clear();
		// }

		virtual void init() = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void setClearColor(glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return Api; }

	private:
		static API Api;
	};

}
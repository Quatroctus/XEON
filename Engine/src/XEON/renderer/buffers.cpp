#include "xeonpch.h"

#include "XEON/renderer/buffers.h"
#include "XEON/renderer/renderer.h"
#include "XEON/platform/opengl/opengl_buffers.h"

namespace XEON {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
			case RendererAPI::API::OPENGL: return CreateRef<OpenGLVertexBuffer>(size);
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
			case RendererAPI::API::OPENGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
			case RendererAPI::API::OPENGL: return CreateRef<OpenGLIndexBuffer>(indices, size);
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}


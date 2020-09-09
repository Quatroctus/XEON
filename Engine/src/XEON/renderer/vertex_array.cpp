#include "xeonpch.h"

#include "XEON/renderer/vertex_array.h"
#include "XEON/renderer/renderer.h"

#include "XEON/platform/opengl/opengl_vertex_array.h"

namespace XEON {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
			case RendererAPI::API::OPENGL: return CreateRef<OpenGLVertexArray>();
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

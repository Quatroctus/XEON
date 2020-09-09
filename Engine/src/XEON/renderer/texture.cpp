#include "xeonpch.h"

#include "XEON/renderer/texture.h"
#include "XEON/renderer/renderer.h"

#include "XEON/platform/opengl/opengl_texture.h"

namespace XEON {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
			case RendererAPI::API::OPENGL: return CreateRef<OpenGLTexture2D>(path);
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

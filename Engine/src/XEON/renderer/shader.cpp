#include "xeonpch.h"

#include "XEON/renderer/shader.h"

#include "XEON/renderer/renderer.h"
#include "XEON/platform/opengl/opengl_shader.h"

namespace XEON {

	Ref<Shader> Shader::Create(const std::string& path) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
		case RendererAPI::API::OPENGL: return CreateRef<OpenGLShader>(path);
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::NONE: XEON_ASSERT(false, "RendererAPI:NONE is currently not supported."); return nullptr;
			case RendererAPI::API::OPENGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		XEON_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader) {
		auto& name = shader->getName();
		add(name, shader);
	}

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
		XEON_ASSERT(!exists(name), "Shader with name already exists in library.");
		shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& path) {
		auto shader = Shader::Create(path);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& path, const std::string& name) {
		auto shader = Shader::Create(path);
		add(name, shader);
		return shader;
	}

	bool ShaderLibrary::exists(const std::string& name) const {
		return shaders.find(name) != shaders.end();
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name) {
		XEON_ASSERT(exists(name), "No shader with name exists in library.");
		return shaders[name];
	}

}

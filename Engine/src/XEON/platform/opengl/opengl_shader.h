#pragma once
#include "XEON/core/core.h"

#include "XEON/renderer/shader.h"

// TODO: Remove.
using GLenum = unsigned int;

namespace XEON {
	
	class XEON_API OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;
		
		virtual void uploadUniformFloat(const std::string& name, float f) const override;
		virtual void uploadUniformFloat2(const std::string& name, glm::vec2 vec) const override;
		virtual void uploadUniformFloat3(const std::string& name, glm::vec3 vec) const override;
		virtual void uploadUniformFloat4(const std::string& name, glm::vec4 vec) const override;
		virtual void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const override;
		virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
		virtual void uploadUniformInt(const std::string& name, int i) const override;
/*
		virtual void uploadUniformInt2(const std::string& name, glm::vec<2, int> vec) const override;
		virtual void uploadUniformInt3(const std::string& name, glm::vec<3, int> vec) const override;
		virtual void uploadUniformInt4(const std::string& name, glm::vec<4, int> vec) const override;
		virtual void uploadUniformBool(const std::string& name, bool b) const override;
*/

		virtual const std::string& getName() const override { return name; }

	private:
		std::string name;
		uint32_t rendererID = 0;

		std::string readFile(const std::string& path);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& src);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSrcs);

	};

}


#pragma once
#include "XEON/core/core.h"

#include <string>

#include <glm/glm.hpp>

namespace XEON {

	class XEON_API Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void uploadUniformFloat(const std::string& name, float f) const = 0;
		virtual void uploadUniformFloat2(const std::string& name, glm::vec2 vec) const = 0;
		virtual void uploadUniformFloat3(const std::string& name, glm::vec3 vec) const = 0;
		virtual void uploadUniformFloat4(const std::string& name, glm::vec4 vec) const = 0;
		virtual void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const = 0;
		virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;
		virtual void uploadUniformInt(const std::string& name, int i) const = 0;
/*		
		virtual void uploadUniformInt2(const std::string& name, glm::vec<2, int> vec) const = 0;
		virtual void uploadUniformInt3(const std::string& name, glm::vec<3, int> vec) const = 0;
		virtual void uploadUniformInt4(const std::string& name, glm::vec<4, int> vec) const = 0;
		virtual void uploadUniformBool(const std::string & name, bool b) const = 0;
*/

		virtual const std::string& getName() const = 0;

		static Ref<Shader> Create(const std::string& path);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	};

	class ShaderLibrary {
	public:
		void add(const Ref<Shader>& shader);
		void add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> load(const std::string& path);
		Ref<Shader> load(const std::string& path, const std::string& name);

		bool exists(const std::string& name) const;
		Ref<Shader> get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> shaders;

	};

}


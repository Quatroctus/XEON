#include "xeonpch.h"
#include "XEON/platform/opengl/opengl_shader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace XEON {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		XEON_ASSERT(false, "Unknown Shader Type");
		return 0;
	}

	static GLint createShader(const char* const src, GLuint& shader, GLint shaderType) {
		XEON_PROFILE_FN();

		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &src, 0);
		glCompileShader(shader);
		static GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(shader);
			XEON_ERROR((const char*)infoLog.data());
		}
		return compiled;
	}

	OpenGLShader::OpenGLShader(const std::string& path) {
		XEON_PROFILE_FN();

		compile(preProcess(readFile(path)));
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind(".");
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : name(name) {
		XEON_PROFILE_FN();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		XEON_PROFILE_FN();
		glDeleteProgram(rendererID);
	}

	void OpenGLShader::bind() const {
		XEON_PROFILE_FN();
		glUseProgram(rendererID);
	}

	void OpenGLShader::unbind() const {
		XEON_PROFILE_FN();
		glUseProgram(0);
	}

	void OpenGLShader::setFloat(const std::string& name, float f) const {
		XEON_PROFILE_FN();
		uploadUniformFloat(name, f);
	}
	void OpenGLShader::setFloat2(const std::string& name, glm::vec2 vec) const {
		XEON_PROFILE_FN();
		uploadUniformFloat2(name, vec);
	}
	
	void OpenGLShader::setFloat3(const std::string& name, glm::vec3 vec) const {
		XEON_PROFILE_FN();
		uploadUniformFloat3(name, vec);
	}
	
	void OpenGLShader::setFloat4(const std::string& name, glm::vec4 vec) const {
		XEON_PROFILE_FN();
		uploadUniformFloat4(name, vec);
	}
	
	void OpenGLShader::setMat3(const std::string& name, const glm::mat3& matrix) const {
		XEON_PROFILE_FN();
		uploadUniformMat3(name, matrix);
	}
	
	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix) const {
		XEON_PROFILE_FN();
		uploadUniformMat4(name, matrix);
	}
	
	void OpenGLShader::setInt(const std::string& name, int i) const {
		XEON_PROFILE_FN();
		uploadUniformInt(name, i);
	}
	
	void OpenGLShader::setIntArray(const std::string& name, int* array, uint32_t count) const {
		XEON_PROFILE_FN();
		uploadUniformIntArray(name, array, count);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, float f) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1f(location, f);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, glm::vec2 vec) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform2f(location, vec.x, vec.y);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, glm::vec3 vec) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, glm::vec4 vec) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int i) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1i(location, i);
	}

	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* array, uint32_t count) const {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1iv(location, count, array);
	}

	std::string OpenGLShader::readFile(const std::string& path) {
		XEON_PROFILE_FN();

		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize((unsigned int) in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		} else {
			XEON_ERROR("Could not open file: '{0}'.", path);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& src) {
		XEON_PROFILE_FN();

		std::unordered_map<GLenum, std::string> shaderSources;
		constexpr char* typeToken = "#type";
		constexpr size_t typeTokenLength = 5;
		size_t pos = src.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = src.find_first_of("\r\n", pos);
			XEON_ASSERT(eol != std::string::npos, "Shader Syntax Error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = src.substr(begin, eol - begin);
			GLenum shaderType = ShaderTypeFromString(type);
			XEON_ASSERT(shaderType, "Invalid shader type specification!");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			XEON_ASSERT(nextLinePos != std::string::npos, "Syntax Error!");
			pos = src.find(typeToken, nextLinePos);
			shaderSources[shaderType] = (pos == std::string::npos) ? src.substr(nextLinePos) : src.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSrcs) {
		XEON_PROFILE_FN();

		GLuint program = glCreateProgram();
		int index = 0;
		std::array<GLenum, 3> shaderIDs;
		for (const auto& [key, val] : shaderSrcs) {
			GLuint shader = 0;
			if (createShader(val.c_str(), shader, key)) {
				glAttachShader(program, shader);
				shaderIDs[index++] = shader;
			} else break;
		}
		rendererID = program;

		{
			XEON_PROFILE_SCOPE("Linking Shaders to Program - OpenGLShader");
			glLinkProgram(rendererID);

			GLint isLinked = 0;
			glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			if (!isLinked) {
				GLint maxLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				glDeleteProgram(program);

				for (int i = 0; i < index; i++)
					glDeleteShader(shaderIDs[i]);

				XEON_ERROR("{0}", infoLog.data());
				XEON_ASSERT(false, "Program was unable to link Shaders.");
				return;
			}
			for (int i = 0; i < index; i++) {
				glDetachShader(program, shaderIDs[i]);
				glDeleteShader(shaderIDs[i]);
			}
		}
	}
}

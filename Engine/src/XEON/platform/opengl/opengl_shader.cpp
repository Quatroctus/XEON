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
		compile(preProcess(readFile(path)));
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind(".");
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : name(name) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererID);
	}

	void OpenGLShader::bind() const {
		glUseProgram(rendererID);
	}

	void OpenGLShader::unbind() const {
		glUseProgram(0);
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

	std::string OpenGLShader::readFile(const std::string& path) {
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

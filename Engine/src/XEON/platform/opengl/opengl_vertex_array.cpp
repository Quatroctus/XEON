#include "xeonpch.h"
#include "opengl_vertex_array.h"

#include "glad/glad.h"

namespace XEON {

	static GLenum translateShaderDataType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::FLOAT: return GL_FLOAT;
			case ShaderDataType::FLOAT2: return GL_FLOAT;
			case ShaderDataType::FLOAT3: return GL_FLOAT;
			case ShaderDataType::FLOAT4: return GL_FLOAT;
			case ShaderDataType::MAT3: return GL_FLOAT;
			case ShaderDataType::MAT4: return GL_FLOAT;
			case ShaderDataType::INT: return GL_INT;
			case ShaderDataType::INT2: return GL_INT;
			case ShaderDataType::INT3: return GL_INT;
			case ShaderDataType::INT4: return GL_INT;
			case ShaderDataType::BOOL: return GL_BOOL;
		}
		XEON_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		XEON_PROFILE_FN();
		glCreateVertexArrays(1, &rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		XEON_PROFILE_FN();
		glDeleteVertexArrays(1, &rendererID);
	}

	void OpenGLVertexArray::bind() const {
		XEON_PROFILE_FN();
		glBindVertexArray(rendererID);
		
	}

	void OpenGLVertexArray::unbind() const {
		XEON_PROFILE_FN();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		XEON_PROFILE_FN();

		glBindVertexArray(rendererID);
		vertexBuffer->bind();
		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.getComponentCount(), translateShaderDataType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)element.offset);
			index++;
		}

		vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		XEON_PROFILE_FN();

		glBindVertexArray(rendererID);
		indexBuffer->bind();
		this->indexBuffer = indexBuffer;
	}

	inline const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const { return vertexBuffers; }

	inline const Ref<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const { return indexBuffer; }
}
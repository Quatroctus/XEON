#include "xeonpch.h"

#include "XEON/platform/opengl/opengl_buffers.h"

#include <glad/glad.h>

namespace XEON {
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		XEON_PROFILE_FN();

		glCreateBuffers(1, &rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		XEON_PROFILE_FN();

		glCreateBuffers(1, &rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		XEON_PROFILE_FN();
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLVertexBuffer::bind() const {
		XEON_PROFILE_FN();
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	}

	void OpenGLVertexBuffer::unbind() const {
		XEON_PROFILE_FN();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::setData(const void* data, uint32_t size) {
		XEON_PROFILE_FN();

		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void XEON::OpenGLVertexBuffer::setLayout(const BufferLayout& layout) { this->layout = layout; }

	const BufferLayout& XEON::OpenGLVertexBuffer::getLayout() const { return layout; }

	// -------------------------------------------------------------------------------- //
	// ----------------------------- IndexBuffer Begin -------------------------------- //
	// -------------------------------------------------------------------------------- //

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : count(count) {
		XEON_PROFILE_FN();

		glCreateBuffers(1, &rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		XEON_PROFILE_FN();
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLIndexBuffer::bind() const {
		XEON_PROFILE_FN();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	}

	void OpenGLIndexBuffer::unbind() const {
		XEON_PROFILE_FN();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	inline uint32_t OpenGLIndexBuffer::getCount() const { return count; }

}

#pragma once
#include "XEON/renderer/buffers.h"

namespace XEON {
	
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* indices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		
		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& getLayout() const override;

	private:
		uint32_t rendererID;
		BufferLayout layout;
	};
	
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual inline uint32_t getCount() const override;

	private:
		uint32_t rendererID, count;

	};

}


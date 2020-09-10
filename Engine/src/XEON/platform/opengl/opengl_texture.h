#pragma once
#include "XEON/renderer/texture.h"

#include <glad/glad.h>

namespace XEON {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, void* data=nullptr, uint32_t dataFormat=0, size_t size=0);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual void bind(uint32_t slot = 0) const override;

		virtual void setData(void* data, size_t size) const override;

		virtual uint32_t getWidth() const override { return width; }
		virtual uint32_t getHeight() const override { return height; }

	private:
		const std::string path;
		uint32_t width, height;
		GLenum internalFormat, dataFormat;
		uint32_t rendererID;
	};

}

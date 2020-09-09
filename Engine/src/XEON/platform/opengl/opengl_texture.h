#pragma once
#include "XEON/renderer/texture.h"

namespace XEON {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual void bind(uint32_t slot = 0) const override;

		virtual uint32_t getWidth() const override { return width; }
		virtual uint32_t getHeight() const override { return height; }

	private:
		const std::string path;
		uint32_t width, height;
		uint32_t rendererID;
	};

}

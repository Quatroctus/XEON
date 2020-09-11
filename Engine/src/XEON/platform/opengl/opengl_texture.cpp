#include "xeonpch.h"

#include "XEON/platform/opengl/opengl_texture.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace XEON {
	
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data, uint32_t channels, size_t size) : width(width), height(height) {
		XEON_PROFILE_FN();

		if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		} else if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		XEON_ASSERT(internalFormat & dataFormat, "Format not supported.");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (data) setData(data, size);
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path) {
		XEON_PROFILE_FN();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;
		{
			XEON_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		XEON_ASSERT(data, "Failed to load image!");
		this->width = width;
		this->height = height;

		internalFormat = 0;
		dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		XEON_ASSERT(internalFormat & dataFormat, "Format not supported.");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, this->width, this->height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(rendererID, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		XEON_PROFILE_FN();

		glDeleteTextures(1, &rendererID);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		XEON_PROFILE_FN();
		glBindTextureUnit(slot, rendererID);
	}

	void OpenGLTexture2D::setData(void* data, size_t size) const {
		XEON_PROFILE_FN();
		{
			XEON_PROFILE_SCOPE("Size Assert - OpenGLTexture2D");
			const uint32_t channels = dataFormat == GL_RGB ? 3 : dataFormat == GL_RGBA ? 4 : 1;
			XEON_ASSERT(size == width * height * channels, "Data size does not match data format or width and height.");
		}
		glTextureSubImage2D(rendererID, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

}

#pragma once

#include "XEON/renderer/texture.h"

#include <glm/glm.hpp>

namespace XEON {
	
	class SubTexture2D {
	public:
		SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);
		SubTexture2D(Ref<Texture2D> texture, const glm::vec2& lowLeft, const glm::vec2& lowRight, const glm::vec2& upRight, const glm::vec2& upLeft);

		const glm::vec2* getTexCoords() const { return texCoords; }
		const Ref<Texture2D>& getTexture() const { return texture; }

		static SubTexture2D CreateFromCoords(Ref<Texture2D> texture, const glm::vec2& coords, const glm::vec2& spriteSize);

	private:
		const Ref<Texture2D> texture;
		const glm::vec2 texCoords[4];

	};

}

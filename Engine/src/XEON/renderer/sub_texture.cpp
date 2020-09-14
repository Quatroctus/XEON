#include "xeonpch.h"

#include "XEON/renderer/sub_texture.h"

namespace XEON {
	
	SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max) 
		: texture(texture), texCoords{ min, { max.x, min.y }, max, { min.x, max.y } } { }

	SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& lowLeft, const glm::vec2& lowRight, const glm::vec2& upRight, const glm::vec2& upLeft) 
		: texture(texture), texCoords{ lowLeft, lowRight, upRight, upLeft } { }

	SubTexture2D SubTexture2D::CreateFromCoords(Ref<Texture2D> texture, const glm::vec2& coords, const glm::vec2& spriteSize) {
		float texWidth = (float) texture->getWidth(), texHeight = (float) texture->getHeight();
		return SubTexture2D(texture, { (coords.x * spriteSize.x) / texWidth, (coords.y * spriteSize.y) / texHeight}, { ((coords.x + 1) * spriteSize.x) / texWidth, ((coords.y + 1) * spriteSize.y) / texHeight });
	}

}

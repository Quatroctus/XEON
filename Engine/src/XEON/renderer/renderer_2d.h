#pragma once

#include "XEON/renderer/texture.h"
#include "XEON/renderer/orthographic_camera.h"

namespace XEON {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor=1.0F);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0F);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor = 1.0F);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor = 1.0F);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotatedQuad(glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0F);
		static void DrawRotatedQuad(glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0F);
		static void DrawRotatedQuad(glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor = 1.0F);
		static void DrawRotatedQuad(glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor = 1.0F);
	
	private:
		static void PushQuadVertices(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	};

}

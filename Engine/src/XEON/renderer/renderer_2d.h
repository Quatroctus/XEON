#pragma once

#include "XEON/core/timestep.h"
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

		// Also this performance metric is done on a using OpenGL and a RX 580
		// This increased performance dramatically. We are capable of drawing 250,000 textured quads of the same texture at 60 fps on my computer.
		// I suspect something like 40,000 textured quads at around 60 fps on my 5+ year old laptop.
		/**  Next Generation API.
		 * Simply pushes the construction and maintenance of the glm::mat4 onto the client allowing it to be constructed once or only when a change happens.
		 * This will increase the performance by decreasing the amount of matrix calculations done per frame. 
		 * 
		 * // This is a mock up of what a potential Component for a Renderable would be.
		 * class Renderable2D {
		 *	public:
		 *		
		 *		// Example of a markDirty() recalculate.
		 *		glm::vec4& getPosition() { isDirty = true; return position; }
		 *		const glm::vec4& getPosition() const { return position; }
		 *
		 *		void Draw() {
		 *			if (isDirty) recalculateMatrix();
		 *			Renderer2D::DrawQuad(matrix,...);
		 *		}
		 * 
		 *	private:
		 *		bool isDirty = false;
		 *		glm::vec4 position; // Possibly a glm::vec3.
		 *		glm::vec3 size;		// Possibly a glm::vec2.
		 *		float rotation;
		 *		glm::mat4 matrix;	
		 * 
		 *		void recalculateMatrix();
		 * }
		 */
		// DrawQuad(const glm::mat4& transformMatrix, const glm::vec4 color);
		// DrawQuad(const glm::mat4& transformMatrix, const Ref<Texture>& texture, float tilingFactor=1.0F);
		// DrawQuad(const glm::mat4& transformMatrix, const Ref<SubTexture>& subTexture, float tilingFactor=1.0F);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor = 1.0F);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor = 1.0F);


		// TODO: Contemplate removing these or just keeping them kinda like legacy code or perhaps a use will be found. 
		// It will likely just be the new methods more user friendly but less optimal version.
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
	
		struct Statistics {
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalVertexCount() { return quadCount * 4; }
			uint32_t getTotalIndexCount() { return quadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void PushTexturedQuadVertices(const glm::vec3& position, const glm::vec2& size, uint32_t texIndex, float tilingFactor, const glm::vec4& color = glm::vec4(1.0F));
		
		static void PushRotatedTexturedQuadVertices(const glm::vec3& position, const glm::vec2& size, float rotation, uint32_t texIndex, float tilingFactor, const glm::vec4& color = glm::vec4(1.0F));
	
		static uint32_t GetTextureIndex(const Ref<Texture>& texture);
	};

}

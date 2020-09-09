#pragma once

#include "XEON/renderer/render_command.h"
#include "XEON/renderer/shader.h"
#include "XEON/renderer/orthographic_camera.h"

namespace XEON {

	class Renderer {
	public:
		inline static RendererAPI::API Renderer::GetRendererAPI() { return RendererAPI::GetAPI(); }

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0F));
//		static void Flush();

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static Scope<SceneData> Data;

	};

}

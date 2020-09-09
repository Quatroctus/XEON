#include "xeonpch.h"

#include "XEON/renderer/renderer.h"

namespace XEON {

	Scope<Renderer::SceneData> Renderer::Data = CreateScope<Renderer::SceneData>();

	void Renderer::Init() {
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		Data->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->bind();
		shader->uploadUniformMat4("u_ViewProjection", Data->viewProjectionMatrix);
		shader->uploadUniformMat4("u_Transform", transform);
		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}

#include "xeonpch.h"

#include "XEON/renderer/renderer.h"
#include "XEON/renderer/renderer_2d.h"

namespace XEON {

	Scope<Renderer::SceneData> Renderer::Data = CreateScope<Renderer::SceneData>();

	void Renderer::Init() {
		XEON_PROFILE_FN();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown() {
		XEON_PROFILE_FN();

		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
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
		shader->setMat4("u_ViewProjection", Data->viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);
		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}

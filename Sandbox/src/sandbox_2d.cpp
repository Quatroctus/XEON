#include "xeonpch.h"

#include "sandbox_2d.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0F/9.0F, true) {}

Sandbox2D::~Sandbox2D() {}

void Sandbox2D::onAttach() {
	XEON::Renderer2D::Init();
}

void Sandbox2D::onDetach() {
	XEON::Renderer2D::Shutdown();
}

void Sandbox2D::onUpdate(XEON::Timestep delta) {
	
	cameraController.onUpdate(delta);
	XEON::RenderCommand::SetClearColor(glm::vec4{ 0.1F, 0.1F, 0.1F, 1.0F });
	XEON::RenderCommand::Clear();

	XEON::Renderer2D::BeginScene(cameraController.getCamera());
	{
		XEON::Renderer2D::DrawQuad(glm::vec2{ 0.0F, 0.0F }, glm::vec2{ 0.25F, 0.25F }, squareColor);
	}
	XEON::Renderer2D::EndScene();
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	}
	ImGui::End();
}

void Sandbox2D::onEvent(XEON::Event& e) {
	cameraController.onEvent(e);
}

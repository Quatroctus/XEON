#include "xeonpch.h"

#include "sandbox_2d.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0F/9.0F, true) {}

Sandbox2D::~Sandbox2D() {}

void Sandbox2D::onAttach() {
	XEON_PROFILE_FN();
	texture = XEON::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::onDetach() {
	XEON_PROFILE_FN();
	texture = nullptr;
}

/* Profiling data.
int consistency = 0;
int frames = 0;
float frameTimes = 1.0F / 60.0F;
*/

void Sandbox2D::onUpdate(XEON::Timestep delta) {
	/* Profiling test.
	if (frames++ < 4) return;
	if (delta > frameTimes) {
		APP_TRACE("Timestep({0}) is greater than target({1}) at tile size ({2}, {3}) consistency({4})", delta, frameTimes, tileMapSize.x, tileMapSize.y, consistency);
		if (consistency == 3) {
			frameTimes *= 1.5F;
			APP_TRACE("Consistency threshold met. Switching to {0} seconds.", frameTimes);
			return;
		} else {
			consistency++;
		}
	} else consistency = 0;
	*/
	XEON_PROFILE_FN();

	cameraController.onUpdate(delta);
		
	{
		XEON_PROFILE_SCOPE("Renderer Prep");
		XEON::RenderCommand::SetClearColor(glm::vec4{ 0.1F, 0.1F, 0.1F, 1.0F });
		XEON::RenderCommand::Clear();
	}
	{
		XEON_PROFILE_SCOPE("Renderer Draw");
		XEON::Renderer2D::BeginScene(cameraController.getCamera());
		{
			// Single Square Draw call test.
			/*
			for (float x = 0.0F; x < tileMapSize.x; x++) {
				for (float y = 0.0F; y < tileMapSize.y; y++) {
					XEON::Renderer2D::DrawQuad(glm::vec2{ 0.26F * x, 0.26F * y }, glm::vec2{ 0.25F, 0.25F }, squareColor);
				}
			}
			*/
			XEON::Renderer2D::DrawQuad(glm::vec2{ 1.0F, 1.0F }, glm::vec2{ 1.0F, 1.0F }, texture);
			XEON::Renderer2D::DrawQuad(glm::vec2{ 0.0F, 0.0F }, glm::vec2{ 0.25F, 0.25F }, squareColor);
		}
		XEON::Renderer2D::EndScene();
	}
	// tileMapSize += growthRate * delta;

}

void Sandbox2D::onImGuiRender() {
	XEON_PROFILE_FN();
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	}
	ImGui::End();
}

void Sandbox2D::onEvent(XEON::Event& e) {
	cameraController.onEvent(e);
}

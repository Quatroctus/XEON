#include "xeonpch.h"

#include "sandbox_2d.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0F/9.0F, true) {}

Sandbox2D::~Sandbox2D() {}

template<typename T, typename... Args>
void emplace(T& t, Args&&... args) {
	new(&t) T(std::forward<Args>(args)...);
}

void UpdateFrameData(XEON::Timestep delta) {
	static float frames = 0.0F;
	static float accumulated = 0.0F;
	
	constexpr float fps = 1.0F / 60.0F;
	accumulated += delta;
	frames++;
	if (accumulated >= 1.0F) {
		float framesPS = frames / accumulated * 1.0F;
		XEON_INFO("FPS: {0}", framesPS);
		frames -= framesPS;
		accumulated -= 1.0F;
	}
}

void BatchStress(XEON::Timestep delta, XEON::Ref<XEON::Texture>& texture) {
	UpdateFrameData(delta);
	constexpr uint64_t width = 500;
	constexpr uint64_t size = width * width;
	static bool init = true;
	static glm::vec3 tileMap[size];
	if (init) {
	for (float x = 0; x < (float)width; x++)
		for (float y = 0; y < (float)width; y++)
			emplace(tileMap[(int)y * width + (int)x], x * 0.26F, y * 0.26F, 0.0F);
	init = false;
	}
	constexpr glm::vec2 scale(0.25F);
	for (uint32_t i = 0; i < size; i++) {
		XEON::Renderer2D::DrawQuad(*(tileMap + i), scale, texture);
	}
}

void Sandbox2D::onAttach() {
	XEON_PROFILE_FN();
	texture = XEON::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::onDetach() {
	XEON_PROFILE_FN();
	texture = nullptr;
}

void Sandbox2D::onUpdate(XEON::Timestep delta) {
	XEON_PROFILE_FN();
	cameraController.onUpdate(delta);
	{
		XEON_PROFILE_SCOPE("Renderer Prep");
		XEON::RenderCommand::SetClearColor(glm::vec4{ 0.1F, 0.1F, 0.1F, 1.0F });
		XEON::RenderCommand::Clear();
	}

	XEON::Renderer2D::ResetStats();
	
	{
		XEON_PROFILE_SCOPE("Renderer Draw");
		XEON::Renderer2D::BeginScene(cameraController.getCamera());
		{
			BatchStress(delta, texture);
		}
		XEON::Renderer2D::EndScene();	
	}
}

void Sandbox2D::onImGuiRender() {
	XEON_PROFILE_FN();
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		auto stats = XEON::Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quad Count: %d", stats.quadCount);
		ImGui::Text("\tVertex Count: %d", stats.getTotalVertexCount());
		ImGui::Text("\tIndex Count: %d", stats.getTotalIndexCount());
	}
	ImGui::End();
}

void Sandbox2D::onEvent(XEON::Event& e) {
	cameraController.onEvent(e);
}

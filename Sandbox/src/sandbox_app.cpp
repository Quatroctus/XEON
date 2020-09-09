#include <xeon.h>
#include <XEON/core/entry_point.h>

#include "sandbox_2d.h"

#include <imgui.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class SandboxLayer : public XEON::Layer {
public:
	SandboxLayer() : Layer("Sandbox"), cameraController(16.0F / 9.0F, true) {
		vertexArray = XEON::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5F, -0.5F, 0.0F, 0.0F, 0.0F,
			0.5F, -0.5F, 0.0F, 1.0F, 0.0F,
			0.5F, 0.5F, 0.0F, 1.0F, 1.0F,
			-0.5F, 0.5F, 0.0F, 0.0F, 1.0F
		};
		
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		XEON::Ref<XEON::VertexBuffer> vertexBuffer;
		vertexBuffer = XEON::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		XEON::BufferLayout layout = {
			{ XEON::ShaderDataType::FLOAT3, "a_Position" },
			{ XEON::ShaderDataType::FLOAT2, "a_TexCoord" }
		};
		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);

		XEON::Ref<XEON::IndexBuffer> indexBuffer;
		indexBuffer = XEON::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		vertexArray->setIndexBuffer(indexBuffer);

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}

		)";
		flatColorShader = XEON::Shader::Create("flat_color", flatColorVertexSrc, flatColorFragmentSrc);
		textureShader = XEON::Shader::Create("assets/shaders/texture.glsl");

		texture = XEON::Texture2D::Create("assets/textures/checkerboard.png");
		textureShader->bind();
		textureShader->setInt("u_Texture", 0);
	}

	void onUpdate(XEON::Timestep delta) override {
		cameraController.onUpdate(delta);

		glm::mat4 identity{ 1.0F };
		const int tileMapWidth = 20, tileMapHeight = 20;

		XEON::RenderCommand::SetClearColor(glm::vec4{ 0.1F, 0.1F, 0.1F, 1.0F });
		XEON::RenderCommand::Clear();
		XEON::Renderer::BeginScene(cameraController.getCamera());

		glm::mat4 scale = glm::scale(identity, glm::vec3(this->scale));

		flatColorShader->bind();
		flatColorShader->setFloat3("u_Color", color);
		for (int x = 0; x < tileMapWidth; x++) {
			for (int y = 0; y < tileMapHeight; y++) {
				glm::vec3 pos(x * 0.11F, y * 0.11F, 0.0F);
				glm::mat4 transform = glm::translate(identity, pos) * scale;
				texture->bind();
				XEON::Renderer::Submit(textureShader, vertexArray, transform);
			}
		}
		texture->bind();
		XEON::Renderer::Submit(textureShader, vertexArray, glm::scale(identity, glm::vec3(1.5F)));

		XEON::Renderer::EndScene();
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		{
			ImGui::InputFloat("Scale", &scale, 0.01F, 0.1F, 3);
			ImGui::ColorEdit3("Square Color", glm::value_ptr(color));
		}
		ImGui::End();
	}

	void onEvent(XEON::Event& e) {
		cameraController.onEvent(e);
	}

private:
	//XEON::ShaderLibrary shaderLibrary;
	XEON::Ref<XEON::Shader> flatColorShader, textureShader;
	XEON::Ref<XEON::VertexArray> vertexArray;

	XEON::Ref<XEON::Texture2D> texture;

	XEON::OrthographicCameraController cameraController;

	float scale = 0.1F;
	glm::vec3 color{0.3F, 0.7F, 0.4F};
};

class Sandbox : public XEON::Application {
public:
	Sandbox() : Application({ "Sandbox" }) {
		//pushLayer(new SandboxLayer());
		pushLayer(new Sandbox2D());
	}

	~Sandbox() {}

};

XEON::Application* XEON::CreateApplication() {
	return new Sandbox();
}

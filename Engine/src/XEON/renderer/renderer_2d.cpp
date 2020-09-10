#include "xeonpch.h"

#include "XEON/renderer/renderer_2d.h"

#include "XEON/renderer/shader.h"
#include "XEON/renderer/buffers.h"
#include "XEON/renderer/vertex_array.h"
#include "XEON/renderer/render_command.h"

#include <glm/gtc/matrix_transform.hpp>

namespace XEON {

	struct Renderer2DData {
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
		Ref<Texture> whiteTexture;
	};

	static Renderer2DData* Data;

	void Renderer2D::Init() {
		Data = new Renderer2DData();
		Data->vertexArray = VertexArray::Create();
		
		float squareVertices[20] {
			-0.5F, -0.5F, 0.0F, 0.0F, 0.0F,
			0.5F, -0.5F, 0.0F, 1.0F, 0.0F,
			0.5F, 0.5F, 0.0F, 1.0F, 1.0F,
			-0.5F, 0.5F, 0.0F, 0.0F, 1.0F
		};
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		vertexBuffer->setLayout({
			{ ShaderDataType::FLOAT3, "a_Position" },
			{ ShaderDataType::FLOAT2, "a_TexCoord" }
		});
		Data->vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t squareIndices[6] { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		Data->vertexArray->setIndexBuffer(indexBuffer);

		Data->shader = Shader::Create("assets/shaders/2d_renderer.glsl");
		Data->shader->bind();
		Data->shader->setInt("u_Texture", 0);

		uint32_t color = 0XFFFFFFFF;
		Data->whiteTexture = Texture2D::Create(1, 1, &color, 4, 4);
	}

	void Renderer2D::Shutdown() {
		delete Data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera) {
		Data->shader->bind();
		Data->shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {
//		Data->shader->unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
		DrawQuad(glm::vec3(position, 0.0F), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
		constexpr glm::mat4 identity(1.0F);
		Data->shader->setMat4("u_Transform", 
			glm::translate(identity, position) * glm::rotate(identity, rotation, glm::vec3 { 0.0F, 0.0F, 1.0F }) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		Data->shader->setFloat4("u_Color", color);
		Data->whiteTexture->bind();

		Data->vertexArray->bind();
		RenderCommand::DrawIndexed(Data->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float rotation) {
		DrawQuad(glm::vec3(position, 0.0F), size, texture, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float rotation) {
		constexpr glm::mat4 identity(1.0F);
		Data->shader->setMat4("u_Transform",
			glm::translate(identity, position) * glm::rotate(identity, rotation, glm::vec3{ 0.0F, 0.0F, 1.0F }) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		constexpr glm::vec4 color(1.0F);
		Data->shader->setFloat4("u_Color", color);
		texture->bind(0);
		
		Data->vertexArray->bind();
		RenderCommand::DrawIndexed(Data->vertexArray);
	}

}

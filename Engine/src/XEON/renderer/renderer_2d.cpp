#include "xeonpch.h"

#include "XEON/renderer/renderer_2d.h"

#include "XEON/renderer/shader.h"
#include "XEON/renderer/buffers.h"
#include "XEON/renderer/vertex_array.h"
#include "XEON/renderer/render_command.h"

#include <glm/gtc/matrix_transform.hpp>

namespace XEON {

	struct QuadVertex {
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
	};

	struct QuadBatchData {
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;

		uint32_t indexCount;
		QuadVertex* base = nullptr;
		QuadVertex* ptr = nullptr;
	};

	struct Renderer2DData {
		Ref<Shader> shader;
		Ref<Texture> whiteTexture;
		
		QuadBatchData quadData;
	};

	static Renderer2DData Data;

	void Renderer2D::Init() {
		XEON_PROFILE_FN();

		Data.quadData.vertexArray = VertexArray::Create();
		
		Data.quadData.vertexBuffer = VertexBuffer::Create(Data.quadData.maxVertices * sizeof(QuadVertex));
		Data.quadData.vertexBuffer->setLayout({
			{ ShaderDataType::FLOAT3, "a_Position" },
			{ ShaderDataType::FLOAT4, "a_Color" },
			{ ShaderDataType::FLOAT2, "a_TexCoord" }
		});
		Data.quadData.vertexArray->addVertexBuffer(Data.quadData.vertexBuffer);

		Data.quadData.base = new QuadVertex[Data.quadData.maxVertices];

		uint32_t* quadIndices = new uint32_t[Data.quadData.maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < Data.quadData.maxIndices; i+=6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, Data.quadData.maxIndices);
		Data.quadData.vertexArray->setIndexBuffer(indexBuffer);
		delete[] quadIndices;

		Data.shader = Shader::Create("assets/shaders/2d_renderer.glsl");
		Data.shader->bind();
		Data.shader->setInt("u_Texture", 0);

		uint32_t color = 0xFFFFFFFF;
		Data.whiteTexture = Texture2D::Create(1, 1, &color, 4, 4);
		Data.whiteTexture->bind();
	}

	void Renderer2D::Shutdown() {
		XEON_PROFILE_FN();
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera) {
		XEON_PROFILE_FN();

		Data.quadData.ptr = Data.quadData.base;
		Data.shader->bind();
		Data.shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {
		XEON_PROFILE_FN();

		Flush();
	}

	void Renderer2D::Flush() {
		XEON_PROFILE_FN();

		uint64_t size = (uint8_t*)Data.quadData.ptr - (uint8_t*)Data.quadData.base;
		Data.quadData.vertexBuffer->setData(Data.quadData.base, (uint32_t) size);
		RenderCommand::DrawIndexed(Data.quadData.vertexArray);
		Data.quadData.indexCount = 0;
		Data.quadData.ptr = Data.quadData.base;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(glm::vec3(position, 0.0F), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		XEON_PROFILE_FN();

		PushQuadVertices(position, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor) {
		DrawQuad(glm::vec3(position, 0.0F), size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor) {
		/*
		XEON_PROFILE_FN();

		constexpr glm::mat4 identity(1.0F);
		Data.shader->setMat4("u_Transform",
			glm::translate(identity, position) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		constexpr glm::vec4 color(1.0F);
		Data.shader->setFloat4("u_Color", color);
		Data.shader->setFloat("u_TilingFactor", tilingFactor);
		texture->bind(0);
		
		Data.vertexArray->bind();
		RenderCommand::DrawIndexed(Data.vertexArray);
		*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		DrawQuad(glm::vec3(position, 0.0F), size, texture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		/*
		XEON_PROFILE_FN();

		constexpr glm::mat4 identity(1.0F);
		Data.shader->setMat4("u_Transform",
			glm::translate(identity, position) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		Data.shader->setFloat4("u_Color", tintColor);
		Data.shader->setFloat("u_TilingFactor", tilingFactor);
		texture->bind(0);

		Data.vertexArray->bind();
		RenderCommand::DrawIndexed(Data.vertexArray);
		*/
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotatedQuad(glm::vec3(position, 0.0F), size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		/*
		XEON_PROFILE_FN();

		constexpr glm::mat4 identity(1.0F);
		Data.shader->setMat4("u_Transform",
			glm::translate(identity, position) * glm::rotate(identity, rotation, glm::vec3{ 0.0F, 0.0F, 1.0F }) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		Data.shader->setFloat4("u_Color", color);
		Data.whiteTexture->bind();

		Data.vertexArray->bind();
		RenderCommand::DrawIndexed(Data.vertexArray);
		*/
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor) {
		DrawRotatedQuad(glm::vec3(position, 0.0F), size, rotation, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor) {
		/*
		XEON_PROFILE_FN();

		constexpr glm::mat4 identity(1.0F);
		Data.shader->setMat4("u_Transform",
			glm::translate(identity, position) * glm::rotate(identity, rotation, glm::vec3{0.0F, 0.0F, 1.0F}) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		constexpr glm::vec4 color(1.0F);
		Data.shader->setFloat4("u_Color", color);
		Data.shader->setFloat("u_TilingFactor", tilingFactor);
		texture->bind(0);

		Data.vertexArray->bind();
		RenderCommand::DrawIndexed(Data.vertexArray);
		*/
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		DrawRotatedQuad(glm::vec3(position, 0.0F), size, rotation, texture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		/*
		XEON_PROFILE_FN();

		constexpr glm::mat4 identity(1.0F);
		Data.shader->setMat4("u_Transform",
			glm::translate(identity, position) * glm::rotate(identity, rotation, glm::vec3{ 0.0F, 0.0F, 1.0F }) * glm::scale(identity, glm::vec3(size, 1.0F))
		);
		Data.shader->setFloat4("u_Color", tintColor);
		Data.shader->setFloat("u_TilingFactor", tilingFactor);
		texture->bind(0);

		Data.vertexArray->bind();
		RenderCommand::DrawIndexed(Data.vertexArray);
		*/
	}

	void Renderer2D::PushQuadVertices(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		XEON_PROFILE_FN();

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();

		Data.quadData.ptr->pos = position;
		Data.quadData.ptr->color = color;
		Data.quadData.ptr->texCoord = { 0.0F, 0.0F };
		Data.quadData.ptr++;

		Data.quadData.ptr->pos = { position.x + size.x, position.y, position.z };
		Data.quadData.ptr->color = color;
		Data.quadData.ptr->texCoord = { 1.0F, 0.0F };
		Data.quadData.ptr++;

		Data.quadData.ptr->pos = { position.x + size.x, position.y + size.y, position.z };
		Data.quadData.ptr->color = color;
		Data.quadData.ptr->texCoord = { 1.0F, 1.0F };
		Data.quadData.ptr++;

		Data.quadData.ptr->pos = { position.x, position.y + size.y, position.z };
		Data.quadData.ptr->color = color;
		Data.quadData.ptr->texCoord = { 0.0F, 1.0F };
		Data.quadData.ptr++;

		Data.quadData.indexCount += 6;
	}

}

#include "xeonpch.h"

#include "XEON/renderer/renderer_2d.h"

#include "XEON/renderer/shader.h"
#include "XEON/renderer/buffers.h"
#include "XEON/renderer/vertex_array.h"
#include "XEON/renderer/render_command.h"

#include <glm/gtc/matrix_transform.hpp>

namespace XEON {

	struct QuadVertex {
		glm::mat4 transform;
		glm::vec4 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
		int texIndex;
		float tilingFactor;
	};

	// TODO: Make this batch data manageable by the client.
	struct QuadBatchData {
		const uint32_t maxQuads = 10000; // Seems to shift more burden to the graphics card. However a good balance will be necessary because draw calls are likely to have some performance deficit.
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextures = 32; // The above todo will make this unable to be static.

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;

		uint32_t textureSlotIndex = 1;
		std::unordered_map<Ref<Texture>, uint32_t> textureSlots; // This will likely become a raw pointer instead once the todo is implemented.
		
		uint32_t indexCount;
		QuadVertex* base = nullptr;
		QuadVertex* ptr = nullptr;

		glm::vec4 vertexIdentities[4];
		glm::vec2 textureCoordIdentities[4];
	};

	struct Renderer2DData {
		Renderer2D::Statistics stats;
		Ref<Shader> shader;
		QuadBatchData quadData;
	};

	static Renderer2DData Data;

	void Renderer2D::Init() {
		XEON_PROFILE_FN();

		Data.quadData.vertexArray = VertexArray::Create();
		
		Data.quadData.vertexBuffer = VertexBuffer::Create(Data.quadData.maxVertices * sizeof(QuadVertex));
		Data.quadData.vertexBuffer->setLayout({
			{ ShaderDataType::MAT4, "a_Transform" },
			{ ShaderDataType::FLOAT4, "a_Position" },
			{ ShaderDataType::FLOAT4, "a_Color" },
			{ ShaderDataType::FLOAT2, "a_TexCoord" },
			{ ShaderDataType::INT, "a_TexIndex" },
			{ ShaderDataType::FLOAT, "a_TilingFactor" }
		});

		Data.quadData.vertexArray->addVertexBuffer(Data.quadData.vertexBuffer);

		Data.quadData.base = new QuadVertex[Data.quadData.maxVertices];

		uint32_t* quadIndices = new uint32_t[Data.quadData.maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i + 5 < Data.quadData.maxIndices; i+=6) {
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

		int32_t samplers[QuadBatchData::maxTextures];
		for (int32_t i = 0; i < QuadBatchData::maxTextures; i++) samplers[i] = i;
		Data.shader = Shader::Create("assets/shaders/2d_renderer.glsl");
		Data.shader->bind();
		Data.shader->setIntArray("u_Textures", samplers, QuadBatchData::maxTextures);

		uint32_t color = 0xFFFFFFFF;
		Ref<Texture> whiteTexture = Texture2D::Create(1, 1, &color, 4, 4);
		Data.quadData.textureSlots[whiteTexture] = 0;
		whiteTexture->bind();

		Data.quadData.vertexIdentities[0] = { -0.5, -0.5, 0.0F, 1.0F };
		Data.quadData.vertexIdentities[1] = { 0.5, -0.5, 0.0F, 1.0F };
		Data.quadData.vertexIdentities[2] = { 0.5, 0.5, 0.0F, 1.0F };
		Data.quadData.vertexIdentities[3] = { -0.5, 0.5, 0.0F, 1.0F };

		Data.quadData.textureCoordIdentities[0] = { 0.0F, 0.0F };
		Data.quadData.textureCoordIdentities[1] = { 1.0F, 0.0F };
		Data.quadData.textureCoordIdentities[2] = { 1.0F, 1.0F };
		Data.quadData.textureCoordIdentities[3] = { 0.0F, 1.0F };
	}

	void Renderer2D::Shutdown() {
		XEON_PROFILE_FN();
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera) {
		XEON_PROFILE_FN();

		Data.shader->bind();
		Data.shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

		Data.quadData.indexCount = 0;
		Data.quadData.ptr = Data.quadData.base;
		Data.quadData.textureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		XEON_PROFILE_FN();

		if (Data.quadData.indexCount) Flush();
	}

	void Renderer2D::Flush() {
		XEON_PROFILE_FN();

		if (!Data.quadData.indexCount) return;

		for (auto&[texture, slot] : Data.quadData.textureSlots) {
			if (Data.quadData.indexCount == Data.quadData.maxIndices && slot >= QuadBatchData::maxTextures)
				XEON_TRACE("Texture Slot: {0}", slot);
			texture->bind(slot);
		}
		Data.quadData.textureSlots.clear();

		uint64_t size = (uint8_t*)Data.quadData.ptr - (uint8_t*)Data.quadData.base;
		Data.quadData.vertexBuffer->setData(Data.quadData.base, (uint32_t) size);
		RenderCommand::DrawIndexed(Data.quadData.vertexArray);
		Data.quadData.indexCount = 0;
		Data.quadData.ptr = Data.quadData.base;
		Data.quadData.textureSlotIndex = 1;
		Data.stats.drawCalls++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = transform;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i];
			Data.quadData.ptr->color = color;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = 0;
			Data.quadData.ptr->tilingFactor = 1.0F;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor) {
		uint32_t textureIndex = GetTextureIndex(texture);

		constexpr glm::vec4 color(1.0F);
		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = transform;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i];
			Data.quadData.ptr->color = color;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = textureIndex;
			Data.quadData.ptr->tilingFactor = tilingFactor;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		uint32_t textureIndex = GetTextureIndex(texture);

		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = transform;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i];
			Data.quadData.ptr->color = tintColor;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = textureIndex;
			Data.quadData.ptr->tilingFactor = tilingFactor;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(glm::vec3(position, 0.0F), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		constexpr glm::mat4 identity(1.0F);
		static glm::vec4 scale(0.0F, 0.0F, 1.0F, 1.0F);
		scale.x = size.x; scale.y = size.y;
		
		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = identity;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i] * scale + glm::vec4(position, 0.0F);
			Data.quadData.ptr->color = color;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = 0;
			Data.quadData.ptr->tilingFactor = 1.0F;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor) {
		DrawQuad(glm::vec3(position, 0.0F), size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor) {
		uint32_t textureIndex = GetTextureIndex(texture);
		PushTexturedQuadVertices(position, size, textureIndex, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		DrawQuad(glm::vec3(position, 0.0F), size, texture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		uint32_t textureIndex = GetTextureIndex(texture);
		PushTexturedQuadVertices(position, size, textureIndex, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotatedQuad(glm::vec3(position, 0.0F), size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		constexpr glm::mat4 identity(1.0F);
		constexpr glm::vec3 rotationVec{ 0.0F, 0.0F, 1.0F };
		glm::mat4 transform = glm::translate(identity, position);
		transform = glm::rotate(transform, rotation, rotationVec);
		transform = glm::scale(transform, glm::vec3(size, 1.0F));

		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = transform;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i];
			Data.quadData.ptr->color = color;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = 0;
			Data.quadData.ptr->tilingFactor = 1.0F;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor) {
		DrawRotatedQuad(glm::vec3(position, 0.0F), size, rotation, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor) {
		uint32_t textureIndex = GetTextureIndex(texture);
		PushRotatedTexturedQuadVertices(position, size, rotation, textureIndex, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		DrawRotatedQuad(glm::vec3(position, 0.0F), size, rotation, texture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor, float tilingFactor) {
		uint32_t textureIndex = GetTextureIndex(texture);
		PushRotatedTexturedQuadVertices(position, size, rotation, textureIndex, tilingFactor, tintColor);
	}

	void Renderer2D::ResetStats() {
		std::memset(&Data.stats, 0, sizeof(Statistics));
	}

	XEON::Renderer2D::Statistics Renderer2D::GetStats() {
		return Data.stats;
	}

	void Renderer2D::PushTexturedQuadVertices(const glm::vec3& position, const glm::vec2& size, uint32_t texIndex, float tilingFactor, const glm::vec4& color) {
		constexpr glm::mat4 identity(1.0F);
		static glm::vec4 scale(0.0F, 0.0F, 1.0F, 1.0F);
		scale.x = size.x; scale.y = size.y;

		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = identity;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i] * scale + glm::vec4(position, 0.0F);
			Data.quadData.ptr->color = color;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = texIndex;
			Data.quadData.ptr->tilingFactor = tilingFactor;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	void Renderer2D::PushRotatedTexturedQuadVertices(const glm::vec3& position, const glm::vec2& size, float rotation, uint32_t texIndex, float tilingFactor, const glm::vec4& color) {
		constexpr glm::mat4 identity(1.0F);
		constexpr glm::vec3 rotationVec{0.0F, 0.0F, 1.0F};
		glm::mat4 transform = glm::translate(identity, position);
		transform = glm::rotate(transform, rotation, rotationVec);
		transform = glm::scale(transform, glm::vec3(size, 1.0F));

		for (int i = 0; i < 4; i++) {
			Data.quadData.ptr->transform = transform;
			Data.quadData.ptr->pos = Data.quadData.vertexIdentities[i];
			Data.quadData.ptr->color = color;
			Data.quadData.ptr->texCoord = Data.quadData.textureCoordIdentities[i];
			Data.quadData.ptr->texIndex = texIndex;
			Data.quadData.ptr->tilingFactor = tilingFactor;
			Data.quadData.ptr++;
		}

		Data.quadData.indexCount += 6;
		Data.stats.quadCount++;

		if (Data.quadData.indexCount == Data.quadData.maxIndices) Flush();
	}

	uint32_t Renderer2D::GetTextureIndex(const Ref<Texture>& texture) {
		const auto& val = Data.quadData.textureSlots.find(texture);
		if (val != Data.quadData.textureSlots.end()) return val->second;
		if (Data.quadData.textureSlotIndex == QuadBatchData::maxTextures) {
			Flush();
			Data.quadData.textureSlots[texture] = 1;
			return 1;
		}
		Data.quadData.textureSlots[texture] = Data.quadData.textureSlotIndex;
		return Data.quadData.textureSlotIndex++;
	}

}

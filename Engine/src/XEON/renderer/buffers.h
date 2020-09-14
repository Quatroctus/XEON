#pragma once

namespace XEON {
	
	enum class ShaderDataType : uint8_t {
		NONE = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, MAT3, MAT4, INT, INT2, INT3, INT4, BOOL
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::NONE: return 0;
			case ShaderDataType::FLOAT: return 4;
			case ShaderDataType::FLOAT2: return 8;
			case ShaderDataType::FLOAT3: return 12;
			case ShaderDataType::FLOAT4: return 16;
			case ShaderDataType::MAT3: return 36;
			case ShaderDataType::MAT4: return 64;
			case ShaderDataType::INT: return 4;
			case ShaderDataType::INT2: return 8;
			case ShaderDataType::INT3: return 12;
			case ShaderDataType::INT4: return 16;
			case ShaderDataType::BOOL: return 1;
		}
		XEON_ASSERT(false, "Unknown ShaderDataType.");
		return 0;
	}

	struct BufferElement {
		bool normalized;
		std::string name;
		ShaderDataType type;
		uint32_t size;
		size_t offset;

		BufferElement() : normalized(false), name(""), type(ShaderDataType::NONE), size(0), offset(0) {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = 0)
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) { }

		uint32_t getComponentCount() const {
			switch (type) {
				case ShaderDataType::NONE: return 0;
				case ShaderDataType::FLOAT: return 1;
				case ShaderDataType::FLOAT2: return 2;
				case ShaderDataType::FLOAT3: return 3;
				case ShaderDataType::FLOAT4: return 4;
				case ShaderDataType::INT: return 1;
				case ShaderDataType::INT2: return 2;
				case ShaderDataType::INT3: return 3;
				case ShaderDataType::INT4: return 4;
				case ShaderDataType::MAT3: return 9;
				case ShaderDataType::MAT4: return 16;
				case ShaderDataType::BOOL: return 1;
			}
			XEON_ASSERT(false, "Unknown ShaderDataType.");
			return 0;
		}

	};

	class BufferLayout {
	public:
		BufferLayout() : stride(0) {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) : elements(elements) {
			calculateOffsetsAndStride();
		}

		inline const uint32_t getStride() const { return stride; }

		inline const std::vector<BufferElement>& getElements() const { return elements; }

		inline std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return elements.end(); }

		inline std::vector<BufferElement>::const_iterator begin() const { return elements.cbegin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return elements.cend(); }
	
	private:
		uint32_t stride;
		std::vector<BufferElement> elements;

		void calculateOffsetsAndStride() {
			stride = 0;
			for (auto& element : elements) {
				element.offset = stride;
				stride += element.size;
			}
		}

	};

	class VertexBuffer {
	public:
		using uint32_t = unsigned int;

		virtual ~VertexBuffer() {}
		
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setData(const void* data, uint32_t size) = 0;

		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getLayout() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

	};

	class IndexBuffer {
	public:
		using uint32_t = unsigned int;

		virtual ~IndexBuffer() {}
		
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		inline virtual uint32_t getCount() const = 0;

		//virtual void setData()

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	};

}

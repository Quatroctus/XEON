#pragma once
#include "XEON/core/core.h"

#include <string>
#include <stdint.h>

namespace XEON {

	class Texture {
	public:
		virtual ~Texture() {}

		virtual void bind(uint32_t slot = 0) const = 0;

		virtual void setData(void* data, size_t size) const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
	};

	class Texture2D : public Texture {
	public:
		virtual ~Texture2D() {}

		static Ref<Texture2D> Create(uint32_t width, uint32_t height, void* data=nullptr, uint32_t channels=0, size_t size=0);
		static Ref<Texture2D> Create(const std::string& path);
	};

}

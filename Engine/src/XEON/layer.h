#pragma once

#include "XEON/core.h"
#include "XEON/events/event.h"

namespace XEON {

	class XEON_API Layer {
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& e) {}

		inline const char* getName() const { return debugName; }
	protected:
		const char* debugName;
	};

}

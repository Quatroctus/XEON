#pragma once

#include "XEON/core/core.h"
#include "XEON/events/event.h"
#include "XEON/core/timestep.h"

namespace XEON {

	class XEON_API Layer {
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep delta) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& e) {}

		inline const char* getName() const { return debugName; }
	protected:
		const char* debugName;
	};

}

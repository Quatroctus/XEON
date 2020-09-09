#pragma once
#include "XEON/core/layer.h"

#include "XEON/events/application_event.h"
#include "XEON/events/key_event.h"
#include "XEON/events/mouse_event.h"

namespace XEON {

	class XEON_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

		void begin();
		void end();

	private:
		float time = 0.0F;
	};

}

#pragma once
#include "XEON/layer.h"

#include "XEON/events/application_event.h"
#include "XEON/events/key_event.h"
#include "XEON/events/mouse_event.h"

namespace XEON {

	class XEON_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void onEvent(Event& e) override;
	private:
		float time = 0.0F;
	};

}

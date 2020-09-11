#include "xeonpch.h"

#include "XEON/debug/instrumentor_layer.h"
#include "XEON/debug/instrumentor.h"

#include <imgui.h>

namespace XEON {
	
	void InstrumentorLayer::onUpdate(Timestep delta) {
		Instrumentor& in = Instrumentor::Get();
		if (in.frame) {
			in.log = true;
			in.frame--;
		} else if (in.log && !in.over) in.log = false;
	}

	void InstrumentorLayer::onImGuiRender() {
		ImGui::Begin("Instrumentor Layer");
		{
			Instrumentor& in = Instrumentor::Get();
			ImGui::Checkbox("Override", &in.over);
			ImGui::Checkbox("Log", &in.log);
			ImGui::SliderInt("Frame", (int*) &in.frame, 0, 240);
		}
		ImGui::End();
	}

}

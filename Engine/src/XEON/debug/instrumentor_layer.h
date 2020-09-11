#pragma once

#include "XEON/core/layer.h"

namespace XEON {

	class InstrumentorLayer : public Layer {
	public:
		void onUpdate(Timestep delta) override;
		void onImGuiRender() override;

	};

}

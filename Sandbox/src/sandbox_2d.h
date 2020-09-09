#pragma once

#include "xeon.h"

class Sandbox2D : public XEON::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(XEON::Timestep delta) override;
	virtual void onImGuiRender() override;
	virtual void onEvent(XEON::Event& e) override;

private:
	XEON::Ref<XEON::VertexArray> vertexArray;
	XEON::Ref<XEON::Shader> flatColorShader;

	XEON::OrthographicCameraController cameraController;

	glm::vec4 squareColor { 0.2F, 0.3F, 0.8F, 1.0F };

};


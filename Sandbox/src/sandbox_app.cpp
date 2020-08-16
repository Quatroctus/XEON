#include <xeon.h>

class SandboxLayer : public XEON::Layer {
public:
	SandboxLayer() : Layer("Sandbox") {}

	void onUpdate() override {
		if (XEON::Input::isKeyPressed(XEON_KEY_SPACE))
			APP_INFO("Space bar is Pressed.");
	}

	void onEvent(XEON::Event& e) {}

};

class Sandbox : public XEON::Application {
public:
	Sandbox() : Application({"Sandbox"}) {
		pushLayer(new SandboxLayer());
		pushOverlay(new XEON::ImGuiLayer());
	}

	~Sandbox() {}

};

XEON::Application* XEON::CreateApplication() {
	return new Sandbox();
}

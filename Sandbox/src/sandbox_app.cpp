#include <xeon.h>

class SandboxLayer : public XEON::Layer {
public:
	SandboxLayer() : Layer("Sandbox") {}

	void onUpdate() override {
		APP_TRACE("SandboxLayer::onUpdate");
	}

	void onEvent(XEON::Event& e) {
		APP_TRACE("{0}", e);
	}

};

class Sandbox : public XEON::Application {
public:
	Sandbox() : Application({"Sandbox"}) {
		pushLayer(new SandboxLayer());
	}

	~Sandbox() {}

};

XEON::Application* XEON::CreateApplication() {
	return new Sandbox();
}

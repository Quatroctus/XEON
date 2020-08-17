#include <xeon.h>

#include <imgui.h>

class SandboxLayer : public XEON::Layer {
public:
	SandboxLayer() : Layer("Sandbox") {}

	void onUpdate() override {
		if (XEON::Input::isKeyPressed(XEON_KEY_SPACE))
			APP_INFO("Space bar is Pressed.");
	}

	void onImGuiRender() override {
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void onEvent(XEON::Event& e) {}

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

#include "xeonpch.h"
#include "XEON/imgui/imgui_layer.h"
#include "XEON/application.h"

#include "XEON/platform/opengl/imgui_opengl_renderer.h"

// TEMP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

XEON::ImGuiLayer::ImGuiLayer() : Layer("ImGui") {}

XEON::ImGuiLayer::~ImGuiLayer() {}

void XEON::ImGuiLayer::onAttach() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	//TEMPORARY
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	ImGui_ImplOpenGL3_Init("#version 410");
}

void XEON::ImGuiLayer::onDetach() {
	
}

void XEON::ImGuiLayer::onUpdate() {
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());
	float time = (float)glfwGetTime();
	io.DeltaTime = this->time > 0.0 ? (time - this->time) : 1.0F / 60.0F;
	this->time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#define BIND_IMGUI_GLFW_EVENT_FN(x) std::bind(&x, nullptr, std::placeholders::_1)

void XEON::ImGuiLayer::onEvent(Event& e) {
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<MouseButtonPressedEvent>([](MouseButtonPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getButton()] = true;
		return false;
	});
	dispatcher.dispatch<MouseButtonReleasedEvent>([](MouseButtonReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getButton()] = false;
		return false;
	});
	dispatcher.dispatch<MouseMovedEvent>([](MouseMovedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.getX(), e.getY());
		return false;
	});
	dispatcher.dispatch<MouseScrolledEvent>([](MouseScrolledEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.getXOffset();
		io.MouseWheel += e.getYOffset();
		return false;
	});
	dispatcher.dispatch<WindowResizeEvent>([](WindowResizeEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
		io.DisplayFramebufferScale = ImVec2(1.0F, 1.0F);
		glViewport(0, 0, e.getWidth(), e.getHeight());
		return false;
	});

	dispatcher.dispatch<KeyPressedEvent>([](KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	});
	dispatcher.dispatch<KeyReleasedEvent>([](KeyReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = false;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	});
	dispatcher.dispatch<KeyTypedEvent>([](KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keyCode = e.getKeyCode();
		if (keyCode > 0 && keyCode < 0x10000) io.AddInputCharacter((unsigned short) keyCode);
		return false;
	});
}

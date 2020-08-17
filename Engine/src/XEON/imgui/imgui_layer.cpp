#include "xeonpch.h"
#include "XEON/imgui/imgui_layer.h"

#include <imgui.h>

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "XEON/application.h"

// TEMP
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace XEON {

	ImGuiLayer::ImGuiLayer() : Layer("ImGui") {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::onAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0F;
			style.Colors[ImGuiCol_WindowBg].w = 1.0F;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::onDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float) app.getWindow().getWidth(), (float) app.getWindow().getHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}
	}

	void ImGuiLayer::onImGuiRender() {
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	//void ImGuiLayer::onUpdate() {
	//	ImGuiIO& io = ImGui::GetIO();
	//	Application& app = Application::Get();
	//	io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());
	//	float time = (float)glfwGetTime();
	//	io.DeltaTime = this->time > 0.0 ? (time - this->time) : 1.0F / 60.0F;
	//	this->time = time;
	//
	//	ImGui_ImplOpenGL3_NewFrame();
	//	ImGui::NewFrame();
	//
	//	static bool show = true;
	//	ImGui::ShowDemoWindow(&show);
	//	ImGui::Render();
	//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//}

	//#define BIND_IMGUI_GLFW_EVENT_FN(x) std::bind(&x, nullptr, std::placeholders::_1)
	//
	//void ImGuiLayer::onEvent(Event& e) {
	//	EventDispatcher dispatcher(e);
	//	dispatcher.dispatch<MouseButtonPressedEvent>([](MouseButtonPressedEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.MouseDown[e.getButton()] = true;
	//		return false;
	//	});
	//	dispatcher.dispatch<MouseButtonReleasedEvent>([](MouseButtonReleasedEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.MouseDown[e.getButton()] = false;
	//		return false;
	//	});
	//	dispatcher.dispatch<MouseMovedEvent>([](MouseMovedEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.MousePos = ImVec2(e.getX(), e.getY());
	//		return false;
	//	});
	//	dispatcher.dispatch<MouseScrolledEvent>([](MouseScrolledEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.MouseWheelH += e.getXOffset();
	//		io.MouseWheel += e.getYOffset();
	//		return false;
	//	});
	//	dispatcher.dispatch<WindowResizeEvent>([](WindowResizeEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
	//		io.DisplayFramebufferScale = ImVec2(1.0F, 1.0F);
	//		glViewport(0, 0, e.getWidth(), e.getHeight());
	//		return false;
	//	});
	//
	//	dispatcher.dispatch<KeyPressedEvent>([](KeyPressedEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.KeysDown[e.getKeyCode()] = true;
	//		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//		return false;
	//	});
	//	dispatcher.dispatch<KeyReleasedEvent>([](KeyReleasedEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		io.KeysDown[e.getKeyCode()] = false;
	//		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//		return false;
	//	});
	//	dispatcher.dispatch<KeyTypedEvent>([](KeyTypedEvent& e) {
	//		ImGuiIO& io = ImGui::GetIO();
	//		int keyCode = e.getKeyCode();
	//		if (keyCode > 0 && keyCode < 0x10000) io.AddInputCharacter((unsigned short) keyCode);
	//		return false;
	//	});
}
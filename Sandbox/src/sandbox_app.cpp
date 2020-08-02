#include <xeon.h>

class Sandbox : public XEON::Application {
public:
	Sandbox() : Application({"Sandbox"}) {}

	~Sandbox() {}

};

XEON::Application* XEON::CreateApplication() {
	return new Sandbox();
}

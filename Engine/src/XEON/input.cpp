#include "xeonpch.h"
#include "XEON/input.h"

namespace XEON {

	int Input::mouseX;
	int Input::mouseY;
	std::unordered_map<int, int> Input::KeyStates;
	std::bitset<8> Input::MouseStates;

	void Input::setKeyRepeatCount(int keyCode, int repeatCount) { Input::KeyStates.insert_or_assign(keyCode, repeatCount); }
	int Input::getKeyRepeatCount(int keyCode) { return Input::KeyStates.at(keyCode); }
	bool Input::isKeyPressed(int keyCode) { return Input::KeyStates.find(keyCode) != Input::KeyStates.end() ? Input::KeyStates[keyCode] != -1 : false; }
	void Input::setMouseButtonState(int button, bool state) { Input::MouseStates.set(button, state); }
	bool Input::getMouseButtonState(int button) { return Input::MouseStates.test(button); }

	int Input::getMouseX() { return Input::mouseX; }
	int Input::getMouseY() { return Input::mouseY; }

	void Input::setMouseX(int x) { Input::mouseX = x; }
	void Input::setMouseY(int y) { Input::mouseY = y; }
	void Input::setMousePosition(int x, int y) { Input::mouseX = x; Input::mouseY = y; }

}

#pragma once

#include "XEON/core.h"
#include <bitset>
#include <unordered_map>

namespace XEON {

	class XEON_API Input {
	public:
		
		inline static void setKeyRepeatCount(int keyCode, int repeatCount);
		inline static int getKeyRepeatCount(int keyCode);
		inline static bool isKeyPressed(int keyCode);
		inline static bool getMouseButtonState(int button);
		inline static void setMouseButtonState(int button, bool state);
		inline static int getMouseX();
		inline static int getMouseY();
		inline static void setMouseX(int x);
		inline static void setMouseY(int y);
		inline static void setMousePosition(int x, int y);

	private:
		static int mouseX;
		static int mouseY;
		static std::unordered_map<int, int> KeyStates;
		static std::bitset<8> MouseStates;
	};

}

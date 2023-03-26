#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GlM/glm.hpp>

export module Mouse;

namespace Mouse {

	export int x = 0, y = 0;
	export int lastX = 0, lastY = 0;

	export bool clicked = false, released = false;

	export void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			clicked = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			released = true;
		}
	}

	export void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
		x = xpos;
		y = ypos;
	}

};
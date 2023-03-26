#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GlM/glm.hpp>

export module Util;

export struct vec2 {
	GLfloat x, y;
};

export struct vec4 {
	GLfloat x, y, z, w;
};

export float map(float value, float start1, float stop1, float start2, float stop2) {
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

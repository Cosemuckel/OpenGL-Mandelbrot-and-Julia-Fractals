#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GlM/glm.hpp>

export module Theme;

namespace Theme {
	
#define C(x) (x / 255.0f)

	export GLfloat brown[16][4]{
		{ C(66), C(30), C(15), 1.f},
		{ C(25), C(7), C(26), 1.f},
		{ C(9), C(1), C(47), 1.f},
		{ C(4), C(4), C(73), 1.f},
		{ C(0), C(7), C(100), 1.f},
		{ C(12), C(44), C(138), 1.f},
		{ C(24), C(82), C(177), 1.f},
		{ C(57), C(125), C(209), 1.f},
		{ C(134), C(181), C(229), 1.f},
		{ C(211), C(236), C(248), 1.f},
		{ C(241), C(233), C(191), 1.f},
		{ C(248), C(201), C(95), 1.f},
		{ C(255), C(170), C(0), 1.f},
		{ C(204), C(128), C(0), 1.f},
		{ C(153), C(87), C(0), 1.f},
		{ C(106), C(52), C(3), 1.f}
	};
	
};
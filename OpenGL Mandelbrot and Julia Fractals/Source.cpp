#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GlM/glm.hpp>

import Fractal;
import Mouse;

int screenWidth = 720;
int screenHeight = 720;

void resizeCallback(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* description) {
	std::cout << "Encountered opengl error " << error << ": " << description << "\n";
}

int main() {
	
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Mandelbrot and Julia Fractals", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetErrorCallback(errorCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPositionCallback);

	glEnable(GL_DEPTH_TEST);
	glClearColor(.2f, .3f, .3f, 1.f);

	Fractal fractal = Fractal(screenWidth, screenHeight);
	
	while (!glfwWindowShouldClose(window)) {	

		fractal.update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		fractal.render();

		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}
	
	glfwTerminate();
	return 0;

}
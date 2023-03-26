#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GlM/glm.hpp>

import Shader;
import Theme;
import Mouse;

int screenWidth = 720;
int screenHeight = 720;

struct vec2 {
	GLfloat x, y;
};
struct vec4 {
	GLfloat x, y, z, w;
};

void resizeCallback(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* description) {
	std::cout << "Encountered opengl error " << error << ": " << description << "\n";
}

float map(float value, float start1, float stop1, float start2, float stop2) {
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
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


	float vertices[]{
		-1.f, -1.f, 0.f,
		 1.f,  1.f, 0.f,
		-1.f,  1.f, 0.f,
		 1.f, -1.f, 0.f,
	};

	unsigned int indices[]{
		0, 1, 2,
		0, 3, 1
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
 
	Shader shader("shader.vert", "shader.frag");
	GLint colorLocation = shader.getUniformLocation("color");
	GLint boundsLocation = shader.getUniformLocation("bounds");
	GLint pointLocation = shader.getUniformLocation("point");
	GLint resolutionLocation = shader.getUniformLocation("resolution");

	const float stdXmin = -2.5f;
	const float stdXmax =  1.5f;
	const float stdYmin = -2.f;
	const float stdYmax =  2.f;

	float zoom = 2.f;
	float scale = ((stdXmax - stdXmin) / screenWidth) * zoom;


	vec2 offset = { 0.f, 0.f };
	vec2 point  = { 0.f, 0.f };
	vec4 bounds = { (stdXmin + offset.x) * scale, (stdXmax + offset.x) * scale, (stdYmin + offset.y) * scale, (stdYmax + offset.y) * scale };
	
	//Calculate the position of the point in pixel coordinates
	vec2 pc = {
		map(point.x, bounds.x, bounds.y, 0.f, screenWidth),
		map(point.y, bounds.z, bounds.w, 0.f, screenHeight)
	};
	
	bool dragging = false, draggingPoint = false;

	glEnable(GL_DEPTH_TEST);
	glClearColor(.2f, .3f, .3f, 1.f);
	
	while (!glfwWindowShouldClose(window)) {

		if (Mouse::clicked) {
			vec2 toPoint = { Mouse::x - pc.x, Mouse::y - pc.y };
			if (toPoint.x * toPoint.x + toPoint.y * toPoint.y < 10 * 10) {
				draggingPoint = true;
			}
			else {
				dragging = true;
			}
			Mouse::clicked = false;
		}

		if (Mouse::released) {
			dragging = false;
			draggingPoint = false;
			Mouse::released = false;
		}

		if (dragging) {
			offset.x -= (Mouse::x - Mouse::lastX) * scale / zoom;
			offset.y += (Mouse::y - Mouse::lastY) * scale / zoom;
			pc.x = map(point.x, bounds.x, bounds.y, 0.f, screenWidth);
			pc.y = map(point.y, bounds.w, bounds.z, 0.f, screenHeight);
		}

		if (draggingPoint) {
			pc.x = Mouse::x;
			pc.y = Mouse::y;
			point.x = map(Mouse::x, 0.f, screenWidth, bounds.x, bounds.y);
			point.y = map(Mouse::y, 0.f, screenHeight, bounds.w, bounds.z);
		}		

		Mouse::lastX = Mouse::x;
		Mouse::lastY = Mouse::y;
		
		bounds = { (stdXmin + offset.x) / zoom, (stdXmax + offset.x) / zoom, (stdYmin + offset.y) / zoom, (stdYmax + offset.y) / zoom };

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glBindVertexArray(VAO);
		
		glUniform4fv(boundsLocation, 1, (GLfloat*)&bounds);
		glUniform2fv(pointLocation, 1, (GLfloat*)&point);
		glUniform4fv(colorLocation, 16, (GLfloat*) & Theme::brown);
		glUniform2f(resolutionLocation, screenWidth, screenHeight);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;

}
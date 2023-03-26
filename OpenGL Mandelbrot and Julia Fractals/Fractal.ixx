#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GlM/glm.hpp>

export module Fractal;

import Shader;
import Mouse;
import Theme;
import Util;

export class Fractal {
private:

	float vertices[12] {
		-1.f, -1.f, 0.f,
		-1.f,  1.f, 0.f,
		 1.f,  1.f, 0.f,
		 1.f, -1.f, 0.f,
	};

	unsigned int indices[6] {
		0, 1, 2,
		0, 2, 3,
	};
	
	unsigned int VAO, VBO, EBO;

	Shader shader;
	GLint colorLocation;
	GLint boundsLocation;
	GLint pointLocation;
	GLint resolutionLocation;
	
	const float stdXmin;
	const float stdXmax;
	const float stdYmin;
	const float stdYmax;

	float zoom;
	float scale;

	vec2 offset;
	vec2 point;
	vec4 bounds;

	vec2 pixelCoodinates;

	float screenWidth, screenHeight;

	bool dragging, draggingPoint;

	void createBuffers() {
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
	}

	void handleInputs() {
		if (Mouse::clicked) {
			vec2 toPoint = { Mouse::x - pixelCoodinates.x, Mouse::y - pixelCoodinates.y };
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
			pixelCoodinates.x = map(point.x, bounds.x, bounds.y, 0.f, screenWidth);
			pixelCoodinates.y = map(point.y, bounds.w, bounds.z, 0.f, screenHeight);
		}

		if (draggingPoint) {
			pixelCoodinates.x = Mouse::x;
			pixelCoodinates.y = Mouse::y;
			point.x = map(Mouse::x, 0.f, screenWidth, bounds.x, bounds.y);
			point.y = map(Mouse::y, 0.f, screenHeight, bounds.w, bounds.z);
		}

		Mouse::lastX = Mouse::x;
		Mouse::lastY = Mouse::y;
	}

public:

	Fractal(float screenWidth, float screenHeight) 
		: screenWidth(screenWidth), screenHeight(screenHeight),
		stdXmin(-2.5f), stdXmax(1.5f), stdYmin(-2.f), stdYmax(2.f),
		offset({ 0.f, 0.f }), point({ 0.f, 0.f }), zoom(1.f), scale(((stdXmax - stdXmin) / screenWidth) * zoom),
		bounds({ (stdXmin + offset.x) / zoom, (stdXmax + offset.x) / zoom, (stdYmin + offset.y) / zoom, (stdYmax + offset.y) / zoom }),
		pixelCoodinates({ map(point.x, bounds.x, bounds.y, 0.f, screenWidth), map(point.y, bounds.w, bounds.z, 0.f, screenHeight) }),
		shader("shader.vert", "shader.frag"),
		colorLocation(glGetUniformLocation(shader.programID, "color")),
		boundsLocation(glGetUniformLocation(shader.programID, "bounds")),
		pointLocation(glGetUniformLocation(shader.programID, "point")),
		resolutionLocation(glGetUniformLocation(shader.programID, "resolution")),
		dragging(false), draggingPoint(false) {
		createBuffers();
		
	}

	void update() {
		handleInputs();
		bounds = { (stdXmin + offset.x) / zoom, (stdXmax + offset.x) / zoom, (stdYmin + offset.y) / zoom, (stdYmax + offset.y) / zoom };
	}

	void render() {
		
		shader.use();

		glBindVertexArray(VAO);

		glUniform4fv(boundsLocation, 1, (GLfloat*)&bounds);
		glUniform2fv(pointLocation, 1, (GLfloat*)&point);
		glUniform4fv(colorLocation, 16, (GLfloat*)&Theme::brown);
		glUniform2f(resolutionLocation, screenWidth, screenHeight);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);		

	}

	~Fractal() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

};
#include "config.h"
#include "lab2.h"
#include "SDL2/SDL.h"
#include "input.h"
#include "triangle_soup.h"
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <stdlib.h>
#include <iostream>

const GLchar* vs =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec2 pos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, -1, 1);\n"
"}\n";

const GLchar* ps =
"#version 310 es\n"
"precision mediump float;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = vec4(0, 0, 0, 1);\n"
"}\n";

const GLchar* psPointC =
"#version 310 es\n"
"precision mediump float;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = vec4(1, 0, 0, 1);\n"
"}\n";

const GLuint point_attrib_index = 0;
const GLuint point_record = 1 * sizeof(glm::vec2);
const GLuint point_offset = 0 * sizeof(glm::vec2);

using namespace Display;
namespace Lab2 {
	// Display configs
	bool displayC = true;

	PointSet drawPoints;
	PointSet drawCHull;
	Point drawC;
	PointSet drawTriangulation;

	Lab2App::Lab2App() {}
	Lab2App::~Lab2App() {}

	bool Lab2App::Open() {
		App::Open();

		this->window = new Display::Window;
		this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			} else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
				auto inputPointSet = readPointsFromFile();
				auto pointSetError = validatePointSet(inputPointSet);

				if (pointSetError) {
					if (pointSetError == 1) {
						std::cout << "Invalid input: Point set contains atleast one duplicate point\n";
					} else if (pointSetError == 2) {
						std::cout << "Invalid input: The convex hull of the point set does not contain the origin OR all the points lie on the same line\n";
					}
					this->window->Close();
				} else {
					drawPoints = inputPointSet;	// draw points
					tie(drawCHull, drawC, drawTriangulation) = triangleSoup(inputPointSet);
				}
			} else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
				std::cout << "Enter a number n >= 3 and press enter\n";
				this->numInput = "";
			} else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
				std::string pressedNum = std::to_string(key - 48);
				this->numInput += pressedNum;
			} else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
				std::cout << this->numInput + "\n";
				int numPoints = std::stoi(numInput);

				if (numPoints < 3) {
					std::cout << "Invalid input: n is not >= 3\n";
				} else {
					auto randomSet = randomPointSet(numPoints);
					drawPoints = randomSet;	// draw points
					tie(drawCHull, drawC, drawTriangulation) = triangleSoup(randomSet);
				}
			}
		});
		this->window->SetTitle(std::string("Lab 2"));
		this->window->SetSize(1200, 1200);

		if (this->window->Open()) {
			// set clear color to pale yellow
			glClearColor(1.0f, 1.0f, 0.6f, 1.0f);

			// setup vertex shader
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLint length = (GLint)std::strlen(vs);
			glShaderSource(this->vertexShader, 1, &vs, &length);
			glCompileShader(this->vertexShader);

			// get error log
			GLint shaderLogSize;
			glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// setup pixel shaders
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(ps);
			glShaderSource(this->pixelShader, 1, &ps, &length);
			glCompileShader(this->pixelShader);

			this->cPointPixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(psPointC);
			glShaderSource(this->cPointPixelShader, 1, &psPointC, &length);
			glCompileShader(this->cPointPixelShader);			

			// get error log
			shaderLogSize;
			glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// create a program object
			this->program = glCreateProgram();
			glAttachShader(this->program, this->vertexShader);
			glAttachShader(this->program, this->pixelShader);
			glLinkProgram(this->program);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}

			// second program for coloring c point
			this->cPointProgram = glCreateProgram();
			glAttachShader(this->cPointProgram, this->vertexShader);
			glAttachShader(this->cPointProgram, this->cPointPixelShader);
			glLinkProgram(this->cPointProgram);
			glGetProgramiv(this->cPointProgram, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->cPointProgram, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}

			// setup array buffer
			glGenBuffers(1, &this->buf);
			glUseProgram(this->program);
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glEnableVertexAttribArray(point_attrib_index);
			glVertexAttribPointer(point_attrib_index, 2, GL_FLOAT, GL_FALSE, point_record, (GLvoid*)point_offset);
			
			glPointSize(10);
			return true;
		}
		return false;
	}

	void Lab2App::Run() {
		// this->points = {
		// 	glm::vec2(-0.5, 0.5),
		// 	glm::vec2(0.5, 0.5),
		// 	glm::vec2(-0.5, -0.5),
		// 	glm::vec2(0.5, -0.5)
		// };

		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT);
			this->window->Update();
			
			glUseProgram(this->program);
			// Draw convex hull
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glBufferData(GL_ARRAY_BUFFER, drawCHull.size() * sizeof(glm::vec2), &drawCHull[0], GL_STATIC_DRAW);
			glDrawArrays(GL_LINE_LOOP, point_attrib_index, drawCHull.size());

			// Draw points
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glBufferData(GL_ARRAY_BUFFER, drawPoints.size() * sizeof(glm::vec2), &drawPoints[0], GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, point_attrib_index, drawPoints.size());

			// Draw triangulation
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glBufferData(GL_ARRAY_BUFFER, drawTriangulation.size() * sizeof(glm::vec2), &drawTriangulation[0], GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, point_attrib_index, drawTriangulation.size());

			// Draw c point
			if (displayC) {
				glUseProgram(this->cPointProgram);
				glBindBuffer(GL_ARRAY_BUFFER, this->buf);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), &drawC, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, point_attrib_index, 1);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();
		}
	}
}
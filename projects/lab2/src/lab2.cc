#include "config.h"
#include "lab2.h"
#include "SDL2/SDL.h"
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <fstream>
#include <iostream>
#include <iomanip>

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

const GLuint point_attrib_index = 0;
const GLuint point_record = 1 * sizeof(glm::vec2);
const GLuint point_offset = 0 * sizeof(glm::vec2);

using namespace Display;
namespace Lab2 {
	Lab2App::Lab2App() {}
	Lab2App::~Lab2App() {}

	bool Lab2App::Open() {
		App::Open();

		this->window = new Display::Window;
		this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
			if (key == 256 && action == GLFW_PRESS) {	// key ESC
				this->window->Close();
			} else if (key == 73 && action == GLFW_PRESS) { // key 'I' = input
				readPointsFromFile();
			} 
		});
		this->window->SetTitle(std::string("Lab 2"));
		this->window->SetSize(1500, 1500);

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

			// setup pixel shader
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(ps);
			glShaderSource(this->pixelShader, 1, &ps, &length);
			glCompileShader(this->pixelShader);

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

			// setup array buffer
			glGenBuffers(1, &this->buf);
			glUseProgram(this->program);
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glEnableVertexAttribArray(point_attrib_index);
			glVertexAttribPointer(point_attrib_index, 2, GL_FLOAT, GL_FALSE, point_record, (GLvoid*)point_offset);
			
			glPointSize(25);
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
			
			// Set data to koch snowflake vertices
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), &points[0], GL_STATIC_DRAW);

			// Draw points
			glDrawArrays(GL_POINTS, point_attrib_index, points.size());
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();
		}
	}

	std::vector<glm::vec2> readPointsFromFile() {
		std::ifstream inFile;
		std::string line;

		inFile.open("input.txt");

		if (!inFile) {
			std::cout << "Unable to open input file 'input.txt'\n";
		}

		while (std::getline(inFile, line)) {
			std::cout << line << "\n";
		}

		inFile.close();
		return {};
	}
}
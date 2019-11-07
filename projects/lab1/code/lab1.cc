#include "config.h"
#include "lab1.h"
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

const GLchar* vs =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

const GLuint vertex_attrib_index = 0;
const GLuint color_attrib_index = 1;
const GLuint vertex_record = 7 * sizeof(float32);
const GLuint vertex_offset = 0 * sizeof(float32);
const GLuint color_offset = 4 * sizeof(float32);

using namespace Display;
namespace Lab1 {
	Lab1App::Lab1App() {

	}

	Lab1App::~Lab1App() {

	}

	bool Lab1App::Open() {
		App::Open();

		this->window = new Display::Window;
		window->SetKeyPressFunction([this](int32, int32, int32, int32) {
			this->window->Close();
		});
		window->SetTitle(std::string("Lab1: 2D Koch Snowflake"));
		
		
		this->vertices = {
			-0.5f,	-0.5f,	-1,			// pos 0
			0,		0,		1,		1,	// color 0
			0,		0.5f,	-1,			// pos 1
			0,		0,		1,		1,	// color 1
			0.5f,	-0.5f,	-1,			// pos 2
			0,		0,		1,		1,	// color 2
			// 0.75f,	0.75f,	-1,			// pos 3
			// 0,		0,		1,		1,	// color 3
		};
		this->indices = {
			0, 1, 2,
			// 1, 3, 2
		};
	

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
			return true;
		}
		return false;

	}

	void Lab1App::Run() {
		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT);
			this->window->Update();

			// Koch config
			const int depth = 1;
			const int num_edges = 3*(pow(4, depth));
			// The number of vertex in Koch Snowflake is the number of edges * 2
			const int num_vertex = 2*num_edges;
						
			// Get the vertices and indices of the koch snowflake
			// std::pair<std::vector<GLfloat>, std::vector<GLuint>> res = koch_snowflake(&this->vertices, &this->indices, 0);
			// this->vertices = res.first;
			// this->indices = res.second;
			koch_snowflake(this->vertices, this->indices, 0);

			// Bind the new data to the buffer objects
			// setup vbo (vertex buffer object)
			glGenBuffers(1, &this->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// setup ibo (index buffer object)
			glGenBuffers(1, &this->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// Draw the snowflake at specified depth			
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glUseProgram(this->program);
			glEnableVertexAttribArray(vertex_attrib_index);
			glEnableVertexAttribArray(color_attrib_index);
			glVertexAttribPointer(vertex_attrib_index, 3, GL_FLOAT, GL_FALSE, vertex_record, (GLvoid*)(vertex_offset));
			glVertexAttribPointer(color_attrib_index, 4, GL_FLOAT, GL_FALSE, vertex_record, (GLvoid*)(color_offset));
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();
		}
	}

	// Calculates and returns the vertexes and indices given a recursion depth and starting vectors
	// std::pair<std::vector<GLfloat>, std::vector<GLuint>>
	void koch_snowflake(vector<GLfloat> vertices, vector<GLuint> indices, int depth) {
			if (depth == 0) {
				return;
			} else {
				// vector<GLfloat> first = 
			}
	}
}
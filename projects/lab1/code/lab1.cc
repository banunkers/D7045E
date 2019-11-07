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
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"}\n";

const GLchar* ps =
"#version 310 es\n"
"precision mediump float;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = vec4(1, 0, 0, 0.25);\n"
"}\n";

const GLuint vertex_attrib_index = 0;
const GLuint vertex_record = 3 * sizeof(float32);
const GLuint vertex_offset = 0 * sizeof(float32);

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
		
		// this->vertices = {
		// 	-0.5f,	-0.5f,	-1,			// pos 0
		// 	0,		0.5f,	-1,			// pos 1
		// 	0.5f,	-0.5f,	-1,			// pos 2
		// 	// 0.75f,	0.75f,	-1,			// pos 3
		// };
	

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
						
			// Get the vertices of the koch snowflake at input depth
			glm::vec3 p0 = {-0.5f, -0.5f, -1.0f};
			glm::vec3 p1 = {0.0f, 0.5f, -1.0f};
			glm::vec3 p2 = {0.5f, -0.5f, -1.0f};
			glm::vec3 p0p1_m = midpoint(p0, p1);
			glm::vec3 p1p2_m = midpoint(p1, p2);
			glm::vec3 p2p0_m = midpoint(p2, p0);
		
			std::vector<GLfloat> edge1 = koch_snowflake(2, p0, p1, p2, p0p1_m);
			std::vector<GLfloat> edge2 = koch_snowflake(2, p1, p2, p0, p1p2_m);
			std::vector<GLfloat> edge3 = koch_snowflake(2, p2, p0, p1, p2p0_m);
			edge1.insert(edge1.end(), edge2.begin(), edge2.end());
			edge1.insert(edge1.end(), edge3.begin(), edge3.end());
			vertices = edge1;

			// setup vbo (vertex buffer object)
			glGenBuffers(1, &this->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
			glUseProgram(this->program);
			glEnableVertexAttribArray(vertex_attrib_index);
			glVertexAttribPointer(vertex_attrib_index, 3, GL_FLOAT, GL_FALSE, vertex_record, (GLvoid*)vertex_offset);
			
			// debugg transperancy
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Draw the snowflake
			glDrawArrays(GL_TRIANGLES, vertex_attrib_index, vertices.size() / 3);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();
		}
	}

	/**
	 * Calculates the vertices of a Koch Snowflake at a specified depth
	 * @param depth the recursion depth (starting at 1)
	 * @param p0 first point of a edge
	 * @param p1 second point of a edge
	 * @param b helper base
	 * @param m helper midpoint between p0 and  p1
	*/
	std::vector<GLfloat> koch_snowflake(int depth, glm::vec3 p0, glm::vec3 p1, glm::vec3 b, glm::vec3 m) {
			if (depth <= 1) {
				return {
					p0.x, p0.y, p0.z,
					p1.x, p1.y, p1.z,
					b.x, b.y, b.z,
				};
			} else {
				// Calculate point q0 and q1
				glm::vec3 q0 = {(2.0f*p0.x + p1.x)/3.0f, (2.0f*p0.y + p1.y)/3.0f, p0.z};
				glm::vec3 q1 = {(p0.x + 2.0f*p1.x)/3.0f, (p0.y + 2.0f*p1.y)/3.0f, p0.z};

				// Calculate point a
				float p0p1_mag = magnitude(p0, p1);
				float ma_height = sqrtf(powf(p0p1_mag/3.0f, 2.0f) - powf(p0p1_mag/6.0f, 2.0f));
				glm::vec3 b_m_unit_vec = unit_vec(b, m);
				// Point a will be a point which is offset from point m in the direction of b -> m by the height of m -> a
				glm::vec3 a = {m.x + ma_height*b_m_unit_vec.x, m.y + ma_height*b_m_unit_vec.y, -1.0f};

				return {
					p0.x, p0.y, p0.z,
					p1.x, p1.y, p1.y,
					b.x, b.y, b.z,
					q0.x, q0.y, q0.z,
					a.x, a.y, a.z,
					q1.x, q1.y, q1.z,
				};
			}
	}

	// Calculates the magnitude of the vector between two points p and q
	float magnitude(glm::vec3 p, glm::vec3 q) {
		return sqrtf(powf((q.x - p.x), 2.0f) + powf((q.y - p.y), 2.0f));
	}

	// Calculates the unit vector of the vector between two points p and q
	glm::vec3 unit_vec(glm::vec3 p, glm::vec3 q) {
		float mag = magnitude(p, q);
		return glm::vec3{(q.x - p.x)/mag, (q.y - p.y)/mag, -1.0f};
	}

	glm::vec3 midpoint(glm::vec3 p, glm::vec3 q) {
		return glm::vec3{(p.x + q.x) / 2.0f, (p.y + q.y) / 2.0f, -1.0f};
	}
}
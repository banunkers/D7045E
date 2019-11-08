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
	struct point {
		float x;
		float y;
	};

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
		int32 width, height;
		window->GetSize(width, height);
		window->SetSize(2*width, 2*height);
		
		this->vertices = {
			-0.5f,	-0.5f,	-1,			// pos 0
			0,		0.5f,	-1,			// pos 1
			0.5f,	-0.5f,	-1,			// pos 2
			// 0.75f,	0.75f,	-1,			// pos 3
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
						
			// Get the vertices of the koch snowflake at input depth
			point p0 = {-0.866f/1.5f, -0.5f/1.5f};
			point p1 = {0.866f/1.5f, -0.5f/1.5f};
			point p2 = {0.0f/1.5f, 1.0f/1.5f};
			std::vector<GLfloat> edge1 = koch_snowflake(2, p0, p1, p2);
			std::vector<GLfloat> edge2 = koch_snowflake(2, p1, p2, p0);
			std::vector<GLfloat> edge3 = koch_snowflake(2, p2, p0, p1);
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
	std::vector<float> koch_snowflake(int depth, point p0, point p1, point b) {
			if (depth <= 1) {
				return {
					p0.x, p0.y, -1.0f,
					p1.x, p1.y, -1.0f,
					b.x, b.y, -1.0f,
				};
			} else {
				point m = midpoint(p0, p1);

				// Calculate point q0 and q1
				point q0 = {(2.0f*p0.x + p1.x)/3.0f, (2.0f*p0.y + p1.y)/3.0f};
				point q1 = {(p0.x + 2.0f*p1.x)/3.0f, (p0.y + 2.0f*p1.y)/3.0f};

				// Calculate point a
				float p0p1_mag = magnitude(p0, p1);
				float ma_height = sqrtf(powf(p0p1_mag/3.0f, 2.0f) - powf(p0p1_mag/6.0f, 2.0f));
				point b_m_unit_vec = unit_vec(b, m);
				
				// Point a will be a point which is offset from point m in the direction of b -> m by the height of m -> a
				point a = {m.x + ma_height*b_m_unit_vec.x, m.y + ma_height*b_m_unit_vec.y};

				return {
					p0.x, p0.y, -1.0f,
					p1.x, p1.y, -1.0f,
					b.x, b.y, -1.0f,
					q0.x, q0.y, -1.0f,
					a.x, a.y, -1.0f,
					q1.x, q1.y, -1.0f,
				};
			}
	}

	// Calculates the magnitude of the vector between two points p and q
	float magnitude(point p, point q) {
		return sqrtf(powf(q.x - p.x, 2.0f) + powf(q.y - p.y, 2.0f));
	}

	// Calculates the unit vector of the vector between two points p and q
	point unit_vec(point p, point q) {
		float mag = magnitude(p, q);
		return point{(q.x - p.x)/mag, (q.y - p.y)/mag};
	}

	point midpoint(point p, point q) {
		return point{(p.x + q.x)/2.0f, (p.y + q.y)/2.0f};
	}
}
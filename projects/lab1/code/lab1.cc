#include "config.h"
#include "lab1.h"
#include "SDL2/SDL.h"
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
"	Color = vec4(1, 0, 0, 1);\n"
"}\n";

const GLuint vertex_attrib_index = 0;
const GLuint vertex_record = 2 * sizeof(float32);
const GLuint vertex_offset = 0 * sizeof(float32);

using namespace Display;
namespace Lab1 {
	
	struct Point {
		float x;
		float y;

		Point(float x, float y) {
			this->x = x;
			this->y = y;
		}
	};

	struct Triangle {
		Point q0 = Point(0, 0);
		Point a = Point(0, 0);
		Point q1 = Point(0, 0);

		Triangle(Point q0, Point a, Point q1) {
			this->q0 = q0;
			this->a = a;
			this->q1 = q1;
		}
	};

	Lab1App::Lab1App() {

	}

	Lab1App::~Lab1App() {

	}

	bool Lab1App::Open() {
		App::Open();

		this->window = new Display::Window;
		this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
			if (key == 256) {	// key ESC
				this->window->Close();
			} else if (key > 48 && key < 58) {// key 1-9
				auto num = key - 48;	// Get the specific number pressed
				
				// Vertices of equilateral starting triangle
				Point p0 = Point(-0.866f/1.5f, -0.5f/1.5f);
				Point p1 = Point(0.0f/1.5f, 1.0f/1.5f);
				Point p2 = Point(0.866f/1.5f, -0.5f/1.5f);

				// get the koch snowflake vertices given the starting triangle 
				vertices = koch_snowflake(num, p0, p1, p2, false);
			}
		});

		// SDL_KeyboardEvent();

		this->window->SetTitle(std::string("Lab1: 2D Koch Snowflake"));
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
			return true;
		}
		return false;

	}

	void Lab1App::Run() {
		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT);
			this->window->Update();

			// setup vbo (vertex buffer object)
			glGenBuffers(1, &this->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
			glUseProgram(this->program);
			glEnableVertexAttribArray(vertex_attrib_index);
			glVertexAttribPointer(vertex_attrib_index, 2, GL_FLOAT, GL_FALSE, vertex_record, (GLvoid*)vertex_offset);
			
			// debugg transperancy
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Draw the snowflake
			glLineWidth(5);
			glDrawArrays(GL_LINE_LOOP, vertex_attrib_index, vertices.size()/2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();
		}
	}

	/**
	 * @brief Creates a koch snowflake given the vertices of a equilateral triangle
	 * @param depth the recusion depth
	 * @param p0 vertex 
	 * @param p1 vertex
	 * @param p2 vertex
	 * @param one_edge if the koch snowflake should be calculated for only the first edge p0p1
	*/
	std::vector<float> koch_snowflake(int depth, Point p0, Point p1, Point p2, bool one_edge) {
		if (depth <= 1) {
			if (one_edge) {
				return {
					p0.x, p0.y,
					p1.x, p1.y,
				};
			} else {
				return {
					p0.x, p0.y,
					p1.x, p1.y,
					p2.x, p2.y,
				};
			}
		} else {
			std::vector<float> snowflake;
			if (!one_edge) {
				// Calc the new triangles on every edge
				Triangle fe_trig = calc_triangle(depth, p0, p1, p2);
				Triangle se_trig = calc_triangle(depth, p1, p2, p0);
				Triangle te_trig = calc_triangle(depth, p2, p0, p1);

				/* 
				* 
				* Calculate next Koch at depth - 1
				* 
				*/
				// First edge
				auto fe_q0_a = koch_snowflake(depth - 1, fe_trig.q0, fe_trig.a, fe_trig.q1, true);
				auto fe_a_q1 = koch_snowflake(depth - 1, fe_trig.a, fe_trig.q1, fe_trig.q0, true);
				auto fe_p0_q0 = koch_snowflake(depth -1, p0, fe_trig.q0, te_trig.q1, true);
				auto fe_q1_p1 = koch_snowflake(depth -1, fe_trig.q1, p1, se_trig.q0, true);

				// Append results inbetween edge p0p1
				snowflake = {p0.x, p0.y};
				snowflake.insert(snowflake.end(), fe_p0_q0.begin(), fe_p0_q0.end());
				snowflake.insert(snowflake.end(), fe_q0_a.begin(), fe_q0_a.end());
				snowflake.insert(snowflake.end(), fe_a_q1.begin(), fe_a_q1.end());
				snowflake.insert(snowflake.end(), fe_q1_p1.begin(), fe_q1_p1.end());
				snowflake.insert(snowflake.end(), {p1.x, p1.y});
				
				// Second edge
				auto se_q0_a = koch_snowflake(depth - 1, se_trig.q0, se_trig.a, se_trig.q1, true);
				auto se_a_q1 = koch_snowflake(depth - 1, se_trig.a, se_trig.q1, se_trig.q0, true);
				auto se_p1_q0 = koch_snowflake(depth -1, p1, se_trig.q0, fe_trig.q1, true);
				auto se_q1_p2 = koch_snowflake(depth -1, se_trig.q1, p2, te_trig.q0, true);

				// Append results inbetween edge p1p2
				snowflake.insert(snowflake.end(), se_p1_q0.begin(), se_p1_q0.end());
				snowflake.insert(snowflake.end(), se_q0_a.begin(), se_q0_a.end());
				snowflake.insert(snowflake.end(), se_a_q1.begin(), se_a_q1.end());
				snowflake.insert(snowflake.end(), se_q1_p2.begin(), se_q1_p2.end());
				snowflake.insert(snowflake.end(), {p2.x, p2.y});

				// Third edge
				auto te_q0_a = koch_snowflake(depth - 1, te_trig.q0, te_trig.a, te_trig.q1, true);
				auto te_a_q1 = koch_snowflake(depth - 1, te_trig.a, te_trig.q1, te_trig.q0, true);
				auto te_p2_q0 = koch_snowflake(depth -1, p2, te_trig.q0, se_trig.q1, true);
				auto te_q1_p0 = koch_snowflake(depth -1, te_trig.q1, p0, fe_trig.q0, true);

				// Append results inbetween endge p2p0
				snowflake.insert(snowflake.end(), te_p2_q0.begin(), te_p2_q0.end());
				snowflake.insert(snowflake.end(), te_q0_a.begin(), te_q0_a.end());
				snowflake.insert(snowflake.end(), te_a_q1.begin(), te_a_q1.end());
				snowflake.insert(snowflake.end(), te_q1_p0.begin(), te_q1_p0.end());
			} else {	// Only one edge p0p1
				Triangle trig = calc_triangle(depth, p0, p1, p2);
				// Helper triangles used as base for next iterion of edges
				Triangle se_trig = calc_triangle(depth, p1, p2, p0);
				Triangle te_trig = calc_triangle(depth, p2, p0, p1);
				
				auto q0_a = koch_snowflake(depth - 1, trig.q0, trig.a, trig.q1, true);
				auto a_q1 = koch_snowflake(depth - 1, trig.a, trig.q1, trig.q0, true);
				auto p0_q0 = koch_snowflake(depth -1, p0, trig.q0, te_trig.q1, true);
				auto q1_p1 = koch_snowflake(depth -1, trig.q1, p1, se_trig.q0, true);

				// Append results between edge p0p1
				snowflake = {p0.x, p0.y};
				snowflake.insert(snowflake.end(), p0_q0.begin(), p0_q0.end());
				snowflake.insert(snowflake.end(), q0_a.begin(), q0_a.end());
				snowflake.insert(snowflake.end(), a_q1.begin(), a_q1.end());
				snowflake.insert(snowflake.end(), q1_p1.begin(), q1_p1.end());
				snowflake.insert(snowflake.end(), {p1.x, p1.y});
			}
			
			return snowflake;
		}
	}

	/**
	 * @brief Calculates the vertices of the next triangle in the koch snowflake at a specified depth
	 * @param depth the recursion depth (starting at 1)
	 * @param p0 first point of a edge
	 * @param p1 second point of a edge
	 * @param b helper base
	*/
	Triangle calc_triangle(int depth, Point p0, Point p1, Point b) {
		Point m = midpoint(p0, p1);

		// q0 and q1: 1/3 and 2/3 away from p0
		Point q0 = {(2.0f*p0.x + p1.x)/3.0f, (2.0f*p0.y + p1.y)/3.0f};
		Point q1 = {(p0.x + 2.0f*p1.x)/3.0f, (p0.y + 2.0f*p1.y)/3.0f};

		// Point 'a' will be a point which is offset from Point 'm' in the direction of vector 'bm' by the height of vector 'ma'
		float p0p1_mag = magnitude(p0, p1);
		float ma_height = sqrtf(powf(p0p1_mag/3.0f, 2.0f) - powf(p0p1_mag/6.0f, 2.0f));
		Point b_m_unit_vec = unit_vec(b, m);
		Point a = {m.x + ma_height*b_m_unit_vec.x, m.y + ma_height*b_m_unit_vec.y};
		
		return Triangle(q0, a, q1);
	}

	// Calculates the magnitude of the vector between two points p and q
	float magnitude(Point p, Point q) {
		return sqrtf(powf(q.x - p.x, 2.0f) + powf(q.y - p.y, 2.0f));
	}

	// Calculates the unit vector of the vector between two points p and q
	Point unit_vec(Point p, Point q) {
		float mag = magnitude(p, q);
		return Point((q.x - p.x)/mag, (q.y - p.y)/mag);
	}

	Point midpoint(Point p, Point q) {
		return Point((p.x + q.x)/2.0f, (p.y + q.y)/2.0f);
	}
}
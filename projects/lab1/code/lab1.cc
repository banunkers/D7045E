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
						
			// Vertices of equilateral triangle
			Point p0 = Point(-0.866f/1.5f, -0.5f/1.5f);
			Point p1 = Point(0.0f/1.5f, 1.0f/1.5f);
			Point p2 = Point(0.866f/1.5f, -0.5f/1.5f);

			// get the koch snowflake vertices given the starting triangle 
			vertices = koch_snowflake(3, p0, p1, p2);

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
	 * @brief Creates a koch snowflake given the vertices of a equilateral triangle
	 * @param depth the recusion depth
	 * @param p0 vertex 
	 * @param p1 vertex
	 * @param p2 vertex
	*/
	std::vector<float> koch_snowflake(int depth, Point p0, Point p1, Point p2) {
		std::vector<float> snowflake = {
			p0.x, p0.y, -1.0f,
			p1.x, p1.y, -1.0f,
			p2.x, p2.y, -1.0f,
		};

		if (depth <= 1) {
			return snowflake;
		} else {
			// Get the new triangles on every edge and calculate Koch on each new parts at depth-1
			// First edge 
			Triangle fe_trig = calc_triangle(depth, p0, p1, p2);
			std::vector<float> fe_q0_a = koch_snowflake(depth - 1, fe_trig.q0, fe_trig.a, fe_trig.q1);
			std::vector<float> fe_a_q1 = koch_snowflake(depth - 1, fe_trig.a, fe_trig.q1, fe_trig.q0);
			// Append results to entire snowflake
			snowflake.insert(snowflake.end(), fe_q0_a.begin(), fe_q0_a.end());
			snowflake.insert(snowflake.end(), fe_a_q1.begin(), fe_a_q1.end());
			
			// Second edge
			Triangle se_trig = calc_triangle(depth, p1, p2, p0);
			std::vector<float> se_q0_a = koch_snowflake(depth - 1, se_trig.q0, se_trig.a, se_trig.q1);
			std::vector<float> se_a_q1 = koch_snowflake(depth - 1, se_trig.a, se_trig.q1, se_trig.q0);

			// Append results to entire snowflake
			snowflake.insert(snowflake.end(), se_q0_a.begin(), se_q0_a.end());
			snowflake.insert(snowflake.end(), se_a_q1.begin(), se_a_q1.end());
			

			Triangle te_trig = calc_triangle(depth, p2, p0, p1);
			std::vector<float> te_q0_a = koch_snowflake(depth - 1, te_trig.q0, te_trig.a, te_trig.q1);
			std::vector<float> te_a_q1 = koch_snowflake(depth - 1, te_trig.a, te_trig.q1, te_trig.q0);

			// Append results to entire snowflake
			snowflake.insert(snowflake.end(), te_q0_a.begin(), te_q0_a.end());
			snowflake.insert(snowflake.end(), te_a_q1.begin(), te_a_q1.end());

			return snowflake;
		}
	}

	/**
	 * @brief Calculates the vertices of the next triangle in the koch snowflake at a specified depth
	 * @param depth the recursion depth (starting at 1)
	 * @param p0 first point of a edge
	 * @param p1 second point of a edge
	 * @param b helper base
	 * @param m helper midpoint between p0 and  p1
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
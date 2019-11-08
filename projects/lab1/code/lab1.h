#pragma once

#include "core/app.h"
#include "render/window.h"
#include <glm/vec3.hpp> // point
#include <vector>

namespace Lab1 {
	class Lab1App: public Core::App {
		public:
			/// constructor
			Lab1App();
			/// destructor
			~Lab1App();
			/// open app
			bool Open();
			/// run app
			void Run();
		private:
			GLuint program;
			GLuint vertexShader;
			GLuint pixelShader;
			GLuint vbo;
			std::vector<GLfloat> vertices;
			Display::Window* window;
	};

	struct point;

	// std::pair<std::vector<GLfloat>, std::vector<GLuint>> koch_snowflake(vector<GLfloat> *vertices, vector<GLuint> *indices, int depth);
	std::vector<float> koch_snowflake(int depth, point p0, point p1, point b);
	
	float magnitude(point p, point q);

	point unit_vec(point p, point q);

	point midpoint(point p, point q);
}
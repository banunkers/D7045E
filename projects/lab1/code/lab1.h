#pragma once

#include "core/app.h"
#include "render/window.h"
#include <glm/vec3.hpp> // Point
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
			GLuint buff;
			std::vector<GLfloat> vertices;
			Display::Window* window;
	};

	struct Point; 
	struct Triangle;

	std::vector<float> koch_snowflake(int depth, Point p0, Point p1, Point p2, bool one_edge);

	Triangle calc_triangle(int depth, Point p0, Point p1, Point b);
	
	float magnitude(Point p, Point q);

	Point unit_vec(Point p, Point q);

	Point midpoint(Point p, Point q);
}
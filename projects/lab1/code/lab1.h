#pragma once

#include "core/app.h"
#include "render/window.h"
#include <glm/vec3.hpp> // glm::vec3
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

	// std::pair<std::vector<GLfloat>, std::vector<GLuint>> koch_snowflake(vector<GLfloat> *vertices, vector<GLuint> *indices, int depth);
	std::vector<GLfloat> koch_snowflake(int depth, glm::vec3 p0, glm::vec3 p1, glm::vec3 b, glm::vec3 m);
	
	float magnitude(glm::vec3 p, glm::vec3 q);

	glm::vec3 unit_vec(glm::vec3 p, glm::vec3 q);

}
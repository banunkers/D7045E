#pragma once

#include "core/app.h"
#include "render/window.h"
#include <vector>

using namespace std;

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
			GLuint ibo;
			vector<GLfloat> vertices;
			vector<GLuint> indices;
			Display::Window* window;
	};

	// std::pair<std::vector<GLfloat>, std::vector<GLuint>> koch_snowflake(vector<GLfloat> *vertices, vector<GLuint> *indices, int depth);
	void koch_snowflake(vector<GLfloat> vertices, vector<GLuint> indices, int depth);
}
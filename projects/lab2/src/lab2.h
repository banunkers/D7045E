#pragma once

#include "core/app.h"
#include "core/app.h"
#include "render/window.h"
#include "types.h"

namespace Lab2 {
	class Lab2App: public Core::App {
		public:
			Lab2App();
			~Lab2App();
			bool Open();
			void Run();
		private:
			std::vector<GLfloat> genVertexBuffer();
			std::vector<unsigned int> genVertexIndices();
			std::vector<unsigned int> genTriangleIndices();
			std::vector<unsigned int> genEdgeIndices();
			void updateBuffers();
			void pickRGBVertices();
			glm::vec4 getVertexColor(Point &vertex);

			GLuint program;
			GLuint programEdge;
			GLuint vertexShader;
			GLuint pixelShader;
			GLuint pixelShaderEdge;
			GLuint vbo;
			GLuint ibo;
			Display::Window* window;
			std::string numInput;
	};
};
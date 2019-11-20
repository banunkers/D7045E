#pragma once

#include "core/app.h"
#include "core/app.h"
#include "render/window.h"
#include <glm/vec2.hpp> // Point
#include <vector>

namespace Lab2 {
	class Lab2App: public Core::App {
		public:
			Lab2App();
			~Lab2App();
			bool Open();
			void Run();
		private:
			GLuint program;
			GLuint vertexShader;
			GLuint pixelShader;
			GLuint buf;
			std::vector<glm::vec2> points;
			Display::Window* window;
	};

	std::vector<glm::vec2> readPointsFromFile();
}
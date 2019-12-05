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
			GLuint program;
			GLuint cPointProgram;
			GLuint vertexShader;
			GLuint pixelShader;
			GLuint cPointPixelShader;
			GLuint buf;
			Display::Window* window;
			std::string numInput;
	};

	extern PointSet drawPoints;
	extern PointSet drawCHull;
	extern Point drawC;
	extern PointSet drawTriangulation;
};
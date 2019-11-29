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
			GLuint cPointProgram;
			GLuint vertexShader;
			GLuint pixelShader;
			GLuint cPointPixelShader;
			GLuint buf;
			Display::Window* window;
			std::string numInput;
	};

	typedef std::vector<glm::vec2> PointSet;
	typedef glm::vec2 Point;

	extern PointSet points;
	extern PointSet cHullPoints;
	extern Point cPoint;

	PointSet readPointsFromFile();
	PointSet randomPointSet(int numPoints);
	int validatePointSet(PointSet &set);
	bool leftOf(Point &a, Point &b, Point &point);
	void sortPointSet(PointSet &set);
	Point pickPoint(PointSet &set, PointSet &cHull);
	PointSet triangleSoup(PointSet &set);
	PointSet convexHull(PointSet &set);
};
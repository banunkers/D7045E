#include "config.h"
#include "lab2.h"
#include "SDL2/SDL.h"
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <algorithm>

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
"	Color = vec4(0, 0, 0, 1);\n"
"}\n";

const GLuint point_attrib_index = 0;
const GLuint point_record = 1 * sizeof(glm::vec2);
const GLuint point_offset = 0 * sizeof(glm::vec2);

using namespace Display;
namespace Lab2 {
	Lab2App::Lab2App() {}
	Lab2App::~Lab2App() {}

	bool Lab2App::Open() {
		App::Open();

		this->window = new Display::Window;
		this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			} else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
				auto inputPointSet = readPointsFromFile();
				auto pointSetError = validatePointSet(inputPointSet);

				if (pointSetError) {
					if (pointSetError == 1) {
						std::cout << "Invalid input: Point set contains atleast one duplicate point\n";
					} else if (pointSetError == 2) {
						std::cout << "Invalid input: The convex hull of the point set does not contain the origin OR all the points lie on the same line\n";
					}
					this->window->Close();
				} else {
					this->points = inputPointSet;
					this->convexHull = triangleSoup(inputPointSet);
				}
			} else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
				std::cout << "Enter a number n >= 3 and press enter\n";
				this->numInput = "";
			} else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
				std::string pressedNum = std::to_string(key - 48);
				this->numInput += pressedNum;
			} else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
				std::cout << this->numInput + "\n";
				int numPoints = std::stoi(numInput);

				if (numPoints < 3) {
					std::cout << "Invalid input: n is not >= 3\n";
				} else {
					// Generate a ok random point set
					auto randomSet = randomPointSet(numPoints);
					auto pointSetError = validatePointSet(randomSet);
					while (pointSetError) {
						randomSet = randomPointSet(numPoints);
						pointSetError = validatePointSet(randomSet);
					}

					this->points = randomSet;
					this->convexHull = triangleSoup(randomSet);
				}
			}
		});
		this->window->SetTitle(std::string("Lab 2"));
		this->window->SetSize(1200, 1200);

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

			// setup array buffer
			glGenBuffers(1, &this->buf);
			glUseProgram(this->program);
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glEnableVertexAttribArray(point_attrib_index);
			glVertexAttribPointer(point_attrib_index, 2, GL_FLOAT, GL_FALSE, point_record, (GLvoid*)point_offset);
			
			glPointSize(25);
			return true;
		}
		return false;
	}

	void Lab2App::Run() {
		// this->points = {
		// 	glm::vec2(-0.5, 0.5),
		// 	glm::vec2(0.5, 0.5),
		// 	glm::vec2(-0.5, -0.5),
		// 	glm::vec2(0.5, -0.5)
		// };

		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT);
			this->window->Update();
			
			// Draw point set
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), &points[0], GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, point_attrib_index, points.size());
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Draw convex hull
			glBindBuffer(GL_ARRAY_BUFFER, this->buf);
			glBufferData(GL_ARRAY_BUFFER, convexHull.size() * sizeof(glm::vec2), &convexHull[0], GL_STATIC_DRAW);
			glDrawArrays(GL_LINE_LOOP, point_attrib_index, convexHull.size());

			this->window->SwapBuffers();
		}
	}

	std::vector<glm::vec2> readPointsFromFile() {
		std::ifstream inFile;
		std::string line;
		std::vector<glm::vec2> points = {};

		inFile.open("input.txt");
		if (!inFile) {
			std::cout << "Unable to open input file 'input.txt'\n";
		}
		
		bool firstLine = true;
		while (std::getline(inFile, line)) {
			if (firstLine) {
				if (std::stoi(line) < 3) {
					std::cout << "Invalid input point set: First line is not the number of points or the number of points is < 3" << "\n";
					inFile.close();
					exit(0);
				}
				firstLine = false;
			} else {
				// Find white space seperated x and y cordinate (float) of a point on the input line
				auto whiteSpace = line.find(' ');
				float x = std::stof(line.substr(0, whiteSpace));
				float y = std::stof(line.substr(whiteSpace + 1, line.size()));
				glm::vec2 point = {x, y};
				points.push_back(point);
			}
		}

		inFile.close();
		return points;
	}

	int validatePointSet(std::vector<glm::vec2> pointSet) {
		float maxY;
		float minY;
		float maxX;
		float minX;

		// Check for duplicate points and extract min and max coordinates
		for (int i = 0; i < pointSet.size(); i++) {
			auto point = pointSet[i];

			if (point.x > maxX) {
				maxX = point.x;
			} else if (point.x < minX) {
				minX = point.x;
			}

			if (point.y > maxY) {
				maxY = point.y;
			} else if (point.y < minY) {
				minY = point.y;
			}

			for (int j = i + 1; j < pointSet.size(); j++) {
				if (point == pointSet[j]) {
					return 1;
				}
			}
		}

		// Check if origin is inside the convex hull of the point set
		if (!(maxY > 0 && minY < 0 && maxX > 0 && minX < 0)) {
			return 2;
		}
		
		/*
			Fråga Håkan: Behövs inte ifall man kollar convex hull?
		*/
		// Check if all points lie on the same line
		// bool sameLine = true;
		// for (int i = 0; i < pointSet.size(); i++) {
		// 	auto p0 = pointSet[i];
		// 	auto p1 = pointSet[i+1];
		// 	auto p2 = pointSet[i+2];

		// 	auto p0p1Slope = (p1.x - p0.x) / (p1.y - p0.y);
		// 	auto p1p2Slope = (p2.x - p1.x) / (p2.y - p1.x);

		// 	if (p0p1Slope != p1p2Slope) {
		// 		sameLine = false;
		// 		break;
		// 	}
		// }

		// if (sameLine) {
		// 	return 3;
		// }

		// point set ok
		return 0;
	}

	std::vector<glm::vec2> randomPointSet(int numPoints) {
		std::srand(std::time(NULL)); // new random seed
		std::vector<glm::vec2> set = {};
		for (int i = 0; i < numPoints; i++) {
			auto signOfX = (rand() % 2 == 1) ? -1.0f : 1.0f;
			auto signOfY = (rand() % 2 == 1) ? -1.0f : 1.0f;	
			auto x = (rand() % 100) / 100.0f * signOfX;
			auto y = (rand() % 100) / 100.0f * signOfY;		
			set.push_back(glm::vec2(x, y));
		}
		return set;
	}

	std::vector<glm::vec2> triangleSoup(std::vector<glm::vec2> pointSet) {
		std::vector<glm::vec2> cHull = convexHull(pointSet);
		
		return cHull;
	}

	std::vector<glm::vec2> convexHull(std::vector<glm::vec2> pointSet) {
		if (pointSet.size() <= 3) return pointSet;

		// Sort the point set by x-coordinate
		auto sortedSet = pointSet;
		std::sort(sortedSet.begin(), sortedSet.end(), [](const glm::vec2 &p0, const glm::vec2 &p1) {
			if (p0.x == p1.x) {
				return p0.y < p1.y;
			}
			return p0.x < p1.x;
		});

		// Calculate upper hull
		std::vector<glm::vec2> upper = {};
		for (int i = 0; i < sortedSet.size(); i++) {
			auto considered = sortedSet[i];
			// while the hull contains at least two points and the considered point lies to the left of the line through last two points
			// of the hull, pop from the hull
			while (upper.size() > 1 && leftOf(upper[upper.size() - 2], upper[upper.size() - 1], considered)) {
				upper.pop_back();
			}
			upper.push_back(considered);
		}

		// Calculate lower hull
		std::vector<glm::vec2> lower = {};
		for (int i = sortedSet.size() - 1; i  >= 0; i--) {
			auto considered = sortedSet[i];
			
			while (lower.size() > 1 && leftOf(lower[lower.size() - 2], lower[lower.size() - 1], considered)) {
				lower.pop_back();
			}
			lower.push_back(considered);
		}

		// Remove last element in each hull to not get duplicate points
		upper.pop_back();
		lower.pop_back();

		upper.insert(upper.end(), lower.begin(), lower.end());
		return upper;
	}

	/**
	 * Calculates if a point is left of a line through two points
	 * @param a point on the line
	 * @param b point on the line 
	 * @param point the point
	 **/
	bool leftOf(glm::vec2 a, glm::vec2 b, glm::vec2 point) {
		return ((b.x - a.x) * (point.y - a.y)) > ((b.y - a.y) * (point.x - a.x));
	}
}
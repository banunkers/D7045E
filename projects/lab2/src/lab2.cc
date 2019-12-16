#include "config.h"
#include "lab2.h"
#include "SDL2/SDL.h"
#include "input.h"
#include "triangle_soup.h"
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <stdlib.h>
#include <iostream>
#include <algorithm>

const GLchar* vs =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec2 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, -1, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

const GLchar* psEdge =
"#version 310 es\n"
"precision mediump float;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = vec4(0, 0, 0, 1);\n"
"}\n";

using namespace Display;
namespace Lab2 {
	bool coloring = false;
	Point redVertex;
	Point greenVertex;
	Point blueVertex;

	PointSet vertices;	// all the points
	PointSet convexHull;	// the points on the convex hull 
	PointSet triangles;	// the triangles in the 2-3 seach tree's leafs
	PointSet edges; // the edges of triangles in the leafs

	std::vector<GLfloat> vertexBuffer;

	// index buffers
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> cHullIndices;
	std::vector<unsigned int> triangleIndices;
	std::vector<unsigned int> edgeIndices;

	Lab2App::Lab2App() {}
	Lab2App::~Lab2App() {}

	/**
	 * Generates a vertex buffer given vertices with x and y coordinate.
	 * The generated vertex buffer consists for vertices with a record of size 6
	 * (2 for pos and 4 for color).
	 **/
	std::vector<GLfloat> Lab2App::genVertexBuffer() {
		auto res = std::vector<GLfloat>();
		for (auto &vertex: vertices) {
			res.push_back(vertex.x);
			res.push_back(vertex.y);
			auto vertexColor = getVertexColor(vertex);
			res.push_back(vertexColor.r); 	// r
			res.push_back(vertexColor.g); 	// g
			res.push_back(vertexColor.b); 	// b
			res.push_back(vertexColor.a); 	// a
		}
		return res;
	}

	/**
	 * Generates an index buffer consisting of all the vertices
	 **/
	std::vector<unsigned int> Lab2App::genVertexIndices() {
		auto res = std::vector<unsigned int>();
		for (int i = 0; i < vertices.size(); i++) {
			res.push_back(i);
		}
		return res;
	}


	/**
	 * Generates an index buffer consisting of the points on the convex hull (with indices of the points in the vertex buffer).
	 **/
	std::vector<unsigned int> Lab2App::genCHullIndices() {
		auto res = std::vector<unsigned int>();
		for (const auto &point: convexHull) {
			auto verticesIndex = std::find(vertices.begin(), vertices.end(), point);
			res.push_back(verticesIndex - vertices.begin()); // the index of the vertex in the vertex buffer
		}
		return res;
	}

	/**
	 * Generates an index buffer consisting of all the triangles of the triangle soup
	 **/
	std::vector<unsigned int> Lab2App::genTriangleIndices() {
		auto res = std::vector<unsigned int>();
		for (const auto &point: triangles) {
			auto verticesIndex = std::find(vertices.begin(), vertices.end(), point);
			res.push_back(verticesIndex - vertices.begin());
		}
		return res;
	}

	/**
	 * Generates and index buffer consisting of all the edges of the triangles soup
	 **/
	std::vector<unsigned int> Lab2App::genEdgeIndices() {
		auto res = std::vector<unsigned int>();
		for (const auto &point: edges) {
			auto verticesIndex = std::find(vertices.begin(), vertices.end(), point);
			res.push_back(verticesIndex - vertices.begin());
		}
		return res;
	}

	/**
	 * Calculates the vertex color
	 **/
	glm::vec4 Lab2App::getVertexColor(Point &vertex) {
		if (!coloring) {	// "default" coloring
			return glm::vec4(1.0f, 0.35f, 0.35f, 1.0f);
		} else {	// rgb with interpolation
			if (vertex == redVertex) {
				return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			} else if (vertex == greenVertex) {
				return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			} else if (vertex == blueVertex) {
				return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			} else {
				return glm::vec4(1.0f, 0.35f, 0.35f, 1.0f);
			}
		}
	}

	/**
	 * Picks three different vertices (inside the convex hull if possible)
	 **/
	void Lab2App::pickRGBVertices() {
		auto insideCHull = PointSet();
		for (const auto &vertex: vertices) {
			if (std::find(convexHull.begin(), convexHull.end(), vertex) == convexHull.end()) {
				insideCHull.push_back(vertex);
			}
		}
		if (insideCHull.size() < 3) {
			redVertex = vertices[0];
			greenVertex = vertices[1];
			blueVertex = vertices[2];
		} else {
			redVertex = insideCHull[0];
			greenVertex = insideCHull[1];
			blueVertex = insideCHull[2];
		}
	}

	/**
	 * Updates the vertex buffer and generates new index buffers
	 **/
	void Lab2App::updateBuffers() {
		if (coloring) {
			pickRGBVertices();
		}
		
		vertexBuffer = genVertexBuffer();
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(GLfloat), &vertexBuffer[0], GL_STATIC_DRAW);

		vertexIndices = genVertexIndices();
		cHullIndices = genCHullIndices();
		triangleIndices = genTriangleIndices();
		edgeIndices = genEdgeIndices();
	}

	bool Lab2App::Open() {
		App::Open();

		this->window = new Display::Window;
		this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			} else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
				coloring = !coloring;
				std::cout << "Coloring scheme set to: " << (coloring ? "default" : "rgb") << "\n";
				updateBuffers();
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
					vertices = inputPointSet;
					tie(convexHull, triangles, edges) = triangleSoup(inputPointSet);
					updateBuffers();
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
					auto randomSet = randomPointSet(numPoints);
					printf("----------------- RANDOM POINT SET GENERATED -----------------\n");
					for (const auto &point: randomSet) {
						printf("(%f, %f)\n", point.x, point.y);
					}
					printf("--------------------------------------------------------------\n");

					vertices = randomSet;
					tie(convexHull, triangles, edges) = triangleSoup(randomSet);
					updateBuffers();
				}
			}
		});
		this->window->SetTitle(std::string("Lab 2"));
		this->window->SetSize(1500, 1500);

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

			// setup pixel shaders
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(ps);
			glShaderSource(this->pixelShader, 1, &ps, &length);
			glCompileShader(this->pixelShader);

			this->pixelShaderEdge = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(psEdge);
			glShaderSource(this->pixelShaderEdge, 1, &psEdge, &length);
			glCompileShader(this->pixelShaderEdge);

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

			// create program for coloring edges
			this->programEdge = glCreateProgram();
			glAttachShader(this->programEdge, this->vertexShader);
			glAttachShader(this->programEdge, this->pixelShaderEdge);
			glLinkProgram(this->programEdge);
			glGetProgramiv(this->programEdge, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->programEdge, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}
			
			// setup vbo
			glGenBuffers(1, &this->vbo);
			glUseProgram(this->program);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 6, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 6, (GLvoid*)(sizeof(float32) * 2));

			// setup ibo
			glGenBuffers(1, &this->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			glPointSize(10);
			glLineWidth(3);

			// enable alpha
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			return true;
		}
		return false;
	}

	void Lab2App::Run() {
		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT);
			this->window->Update();

			glUseProgram(this->program);

			// Draw points
			// glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);
			// glDrawElements(GL_POINTS, vertexIndices.size(), GL_UNSIGNED_INT, (void*)0);

			// Draw convex hull
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, cHullIndices.size() * sizeof(unsigned int), &cHullIndices[0], GL_STATIC_DRAW);
			glDrawElements(GL_LINE_LOOP, cHullIndices.size(), GL_UNSIGNED_INT, (void*)0);

			// Draw triangles
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndices.size() * sizeof(unsigned int), &triangleIndices[0], GL_STATIC_DRAW);
			glDrawElements(GL_TRIANGLES, triangleIndices.size(), GL_UNSIGNED_INT, (void*)0);

			// Draw edges (in order to be able to see the different triangles in the soup)
			glUseProgram(this->programEdge);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(unsigned int), &edgeIndices[0], GL_STATIC_DRAW);
			glDrawElements(GL_LINES, edgeIndices.size(), GL_UNSIGNED_INT, (void*)0);
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			this->window->SwapBuffers();
		}
	}
}
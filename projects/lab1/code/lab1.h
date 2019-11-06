#pragma once

#include "core/app.h"
#include "render/window.h"

namespace Lab1
{
class Lab1App : public Core::App
{
public:
	/// constructor
	Lab1App();
	/// destructor
	Lab1App();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
};
}
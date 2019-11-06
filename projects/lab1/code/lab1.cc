#include "config.h"
#include "lab1.h"

using namespace Display;
namespace Lab1 
{
	Lab1App::Lab1App()
	{

	}

	Lab1App::~Lab1App()
	{

	}


bool 
Lab1App::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

}

void
Lab1App::Run()
{

}

}
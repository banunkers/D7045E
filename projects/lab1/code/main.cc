#include "config.h"
#include "lab1.h"

int main(int argc, const char** argv) {
	Lab1::Lab1App app;
	
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
#include "config.h"
#include "lab2.h"

int main(int argc, const char** argv) {
	Lab2::Lab2App app;
	
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
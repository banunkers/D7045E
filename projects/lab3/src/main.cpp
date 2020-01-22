#include "config.h"
#include "lab3.h"

int main(int argc, const char** argv) {
	Lab3 app;
	
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
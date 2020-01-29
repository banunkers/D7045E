#include "config.h"
#include "lab4.h"

int main(int argc, const char** argv) {
	Lab4 app;
	
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
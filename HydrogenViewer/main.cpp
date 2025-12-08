#include <iostream>
#include "Application/Application.h"

int main (int argc, char** arvg)
{
	HydrogenViewer::Application* App = new HydrogenViewer::Application;
	App->Run();
	delete App;
    return 0;
}
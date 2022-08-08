#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "vec.h"
#include "mat.h"
#include "window.h"
#include <iostream>;

int main()
{
	Window* window = new Window(800, 600);
	window->init();
	window->start();
	delete window;
}

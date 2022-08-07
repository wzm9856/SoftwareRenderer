#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "vec.h"
#include "mat.h"
#include <iostream>;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void line(vec2 a, vec2 b, unsigned char* output) {
    for (float t = 0; t < 1; t += 0.01) {
        int x = a.x() + t * (b.x() - a.x());
        int y = a.y() + t * (b.y() - a.y());
        return;
    }
}

void render(unsigned char* output) {
    vec3 a(1, 2, 3);
}



int main()
{

}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
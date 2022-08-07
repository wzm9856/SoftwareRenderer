#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>;
#include "shader.h"
#include "mat.h"
#include "vec.h"


void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
    Window* curWindow = Window::getCurWindow();
    curWindow->fov *= pow(1.1, curWindow->oldMouseZ - yoffset);
    curWindow->fov = curWindow->fov < 10 ? 10 : curWindow->fov>160 ? 160 : curWindow->fov;
    curWindow->oldMouseZ = yoffset;
}
class Window {
public:
    Window(size_t width, size_t height) :_w(width), _h(height) {
        curWindow = this;
    };
    void* init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(_w, _h, "mihoyo", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }
        ourShader = Shader("vertex.vs", "fragment.fs");
        float vertices[] = {
            // positions          // colors           // texture coords
             1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture"), 0);

        this->colorBuffer = new unsigned char[_w * _h * 3];
        if (!this->colorBuffer) exit(-2);
        this->depthBuffer = new double[_w * _h];
        if (!this->depthBuffer) exit(-2);
        glfwSetScrollCallback(window, scrollCallBack);

        groundScale[0][0] = 30;
        groundScale[1][1] = 1;
        groundScale[3][1] = 0;//平面和石头的距离
        groundScale[2][2] = 30;
        groundScale[3][3] = 1;
    }
    void clearBuffer() {
        std::fill_n(colorBuffer, _h * _w * 3, 0);
        std::fill_n(depthBuffer, _h * _w, 1.0);
    }
    void start() {
        while (!glfwWindowShouldClose(window))
        {
            getM();
            getVP();
            render(data);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _h, _w, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            ourShader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
            clearBuffer();
        }
    }
    void getM() {
        // 处理输入，生成M矩阵
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !buttonDown) {
            buttonDown = true;
            glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
            rotateX = 0;
            rotateY = 0;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double curX, curY;
            glfwGetCursorPos(window, &curX, &curY);
            rotateX += (curX - oldMouseX) / 150.0;
            rotateY += (curY - oldMouseY) / 150.0;
            oldMouseX = curX;
            oldMouseY = curY;
            tmpModel = rotateXY(rotateY, rotateX);
            model = tmpModel * preModel;
        }
        if (buttonDown && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
            buttonDown = false;
            preModel = model;
        }
        groundModel = model * groundScale;
    }
    void getVP() {
        //处理输入生成VP矩阵
        float sensitivity = 0.3;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            eye.z() += sensitivity;
            at.z() += sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            eye.x() -= sensitivity;
            at.x() -= sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            eye.z() -= sensitivity;
            at.z() -= sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            eye.x() += sensitivity;
            at.x() += sensitivity;
        }
        // 生成光源VP矩阵
        {
            lightView = lookAt(vec3(lighteye), vec3(lightat));
            lightProj = perspective(70, 1, 10, 50);
            lightMtx = lightProj * lightView;
        }
        // 生成相机VP矩阵
        {
            view = lookAt(vec3(eye), vec3(at));
            proj = perspective(fov, _w / _h, 0.1, 2000);
        }
    }
    ~Window() {
        delete[] colorBuffer;
        delete[] depthBuffer;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glfwTerminate();
    }
    static Window* getCurWindow() {
        return curWindow;
    }
    friend void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
private:
	size_t _w, _h;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;
    Shader ourShader;
    GLFWwindow* window;

    double  oldMouseX = 0;
    double  oldMouseY = 0;
    int     oldMouseZ = 0;
    float	rotateX = 0; // 是对于屏幕的xy
    float	rotateY = 0;
    float	fov = 60;
    bool	buttonDown = false;
    mat4	model = ones<4,4>(); // 无论什么情况，最终设置给shader的M矩阵都是m_model
    mat4	tmpModel = ones<4, 4>(); //当鼠标按下没松开时，把m_tmpModel设置成当前拖动旋转的矩阵，乘上按下鼠标之前的m_model（被复制到了m_preModel），赋给了m_model
    mat4	preModel = ones<4, 4>(); //当鼠标松开时，把m_model复制给m_premodel
    mat4	view = mat4(0);
    mat4	proj = mat4(0);
    mat4	identity = ones<4, 4>(); //保持他identity的身份，做一个工具人
    mat4	groundScale = mat4(0); // 用于拉伸地面模型
    mat4	groundModel = mat4(0);
    mat4	lightView = mat4(0);
    mat4	lightProj = mat4(0);
    mat4	lightMtx = mat4(0);
    mat3	normalMtx = ones<3, 3>();
    vec4    at = vec4(0); //at是视点，eye是相机坐标
    vec4    eye = vec4( 0.0f, 0.0f, -20.0f, 0.0f );
    vec4    lightat = vec4(0.0f, 0.0f, 0.0f, 0.0f ); //at是视点，eye是光源坐标
    vec4    lighteye = vec4(-20.0f, -20.0f, -20.0f, 0.0f );
    vec4    lightrgb = vec4(1000,1000,1000,0 );

    unsigned char* colorBuffer = nullptr;
    double* depthBuffer = nullptr;

    static Window* curWindow;
};

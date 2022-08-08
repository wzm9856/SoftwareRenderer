#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>;
#include "shader.h"
#include "mat.h"
#include "vec.h"

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void showFPS(GLFWwindow* window);
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
        //stbi_set_flip_vertically_on_load(true);

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
            render();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _w, _h, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            ourShader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            showFPS(window);
            glfwPollEvents();
            clearBuffer();
        }
    }
    void getM();
    void getVP();
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
    void setColor(size_t x, size_t y, const vec3& color) {
        //assert(x < _h);
        //assert(y < _w);
        if (x >= _h || y >= _w)
            return;
        colorBuffer[(y + x * _w) * 3] = color.x() * 255;
        colorBuffer[(y + x * _w) * 3 + 1] = color.y() * 255;
        colorBuffer[(y + x * _w) * 3 + 2] = color.z() * 255;
    }
    void render();
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
    mat4	cubeModel = ones<4,4>(); //最终设置给shader的M矩阵
    mat4	tmpModel = ones<4, 4>(); //当鼠标按下没松开时，把tmpModel设置成当前拖动旋转的矩阵，乘上按下鼠标之前的cubeModel（被复制到了preModel），赋给了cubeModel
    mat4	preModel = ones<4, 4>(); //当鼠标松开时，把model复制给premodel
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

    Vertex vList[3] = { Vertex(-1, 1, 1,0,0,0,0,0),
                        Vertex(1, 1, 1,0,0,0,0,0),
                        Vertex(-1,-1, 1,0,0,0,0,0) };

    unsigned char* colorBuffer = nullptr;
    double* depthBuffer = nullptr;

    static Window* curWindow;
};
Window* Window::curWindow = nullptr;

void Window::getM() {
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
        tmpModel = rotateXY(rotateX, rotateY);
        cubeModel = tmpModel * preModel;
    }
    if (buttonDown && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        buttonDown = false;
        preModel = cubeModel;
    }
    groundModel = cubeModel * groundScale;
}

void Window::getVP() {
    //处理输入生成VP矩阵
    float sensitivity = 0.1;
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
        proj = perspective(fov, _w / _h, 1, 500);
    }
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
    Window* curWindow = Window::getCurWindow();
    curWindow->fov *= pow(1.1, - yoffset);
    curWindow->fov = curWindow->fov < 10 ? 10 : curWindow->fov>160 ? 160 : curWindow->fov;
}

void line(const vec2& a, const vec2& b, const vec3& color, Window* window) {
    bool steep = false;
    int x0 = a.x(), x1=b.x(), y0=a.y(), y1=b.y();
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2; 
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            window->setColor(y, x, color); 
        } else {
            window->setColor(x, y, color);
        }
        error2 += derror2; 
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

void triangle(vec2 a, vec2 b, vec2 c, const vec3& color, Window* window) {
    if (a.y() < b.y()) std::swap(a, b);
    if (a.y() < c.y()) std::swap(a, c);
    if (b.y() < c.y()) std::swap(b, c);
    int bax = b.x() - a.x();
    int aby = a.y() - b.y();
    int cax = c.x() - a.x();
    int acy = a.y() - c.y();
    int cbx = c.x() - b.x();
    int bcy = b.y() - c.y();
    // 下半
    for (int y = b.y(); y < a.y(); y++) {
        //double left_ratio = (a.y() - y) / (a.y() - c.y());
        //int leftx = a.x() + left_ratio * (c.x() - a.x());
        //double right_ratio = (a.y() - y) / (a.y() - b.y());
        //int rightx = a.x() + left_ratio * (b.x() - a.x());
        double left_ratio = (a.y() - y) / acy;
        int leftx = a.x() + left_ratio * cax;
        double right_ratio = (a.y() - y) / aby;
        int rightx = a.x() + right_ratio * bax;
        for (int i = std::min(leftx, rightx); i <= std::max(leftx, rightx); i++)
            window->setColor(i, y, color);
    }
    // 上半
    for (int y = b.y(); y > c.y(); y--) {
        double left_ratio = (a.y() - y) / acy;
        int leftx = a.x() + left_ratio * cax;
        double right_ratio = (b.y() - y) / bcy;
        int rightx = b.x() + right_ratio * cbx;
        for (int i = std::min(leftx, rightx); i <= std::max(leftx, rightx); i++)
            window->setColor(i, y, color);
    }
    line(a, b, color, window);
    line(b, c, color, window);
    line(c, a, color, window);
}

void triangle(vec3 a, vec3 b, vec3 c, const vec3& color, Window* window) {
    if (a.y() < b.y()) std::swap(a, b);
    if (a.y() < c.y()) std::swap(a, c);
    if (b.y() < c.y()) std::swap(b, c);
    int bax = b.x() - a.x();
    int aby = a.y() - b.y()+1;
    int cax = c.x() - a.x();
    int acy = a.y() - c.y()+1;
    int cbx = c.x() - b.x();
    int bcy = b.y() - c.y()+1;
    // 下半
    for (int y = b.y(); y < a.y(); y++) {
        //double left_ratio = (a.y() - y) / (a.y() - c.y());
        //int leftx = a.x() + left_ratio * (c.x() - a.x());
        //double right_ratio = (a.y() - y) / (a.y() - b.y());
        //int rightx = a.x() + left_ratio * (b.x() - a.x());
        double left_ratio = (a.y() - y) / acy;
        int leftx = a.x() + left_ratio * cax;
        double right_ratio = (a.y() - y) / aby;
        int rightx = a.x() + right_ratio * bax;
        for (int i = std::min(leftx, rightx); i <= std::max(leftx, rightx); i++)
            window->setColor(i, y, color);
    }
    // 上半
    for (int y = b.y(); y > c.y(); y--) {
        double left_ratio = (a.y() - y) / acy;
        int leftx = a.x() + left_ratio * cax;
        double right_ratio = (b.y() - y) / bcy;
        int rightx = b.x() + right_ratio * cbx;
        for (int i = std::min(leftx, rightx); i <= std::max(leftx, rightx); i++)
            window->setColor(i, y, color);
    }
    line(a, b, color, window);
    line(b, c, color, window);
    line(c, a, color, window);
}

void vertexShader(Vertex* vList, size_t vSize, const mat4& mvp, size_t _w, size_t _h, vec3* output) {
    size_t halfw = _w / 2;
    size_t halfh = _h / 2;
    for (size_t i = 0; i < vSize; i++) {
        output[i] = homotoscreen(mvp * tohomo(vList[i].pos), halfw, halfh);
    }
}

void Window::render() {
    mat4 mvp = proj * view * cubeModel;
    //Vertex vList[8] = { Vertex(-1, 1, 1,0,0,0,0,0),
    //                    Vertex( 1, 1, 1,0,0,0,0,0),
    //                    Vertex(-1,-1, 1,0,0,0,0,0),
    //                    Vertex( 1,-1, 1,0,0,0,0,0),
    //                    Vertex(-1, 1,-1,0,0,0,0,0),
    //                    Vertex( 1, 1,-1,0,0,0,0,0),
    //                    Vertex(-1,-1,-1,0,0,0,0,0),
    //                    Vertex( 1,-1,-1,0,0,0,0,0)};
    vec3 screenPos[3];
    vertexShader(vList, sizeof(vList) / sizeof(vList[0]), mvp, _w, _h, screenPos);
    uint16_t triList[] = { 0,1,2 };
    size_t triListSize = sizeof(triList) / sizeof(triList[0]) / 3;
    for (size_t i = 0; i < triListSize; i++) {
        triangle(screenPos[triList[i * 3]], screenPos[triList[i * 3 + 1]], screenPos[triList[i * 3 + 2]], vec3(1,1,1), this);
    }
}

void showFPS(GLFWwindow* window) {
    static double lastTime = glfwGetTime();
    static int frameCount = 0;
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    frameCount++;
    if (delta >= 0.5) {
        double fps = double(frameCount) / delta;
        std::stringstream ss;
        std::string title = "mihoyo [FPS:" + std::to_string(fps) + "]";
        glfwSetWindowTitle(window, title.c_str());
        frameCount = 0;
        lastTime = currentTime;
    }
}
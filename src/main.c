#include <stdio.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

/* forward definitions */
void error_cb(int code, const char* description);
void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods);

FILE* logfile;

/* main function, with main loop */
int main()
{
    /* clear logfile */
    {
        logfile = fopen("log.txt", "w");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(logfile, "subterra log %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        fclose(logfile);
    }
    /* glfw & gl init */
    glfwInit();
    glfwSetErrorCallback(error_cb);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "subterra", NULL, NULL);
    glfwSetKeyCallback(window, key_cb);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); /* vsync */
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_CULL_FACE);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwSetErrorCallback(NULL);
    glfwTerminate();
    return 0;
}

void error_cb(int code, const char* description)
{
    logfile = fopen("log.txt", "a+");
    fprintf(logfile, "ERROR IN GLFW: CODE %i, %s\n", code, description);
    fclose(logfile);
}

char fs = 0;
int w=640,h=480,x=0,y=0;
void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    #ifndef __APPLE__
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        if(fs)
        {
            glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowSize(window, w, h);
            glfwSetWindowPos(window, x, y);
            fs = 0;
        } else {
            glfwGetWindowSize(window, &w, &h);
            glfwGetWindowPos(window, &x, &y);
            glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowSize(window, mode->width, mode->height);
            glfwSetWindowPos(window, 0, 0);
            fs = 1;
        }
    }
    #endif
}
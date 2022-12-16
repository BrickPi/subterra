#include "subterra/util/callback.h"

void error_cb(int code, const char* description) { char* s = NULL; sprintf(s, "ERROR IN GLFW: CODE %i, %s\n", code, description); logger_log(s); }

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
        return;
    }
    #endif
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, NULL);
        lostfocus();
    }
}

void fb_cb(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
    glm_perspective(glm_rad(45.0f), 640/480, 0.1f, 100.0f, proj);
}

void click_cb(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_input);
            return;
        }
    }
}
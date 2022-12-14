#include "subterra/util/window.h"

void window_init()
{
    /* create window with OpenGL Core 3.3 */
    glfwInit();
    glfwSetErrorCallback(error_cb);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    /* setup for 4XMSAA */
    glfwWindowHint(GLFW_SAMPLES, 4);
    /* actual window creation */
    window = glfwCreateWindow(640, 480, "subterra", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); /* vsync */
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetKeyCallback(window, key_cb);
    glfwSetFramebufferSizeCallback(window, fb_cb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetMouseButtonCallback(window, click_cb);
    glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

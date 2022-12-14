#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "subterra/util/callback.h"
#ifndef _H_SUBTERRA_WINDOW
#define _H_SUBTERRA_WINDOW
GLFWwindow* window;
unsigned int CUBEV, VAO, PLANEV;
void window_init();
void window_gl_init();
#endif
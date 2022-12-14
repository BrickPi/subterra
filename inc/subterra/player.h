#ifndef _H_SUBTERRA_PLAYER
#define _H_SUBTERRA_PLAYER
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
void player_input(GLFWwindow* win, float dt);
void mouse_input(GLFWwindow* window, double xpos, double ypos);
void lostfocus();
extern mat4* update_camera();
#endif
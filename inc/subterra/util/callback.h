#ifndef _H_SUBTERRA_CALLBACK
#define _H_SUBTERRA_CALLBACK
void error_cb(int code, const char* description);
void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods);
void fb_cb(GLFWwindow* window, int width, int height);
void click_cb(GLFWwindow* window, int button, int action, int mods);
#endif
#include "subterra/util/window.h"
#include "subterra/gfx/geometry.i"

GLFWwindow* window;
unsigned int CUBEV, VAO, PLANEV;

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
    /* set all the callbacks for input etc */
    glfwSetKeyCallback(window, key_cb);
    glfwSetFramebufferSizeCallback(window, fb_cb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetMouseButtonCallback(window, click_cb);
}

void window_gl_init()
{
    /* initialize OpenGL with our geometry etc */
    glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    /* init the vertex array object */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &CUBEV);
    /* cube */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, CUBEV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    /* position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* texcoord attribute */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    /* plane */
    glGenBuffers(1, &PLANEV);
    glBindBuffer(GL_ARRAY_BUFFER, PLANEV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
}

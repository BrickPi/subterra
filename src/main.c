#include <stdio.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "cglm/cglm.h"

#include "subterra/shader.h"
#include "subterra/player.h"

/* forward definitions */
void error_cb(int code, const char* description);
void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods);
void fb_cb(GLFWwindow* window, int width, int height);
void click_cb(GLFWwindow* window, int button, int action, int mods);

float vertices[] = {
   /*positions            texture coords */
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float plane[] = {
    0.5,0.5,0,
    0.5,-0.5,0
    -0.5,-0.5,0,
    -0.5,0.5,0
};

FILE* logfile;
mat4 proj;

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
    
    /* basic glfw & gl init */
    GLFWwindow* window;
    {
        glfwInit();
        glfwSetErrorCallback(error_cb);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        window = glfwCreateWindow(640, 480, "subterra", NULL, NULL);
        glfwSetKeyCallback(window, key_cb);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); /* vsync */
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glViewport(0, 0, 640, 480);
        glfwSetFramebufferSizeCallback(window, fb_cb);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetMouseButtonCallback(window, click_cb);
    }
    
    /* setup primitives */
    unsigned int VBO, VAO, PVBO, PVAO;
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        /* position attribute */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        /* texcoord attribute */
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        /* plane */
        glGenBuffers(1, &PVBO);
        glBindBuffer(GL_ARRAY_BUFFER, PVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
    }
    
    /* textures & shaders */
    shader_load();
    unsigned int texture, floortex;
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load("res/container.jpg", &width, &height, &nrChannels, 4);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            logfile = fopen("log.txt", "a+");
            fprintf(logfile, "ERROR LOADING TEXTURE: res/container.jpg!\n");
            fclose(logfile);
        }
        stbi_image_free(data);
        /* cobblestone */
        glGenTextures(1, &floortex);
        glBindTexture(GL_TEXTURE_2D, floortex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        data = stbi_load("res/cobblestone.jpg", &width, &height, &nrChannels, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    /* all the matrix stuff */
    mat4 model = GLM_MAT4_IDENTITY_INIT; /* each individual object has one of these! */
    mat4 floormodel = GLM_MAT4_IDENTITY_INIT;
    glm_translate(floormodel, (vec3){0,-2.5,0});
    glm_rotate(floormodel, glm_rad(90), GLM_XUP);
    glm_scale(floormodel, (vec3){2.5,8,1});
    /* view matrix is done in player.c */
    glm_perspective(glm_rad(45.0f), 640/480, 0.1f, 100.0f, proj);

    /* main loop */
    float lastFrame = 0, delta;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;
        player_input(window, delta);

        glActiveTexture(GL_TEXTURE0);
        shader_use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glm_rotate(model, glm_rad(1.0f), (vec3){0.5f,1.0f,0.0f});
        shader_uniforms(&proj, update_camera(), &model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        shader_uniforms(&proj, update_camera(), &floormodel);
        glBindBuffer(GL_ARRAY_BUFFER, PVBO);
        glBindTexture(GL_TEXTURE_2D, floortex);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_clean();
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
        return;
    }
    #endif
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_input);
            return;
        }
    }
}

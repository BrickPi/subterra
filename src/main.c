#include <stdio.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "cglm/cglm.h"

#include "subterra/shader.h"

/* forward definitions */
void error_cb(int code, const char* description);
void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods);
void fb_cb(GLFWwindow* window, int width, int height);

float vertices[] = {
   /*positions            texture coords */
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, /* top right */
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, /* bottom right */
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, /* bottom left */
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  /* top left */
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

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
    
    /* basic glfw & gl init */
    GLFWwindow* window;
    {
        glfwInit();
        glfwSetErrorCallback(error_cb);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        window = glfwCreateWindow(640, 480, "subterra", NULL, NULL);
        glfwSetKeyCallback(window, key_cb);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); /* vsync */
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glViewport(0, 0, 640, 480);
        glfwSetFramebufferSizeCallback(window, fb_cb);
    }
    
    /* setup primitives */
    unsigned int VBO, VAO, EBO;
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        /* position attribute */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        /* texcoord attribute */
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    
    /* textures & shaders */
    shader_load();
    unsigned int texture;
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
    }

    mat4 trans;
    glm_mat4_identity(trans);
    glm_rotate(trans, glm_rad(90.0f), (vec3){0.0f,0.0f,1.0f});
    shader_transform(&trans);

    /* main loop */
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader_use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
    }
    #endif
}

void fb_cb(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

#define STB_IMAGE_IMPLEMENTATION
#include "subterra/main.h"

/* main function, with main loop */
int main()
{
    logger_init(); /* see logger.c */
    window_init(); /* see window.c */
    window_gl_init(); /* see window.c */
    
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
        if(!data) { logger_log("ERROR LOADING TEXTURE: res/container.jpg!\n"); }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
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
        glBindBuffer(GL_ARRAY_BUFFER, CUBEV);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        shader_uniforms(&proj, update_camera(), &floormodel);
        glBindBuffer(GL_ARRAY_BUFFER, PLANEV);
        glBindTexture(GL_TEXTURE_2D, floortex);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &CUBEV);
    glDeleteBuffers(1, &PLANEV);
    shader_clean();
    glfwSetErrorCallback(NULL);
    glfwTerminate();
    return 0;
}

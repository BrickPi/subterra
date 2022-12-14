#include "subterra/main.h"

mat4 proj;

/* main function, with main loop */
int main()
{
    logger_init(); /* see logger.c */
    window_init(); /* see window.c */
    window_gl_init(); /* see window.c */
    
    /* textures & shaders */
    shader_load();
    unsigned int boxtex = create_texture("res/container.jpg", false);
    unsigned int floortex = create_texture("res/cobblestone.jpg", false);

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
        glBindTexture(GL_TEXTURE_2D, boxtex);
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

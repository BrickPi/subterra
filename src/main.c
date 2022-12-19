#include "subterra/main.h"

mat4 proj;
char gameActive = 0, won = 0;

/* main function, with main loop */
int main()
{
    logger_init(); /* see logger.c */
    window_init(); /* see window.c */
    window_gl_init(); /* see window.c */
    
    /* textures & shaders */
    shader_load();
    unsigned int floortex = create_texture("res/cobblestone.jpg", 0);
    unsigned int walltex = create_texture("res/wall.jpg", 0);
    unsigned int finishtex = create_texture("res/win.jpg", 0);
    unsigned int startui = create_texture("res/ui.png", 1);
    unsigned int winui = create_texture("res/winui.png", 1);

    /* all the matrix stuff */
    /* each individual object has a model! */
    mat4 floormodel = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(floormodel, glm_rad(90), GLM_XUP);
    glm_scale(floormodel, (vec3){2.5,2.5,1});
    mat4 Xwallmodel = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(Xwallmodel, glm_rad(90), GLM_XUP);
    glm_rotate(Xwallmodel, glm_rad(90), GLM_YUP);
    glm_rotate(Xwallmodel, glm_rad(90), GLM_ZUP);
    glm_scale(Xwallmodel, (vec3){2.5,3.5,1});
    mat4 Zwallmodel = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(Zwallmodel, glm_rad(180), GLM_XUP);
    glm_scale(Zwallmodel, (vec3){2.5,3.5,1});
    mat4 finishModel = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(finishModel, glm_rad(180), GLM_XUP);
    glm_scale(finishModel, (vec3){2.5,3.5,1});
    glm_translate(finishModel, (vec3){-7,0.075,-15.75});
    mat4 uimodel = GLM_MAT4_IDENTITY_INIT;
    glm_scale(uimodel, (vec3){2,2,1});

    /* view matrix is done in player.c */
    glm_perspective(glm_rad(45.0f), 640/480, 0.1f, 100.0f, proj);

    /* main loop */
    float lastFrame = 0, delta;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (won)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);
            shader_use_flat();
            mat4 identity = GLM_MAT4_IDENTITY_INIT;
            shader_uniforms(&identity, &identity, &uimodel);
            glBindBuffer(GL_ARRAY_BUFFER, PLANEV);
            glBindTexture(GL_TEXTURE_2D, winui);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else if (gameActive)
        {
            float currentFrame = glfwGetTime();
            delta = currentFrame - lastFrame;
            lastFrame = currentFrame;
            player_input(window, delta);
            mat4* view = update_camera();

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);
            shader_use_instanced();
            shader_uniforms(&proj, view, &floormodel);
            set_instance_uniform(105, floors);
            glBindBuffer(GL_ARRAY_BUFFER, PLANEV);
            glBindTexture(GL_TEXTURE_2D, floortex);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 105);

            /* draw all X walls */
            shader_uniforms(&proj, view, &Xwallmodel);
            set_instance_uniform(114, Xwalls);
            glBindTexture(GL_TEXTURE_2D, walltex);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 114);

            /* draw all Z walls */
            shader_uniforms(&proj, view, &Zwallmodel);
            set_instance_uniform(88, Zwalls);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 88);
            shader_use_flat();
            shader_uniforms(&proj, view, &finishModel);
            glBindTexture(GL_TEXTURE_2D, finishtex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);
            shader_use_flat();
            mat4 identity = GLM_MAT4_IDENTITY_INIT;
            shader_uniforms(&identity, &identity, &uimodel);
            glBindBuffer(GL_ARRAY_BUFFER, PLANEV);
            glBindTexture(GL_TEXTURE_2D, startui);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
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

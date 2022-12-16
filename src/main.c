#include "subterra/main.h"

mat4 proj;
#include "subterra/gfx/maze.i"

/* main function, with main loop */
int main()
{
    logger_init(); /* see logger.c */
    window_init(); /* see window.c */
    window_gl_init(); /* see window.c */
    
    /* textures & shaders */
    shader_load();
    unsigned int floortex = create_texture("res/cobblestone.jpg");
    unsigned int walltex = create_texture("res/wall.jpg");

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
        mat4* view = update_camera();

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        shader_use_instanced();
        shader_uniforms(&proj, view, &floormodel);
        set_instance_uniform(95, floors);
        glBindBuffer(GL_ARRAY_BUFFER, PLANEV);
        glBindTexture(GL_TEXTURE_2D, floortex);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 95);

        /* draw all X walls */
        shader_uniforms(&proj, view, &Xwallmodel);
        set_instance_uniform(108, Xwalls);
        glBindTexture(GL_TEXTURE_2D, walltex);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 108);

        /* draw all Z walls */
        shader_uniforms(&proj, view, &Zwallmodel);
        set_instance_uniform(75, Zwalls);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 75);

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

/*
maze design
15x15

W W W W W W W W W W W W W W W
W W D D D D D D D D D D D D D
D W D W W W W D D W W W W W D
D D D D D D W _ _ W D D D W D
W W D W W W W D D D D W D W D
W W D D D D D D W W D W W W D
W D D W W W W W W W D W W D D
W W D W D D D X D D D W D D W <- X is spawn (0, 0) (D is for DONE)
W D W W D W W W W W D W W D W
W D D D D D D D D W D W W D  
W W D W W W D W D D D W   W  
W W D W W W D D W W W W   W  
W W D W W W W W W W D D      
W W D D D D D D D D D W W   W
W W W W W W W D W W W W W   E <- E is for exit!
*/

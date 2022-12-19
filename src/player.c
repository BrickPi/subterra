#include "subterra/player.h"

mat4* update_camera();

vec3 camera_pos = {0,0,0};
vec3 camera_front = {0,0,-1};
vec3 camera_up = {0,1,0};

mat4 view = GLM_MAT4_IDENTITY_INIT;

void player_input(GLFWwindow* win, float dt)
{
    vec3 temp;

    const float speed = 5.0f * dt;
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm_vec3_scale(camera_front, speed, temp);
        glm_vec3_add(camera_pos, temp, camera_pos);
    }
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm_vec3_scale(camera_front, speed, temp);
        glm_vec3_sub(camera_pos, temp, camera_pos);
    }
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm_cross(camera_front, camera_up, temp);
        glm_normalize(temp);
        glm_vec3_scale(temp, speed, temp);
        glm_vec3_sub(camera_pos, temp, camera_pos);
    }
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm_cross(camera_front, camera_up, temp);
        glm_normalize(temp);
        glm_vec3_scale(temp, speed, temp);
        glm_vec3_add(camera_pos, temp, camera_pos);
    }
    /* keep player at ground level */
    camera_pos[1] = 0;

    /* do collision detection and resolution */
    for (int i = 0; i < 114; i++) /* X walls */
    {
        /*if (camera_pos[0] == Xwalls[i][0] || camera_pos[2] == Xwalls[i][2])
        {
            camera_pos[0] = Xwalls[i][0];
            camera_pos[2] = Xwalls[i][2];
            logger_log("COLLISION!\n");
        }*/
        
    }
    for (int i = 0; i < 88; i++) /* Z walls */
    {
        /*if (camera_pos[0] == Zwalls[i][0] && camera_pos[2] == Zwalls[i][2])
        {
            camera_pos[0] = Zwalls[i][0];
            camera_pos[2] = Zwalls[i][2];
        }*/
    }
    
    
}

float lastX=320,lastY=240,yaw=-90,pitch=0; /* start at half of default resolution, centre */
bool firstMouse = true;
void mouse_input(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction[1] = sin(glm_rad(pitch));
    direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize(direction);
    glm_vec3_copy(direction, camera_front);
}

void lostfocus()
{
    firstMouse = true;
}

mat4* update_camera()
{
    vec3 pf;
    glm_vec3_add(camera_pos, camera_front, pf);
    glm_lookat(camera_pos, pf, camera_up, view);
    return &view;
}
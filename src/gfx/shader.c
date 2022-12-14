#include "subterra/gfx/shader.h"

unsigned int shaderProgram;

const char* vertexShaderSource = "#version 330 core\nlayout (location=0) in vec3 aPos;layout (location=1) in vec2 aTexCoord;out vec2 TexCoord;uniform mat4 model;uniform mat4 view;uniform mat4 proj;void main(){gl_Position=proj*view*model*vec4(aPos,1.0);TexCoord=vec2(aTexCoord.x,aTexCoord.y);}\0";
const char* fragmentShaderSource = "#version 330 core\nout vec4 FragColor;in vec2 TexCoord;uniform sampler2D tex;void main(){FragColor=texture(tex,TexCoord);}\0";

void shader_load()
{
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
        logger_log("ERROR COMPILING VERTEX SHADER!\n");
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
        logger_log("ERROR COMPILING FRAGMENT SHADER!\n");
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
        logger_log("ERROR LINKING SHADER PROGRAM!\n");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shader_use();
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
}

void shader_use()
{
    glUseProgram(shaderProgram);
}

void shader_clean()
{
    glUseProgram(0);
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
}

void shader_uniforms(mat4* proj, mat4* view, mat4* model)
{
    unsigned int loc = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)proj);
    loc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)view);
    loc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)model);
}
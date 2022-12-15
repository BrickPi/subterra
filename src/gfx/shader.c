#include "subterra/gfx/shader.h"

unsigned int shaderProgram, instance_sp;

const char* vertexShaderSource = "#version 330 core\nlayout (location=0) in vec3 aPos;layout (location=1) in vec2 aTexCoord;out vec2 TexCoord;uniform mat4 model;uniform mat4 view;uniform mat4 proj;void main(){gl_Position=proj*view*model*vec4(aPos,1.0);TexCoord=vec2(aTexCoord.x,aTexCoord.y);}\0";
const char* fragmentShaderSource = "#version 330 core\nout vec4 FragColor;in vec2 TexCoord;uniform sampler2D tex;void main(){FragColor=texture(tex,TexCoord);}\0";

const char* instanceVertexSource = "#version 330 core\nlayout (location=0) in vec3 aPos;layout (location=1) in vec2 aTexCoord;out vec2 TexCoord;uniform mat4 model;uniform mat4 view;uniform mat4 proj;uniform vec3 posarr[512];void main(){vec3 offset=posarr[gl_InstanceID];gl_Position=proj*view*model*vec4(aPos+offset,1.0);TexCoord=vec2(aTexCoord.x,aTexCoord.y);}\0";

void shader_load()
{
    unsigned int vertexShader, instanceVertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    instanceVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(instanceVertexShader, 1, &instanceVertexSource, NULL);
    glCompileShader(instanceVertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    instance_sp = glCreateProgram();
    glAttachShader(instance_sp, instanceVertexShader);
    glAttachShader(instance_sp, fragmentShader);
    glLinkProgram(instance_sp);
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
        logger_log("ERROR COMPILING VERTEX SHADER!\n");
    glGetShaderiv(instanceVertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
        logger_log("ERROR COMPILING INSTANCE VERTEX SHADER!\n");
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
        logger_log("ERROR COMPILING FRAGMENT SHADER!\n");
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
        logger_log("ERROR LINKING SHADER PROGRAM!\n");
    glGetProgramiv(instance_sp, GL_LINK_STATUS, &success);
    if(!success)
        logger_log("ERROR LINKING INSTANCE SHADER PROGRAM!\n");
    glDeleteShader(vertexShader);
    glDeleteShader(instanceVertexShader);
    glDeleteShader(fragmentShader);
    shader_use_flat();
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    shader_use_instanced();
    glUniform1i(glGetUniformLocation(instance_sp, "tex"), 0);
}

void set_instance_uniform(int count, vec3* arr)
{
    glUniform3fv(glGetUniformLocation(instance_sp, "posarr[0]"), count, (const float*)arr);
}

void shader_use_flat()
{
    glUseProgram(shaderProgram);
}

void shader_use_instanced()
{
    glUseProgram(instance_sp);
}

void shader_clean()
{
    glUseProgram(0);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(instance_sp);
    shaderProgram = 0;
    instance_sp = 0;
}

void shader_uniforms(mat4* proj, mat4* view, mat4* model)
{
    unsigned int loc = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)proj);
    loc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)view);
    loc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)model);
    loc = glGetUniformLocation(instance_sp, "proj");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)proj);
    loc = glGetUniformLocation(instance_sp, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)view);
    loc = glGetUniformLocation(instance_sp, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)model);
}

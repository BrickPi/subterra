#include "subterra/shader.h"

unsigned int shaderProgram;

const char* vertexShaderSource = "#version 330 core\nlayout (location=0) in vec3 aPos;layout (location=1) in vec2 aTexCoord;out vec2 TexCoord;void main(){gl_Position=vec4(aPos,1.0);TexCoord=vec2(aTexCoord.x,aTexCoord.y);}\0";
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
    if(!success) {
        FILE* logfile = fopen("log.txt", "a+");
        fprintf(logfile, "ERROR COMPILING VERTEX SHADER!\n");
        fclose(logfile);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        FILE* logfile = fopen("log.txt", "a+");
        fprintf(logfile, "ERROR COMPILING FRAGMENT SHADER\n");
        fclose(logfile);
    }
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        FILE* logfile = fopen("log.txt", "a+");
        fprintf(logfile, "ERROR LINKING SHADER PROGRAM!\n");
        fclose(logfile);
    }
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
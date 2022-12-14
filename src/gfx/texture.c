#include "subterra/gfx/texture.h"

int numtextures;
int* textures[];

void create_texture(char* filename, char* texname, char cube)
{
    unsigned int texture;
    int width, height, channels;
    glGenTextures(1, &texture);
    stbi_set_flip_vertically_on_load(1);
    if (cube)
    {
        logger_log("ERROR: CUBEMAP NOT IMPLEMENTED!");
    } else {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
        if(!data)
        {
            char* s;
            sprintf(s, "ERROR LOADING TEXTURE %s\n", texname);
            logger_log(s);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        numtextures++;
        
    }
}

unsigned int get_texture(char* texname)
{

}

void delete_texture(char* texname)
{

}

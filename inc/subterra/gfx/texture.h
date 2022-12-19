#ifndef _H_SUBTERRA_TEXTURE
#define _H_SUBTERRA_TEXTURE
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "stdlib.h"
#include "subterra/util/logger.h"
unsigned int create_texture(char* filename, char flip);
void delete_texture(unsigned int texture);
#endif
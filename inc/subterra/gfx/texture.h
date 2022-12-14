#ifndef _H_SUBTERRA_TEXTURE
#define _H_SUBTERRA_TEXTURE
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "stdlib.h"
#include "subterra/util/logger.h"
void create_texture(char* filename, char* texname, char cube);
unsigned int get_texture(char* texname);
void delete_texture(char* texname);
#endif
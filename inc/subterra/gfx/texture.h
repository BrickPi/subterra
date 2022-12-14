#ifndef _H_SUBTERRA_TEXTURE
#define _H_SUBTERRA_TEXTURE
void create_texture(char* filename, char* texname, char skybox);
unsigned int get_texture(char* texname);
void delete_texture(char* texname);
#endif
#ifndef _H_SUBTERRA_SHADER
#define _H_SUBTERRA_SHADER
#include <stdio.h>
#include "glad/glad.h"
#include "cglm/cglm.h"
void shader_load();
void shader_use();
void shader_clean();
void shader_uniforms(mat4* proj, mat4* view, mat4* model);
#endif
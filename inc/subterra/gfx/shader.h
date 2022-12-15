#ifndef _H_SUBTERRA_SHADER
#define _H_SUBTERRA_SHADER
#include <stdio.h>
#include "glad/glad.h"
#include "cglm/cglm.h"
#include "subterra/util/logger.h"
void shader_load();
void shader_use_flat();
void shader_use_instanced();
void shader_clean();
void shader_uniforms(mat4* proj, mat4* view, mat4* model);
void set_instance_uniform(int count, vec3* arr);
#endif

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "Utils/Utils.h"

typedef struct {
	char *vertexShaderPath, *fragmentShaderPath;
	unsigned int programID;
} Shader;

extern void Rendering_Shader_Initialize(Shader *shader, const char *vertexShaderPath, const char *fragmentShaderPath);
extern void Rendering_Shader_FreeContent(Shader *shader);

#define MAX_SHAPES 10
#define VERTEX_COUNT_TRIANGLE 3
#define VERTEX_COUNT_RECT 6
#define VERTEX_FLOAT_COUNT 5

typedef enum {
    RECT,
    TRIANGLE,
    COUNT
} SHAPE;

typedef struct Rendering_State {
    unsigned int *vaos, *vbos;
    float **verticies;

    SHAPE *shape_type;
    size_t length;

    void (*edit_shape[COUNT])(struct Rendering_State *rendering_state, float x1, float y1, float x2, float y2, float r, float g, float b);
    void (*add_shape[COUNT])(struct Rendering_State *rendering_state, float x1, float y1, float x2, float y2, float r, float g, float b);
} Rendering_State;

extern void Rendering_State_Initialize(Rendering_State *state);
extern void Rendering_State_Free(Rendering_State *state);

/**
 * x1 - top x coordinate
 * y1 - top y coordinate
 *
 * x2 - bottom right x coordinate
 * y2 - bottom right y coordinate
 */
extern void Rendering_State_Edit_Triangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b);
extern void Rendering_State_Add_Triangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b) ;

extern void Rendering_State_Edit_Rectangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b);
extern void Rendering_State_Add_Rectangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b);

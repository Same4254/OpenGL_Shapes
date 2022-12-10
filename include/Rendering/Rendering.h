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
    SHAPE_TYPE_COUNT
} SHAPE_TYPE;

/**
 * Represent a vertex as the coordinate and its color
 */
typedef union {
    float data[VERTEX_FLOAT_COUNT];
    struct {
        float x;
        float y;
        float r;
        float g;
        float b;
    };
} Vertex_2D;

typedef struct {
    SHAPE_TYPE type;
    unsigned int vao, vbo;

    size_t vertex_count;
    Vertex_2D *verticies;
} Shape;

/**
 * Provide this struct to construct a shape. 
 * Two points and the color is everything needed to define a shape
 * Used to track history on the stack
 */
typedef struct {
    float x1;
    float y1;

    float x2;
    float y2;

    float r;
    float g;
    float b;
} Shape_Definition;

typedef struct RenderingState {
    Shape shapes[MAX_SHAPES];
    size_t length;

    void (*edit_shape[SHAPE_TYPE_COUNT])(struct RenderingState *rendering_state, Shape_Definition *shape_definition, const size_t index);
    bool (*add_shape[SHAPE_TYPE_COUNT])(struct RenderingState *rendering_state, Shape_Definition *shape_definition, const size_t index);
} RenderingState;

extern void RenderingState_Initialize(RenderingState *state);
extern void RenderingState_Free(RenderingState *state);

extern void RenderingState_RemoveShape(RenderingState *state, Shape_Definition *shape_definition, const size_t index);

// **** Shape Functions **** //

extern void RenderingState_Edit_Triangle(RenderingState *state, Shape_Definition *shape_definition, const size_t index);
extern bool RenderingState_Add_Triangle(RenderingState *state, Shape_Definition *shape_definition, const size_t index);

extern void RenderingState_Edit_Rectangle(RenderingState *state, Shape_Definition *shape_definition, const size_t index);
extern bool RenderingState_Add_Rectangle(RenderingState *state, Shape_Definition *shape_definition, const size_t index);

#include "Rendering/Rendering.h"

void Rendering_State_Initialize(Rendering_State *state) {
    state->vaos = malloc(sizeof(*state->vaos) * MAX_SHAPES);
    state->vbos = malloc(sizeof(*state->vbos) * MAX_SHAPES);

    glGenBuffers(MAX_SHAPES, state->vbos);
    glGenVertexArrays(MAX_SHAPES, state->vaos);

    state->verticies = malloc(sizeof(*state->verticies) * MAX_SHAPES);
    state->shape_type = malloc(sizeof(*state->shape_type) * MAX_SHAPES);

    state->edit_shape[TRIANGLE] = Rendering_State_Edit_Triangle;
    state->add_shape[TRIANGLE] = Rendering_State_Add_Triangle;

    state->edit_shape[RECT] = Rendering_State_Edit_Rectangle;
    state->add_shape[RECT] = Rendering_State_Add_Rectangle;

    state->length = 0;
}

void Rendering_State_Free(Rendering_State *state) {
    free(state->vaos);
    free(state->vbos);
    free(state->shape_type);

    for (size_t i = 0; i < state->length; i++)
        free(state->verticies[i]);
    free(state->verticies);
}

void Rendering_State_Edit_Triangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b) {
    size_t index = state->length - 1;
    float *vertices = state->verticies[index];

    //printf("%f, %f, %f, %f\n", x1, y1, x2, y2);

    vertices[0] = x1;
    vertices[1] = y1;
    vertices[2] = r;
    vertices[3] = g;
    vertices[4] = b;

    vertices[5] = x2;
    vertices[6] = y2;
    vertices[7] = r;
    vertices[8] = g;
    vertices[9] = b;

    vertices[10] = x1 - (x2 - x1);
    vertices[11] = y2;
    vertices[12] = r;
    vertices[13] = g;
    vertices[14] = b;

    state->shape_type[index] = TRIANGLE;

    glBindVertexArray(state->vaos[index]);
    glBindBuffer(GL_ARRAY_BUFFER, state->vbos[index]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTEX_FLOAT_COUNT * VERTEX_COUNT_TRIANGLE, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Rendering_State_Add_Triangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b) {
    printf("Add_Trinagle\n");

    state->verticies[state->length] = malloc(sizeof(float) * VERTEX_FLOAT_COUNT * VERTEX_COUNT_TRIANGLE);
    state->shape_type[state->length] = TRIANGLE;
    state->length++;
    Rendering_State_Edit_Triangle(state, x1, y1, x2, y2, r, g, b);
}

void Rendering_State_Edit_Rectangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b) {
    size_t index = state->length - 1;
    float *vertices = state->verticies[index];

    // triangle 1
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[2] = r;
    vertices[3] = g;
    vertices[4] = b;

    vertices[5] = x2;
    vertices[6] = y1;
    vertices[7] = r;
    vertices[8] = g;
    vertices[9] = b;

    vertices[10] = x1;
    vertices[11] = y2;
    vertices[12] = r;
    vertices[13] = g;
    vertices[14] = b;

    // triangle 2
    vertices[15] = x2;
    vertices[16] = y1;
    vertices[17] = r;
    vertices[18] = g;
    vertices[19] = b;

    vertices[20] = x2;
    vertices[21] = y2;
    vertices[22] = r;
    vertices[23] = g;
    vertices[24] = b;

    vertices[25] = x1;
    vertices[26] = y2;
    vertices[27] = r;
    vertices[28] = g;
    vertices[29] = b;

    state->shape_type[index] = RECT;

    glBindVertexArray(state->vaos[index]);
    glBindBuffer(GL_ARRAY_BUFFER, state->vbos[index]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTEX_FLOAT_COUNT * VERTEX_COUNT_RECT, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Rendering_State_Add_Rectangle(Rendering_State *state, float x1, float y1, float x2, float y2, float r, float g, float b) {
    printf("Add_Rect\n");

    state->verticies[state->length] = malloc(sizeof(float) * VERTEX_FLOAT_COUNT * VERTEX_COUNT_RECT);
    state->shape_type[state->length] = RECT;
    state->length++;
    Rendering_State_Edit_Rectangle(state, x1, y1, x2, y2, r, g, b);
}

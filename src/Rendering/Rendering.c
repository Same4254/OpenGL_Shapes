#include "Rendering/Rendering.h"

void RenderingState_Initialize(RenderingState *state) {
    // bind a vbo and vao for each shape we expect to use
    for (size_t i = 0; i < MAX_SHAPES; i++) {
        glGenBuffers(1, &state->shapes[i].vbo);
        glGenVertexArrays(1, &state->shapes[i].vao);
        state->shapes[i].verticies = NULL;
    }

    state->edit_shape[TRIANGLE] = RenderingState_Edit_Triangle;
    state->add_shape[TRIANGLE] = RenderingState_Add_Triangle;

    state->edit_shape[RECT] = RenderingState_Edit_Rectangle;
    state->add_shape[RECT] = RenderingState_Add_Rectangle;

    state->length = 0;
}

void RenderingState_Free(RenderingState *state) {
    for (size_t i = 0; i < state->length; i++)
        free(state->shapes[i].verticies);
}

void RenderingState_RemoveShape(RenderingState *state, const ShapeDefinition *shape_definition, const size_t index) {
	if (state->length == 0)
        return;

    for (size_t i = index; i < state->length - 1; i++) {
        Shape *shape = &state->shapes[i];

        // Preserve the vertex buffer that this shape is attached to
        unsigned int vbo = shape->vbo;
        unsigned int vao = shape->vao;

        *shape = state->shapes[i + 1];
        shape->vbo = vbo;
        shape->vao = vao;

		glBindVertexArray(shape->vao);
        glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_2D) * shape->vertex_count, shape->verticies, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    state->length--;
}

void RenderingState_TranslateShape(RenderingState *state, const ShapeDefinition *shape_definition, const size_t index) {
	Shape *shape = &state->shapes[index];
    shape->shape_definition.x1 += shape_definition->x1;
    shape->shape_definition.y1 += shape_definition->y1;
    shape->shape_definition.x2 += shape_definition->x2;
    shape->shape_definition.y2 += shape_definition->y2;

    if (shape_definition->r != -1.0f) {
        shape->shape_definition.r = shape_definition->r;
        shape->shape_definition.g = shape_definition->g;
        shape->shape_definition.b = shape_definition->b;
    }

    state->edit_shape[shape->type](state, &shape->shape_definition, index);
}

bool Rendering_ShapeContainingPoint(RenderingState *state, const float x_pos, const float y_pos) {

}

void RenderingState_Edit_Triangle(RenderingState *state, const ShapeDefinition *shape_definition, const size_t index) {
    Shape *shape = &state->shapes[index];

    shape->shape_definition.x1 = shape_definition->x1;
    shape->shape_definition.y1 = shape_definition->y1;
    shape->shape_definition.x2 = shape_definition->x2;
    shape->shape_definition.y2 = shape_definition->y2;

    //printf("%f, %f, %f, %f\n", x1, y1, x2, y2);
    Vertex_2D *vertex1 = &shape->verticies[0];
    Vertex_2D *vertex2 = &shape->verticies[1];
    Vertex_2D *vertex3 = &shape->verticies[2];

    vertex1->x = shape_definition->x1;
    vertex1->y = shape_definition->y1;

    vertex2->x = shape_definition->x2;
    vertex2->y = shape_definition->y2;

    vertex3->x = shape_definition->x1 - (shape_definition->x2 - shape_definition->x1);
    vertex3->y = shape_definition->y2;

    if (shape_definition->r != -1.0f && shape_definition->g != -1.0f && shape_definition->b != -1.0f) {
        shape->shape_definition.r = shape_definition->r;
        shape->shape_definition.g = shape_definition->g;
        shape->shape_definition.b = shape_definition->b;

        for (size_t i = 0; i < VERTEX_COUNT_TRIANGLE; i++) {
            shape->verticies[i].r = shape_definition->r;
            shape->verticies[i].g = shape_definition->g;
            shape->verticies[i].b = shape_definition->b;
        }
    }

    glBindVertexArray(shape->vao);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_2D) * shape->vertex_count, shape->verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void RenderingState_Add_Triangle(RenderingState *state, const ShapeDefinition *shape_definition, const size_t index) {
    printf("Add_Trinagle {%f, %f, %f, %f, %f, %f, %f}\n", shape_definition->x1, shape_definition->y1, shape_definition->x2, shape_definition->y2, shape_definition->r, shape_definition->g, shape_definition->b);

    if (index >= MAX_SHAPES)
        return;

    Shape *shape = &state->shapes[index];
    shape->vertex_count = VERTEX_COUNT_TRIANGLE;
    shape->verticies = malloc(sizeof(Vertex_2D) * VERTEX_COUNT_TRIANGLE);
    shape->type = TRIANGLE;
    state->length++;
    RenderingState_Edit_Triangle(state, shape_definition, index);
}

void RenderingState_Edit_Rectangle(RenderingState *state, const ShapeDefinition *shape_definition, const size_t index) {
    Shape *shape = &state->shapes[index];
    shape->shape_definition.x1 = shape_definition->x1;
    shape->shape_definition.y1 = shape_definition->y1;
    shape->shape_definition.x2 = shape_definition->x2;
    shape->shape_definition.y2 = shape_definition->y2;

    // triangle 1
    shape->verticies[0].x = shape_definition->x1;
    shape->verticies[0].y = shape_definition->y1;

    shape->verticies[1].x = shape_definition->x2;
    shape->verticies[1].y = shape_definition->y1;

    shape->verticies[2].x = shape_definition->x1;
    shape->verticies[2].y = shape_definition->y2;

    // triangle 2
    shape->verticies[3].x = shape_definition->x2;
    shape->verticies[3].y = shape_definition->y1;

    shape->verticies[4].x = shape_definition->x2;
    shape->verticies[4].y = shape_definition->y2;

    shape->verticies[5].x = shape_definition->x1;
    shape->verticies[5].y = shape_definition->y2;

    if (shape_definition->r != -1.0f && shape_definition->g != -1.0f && shape_definition->b != -1.0f) {
		shape->shape_definition.r = shape_definition->r;
		shape->shape_definition.g = shape_definition->g;
		shape->shape_definition.b = shape_definition->b;

        for (size_t i = 0; i < VERTEX_COUNT_RECT; i++) {
            shape->verticies[i].r = shape_definition->r;
            shape->verticies[i].g = shape_definition->g;
            shape->verticies[i].b = shape_definition->b;
        }
    }

    glBindVertexArray(shape->vao);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_2D) * VERTEX_COUNT_RECT, shape->verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void RenderingState_Add_Rectangle(RenderingState *state, const ShapeDefinition *shape_definition, const size_t index) {
    printf("Add_Rect\n");

    if (index >= MAX_SHAPES)
        return;
    
    Shape *shape = &state->shapes[index];

    shape->verticies = malloc(sizeof(Vertex_2D) * VERTEX_COUNT_RECT);
    shape->vertex_count = VERTEX_COUNT_RECT;
    shape->type = RECT;
    state->length++;
    RenderingState_Edit_Rectangle(state, shape_definition, index);
}

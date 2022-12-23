#include "Rendering/Rendering.h"
#include "State/State.h"

void InsertMode_Update(ControlState *control_state, RenderingState *rendering_state, GLFWwindow *window) {
	InsertModeState *insert_state = &control_state->insert_mode;
    MouseState *current_mouse = &control_state->current_mouse;
    MouseState *last_mouse = &control_state->last_mouse;

    // follow the mouse if we are creating a shape
    if (insert_state->creating_shape) {
        // preserve previous color
        ShapeDefinition definition;
        definition.x1 = insert_state->startX;
        definition.y1 = insert_state->startY;
        definition.x2 = current_mouse->x_pos;
        definition.y2 = current_mouse->y_pos;

        // no change to the color
        definition.r = -1.0f;
        definition.g = -1.0f;
        definition.b = -1.0f;

        rendering_state->edit_shape[insert_state->creating_shape_type](rendering_state, &definition, rendering_state->length - 1);
    } else {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            insert_state->creating_shape_type = RECT;
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            insert_state->creating_shape_type = TRIANGLE;
    }

    bool clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    if (clicked) {
        if (insert_state->creating_shape) {
            insert_state->creating_shape = false;

            Action *action = &control_state->history[control_state->history_current_pos];
            action->do_definition = rendering_state->shapes[rendering_state->length - 1].shape_definition;
            action->do_func = rendering_state->add_shape[insert_state->creating_shape_type];
            action->undo_func = RenderingState_RemoveShape;
            action->index = rendering_state->length - 1;

            control_state->history_current_pos++;
            control_state->history_length = control_state->history_current_pos;
        } else if (rendering_state->length < MAX_SHAPES) {
            insert_state->startX = current_mouse->x_pos;
            insert_state->startY = current_mouse->y_pos;
            insert_state->creating_shape = true;

            ShapeDefinition definition;
            definition.x1 = current_mouse->x_pos;
            definition.y1 = current_mouse->y_pos;
            definition.x2 = current_mouse->x_pos;
            definition.y2 = current_mouse->y_pos;

            definition.r = (float) rand() / (float) RAND_MAX;
            definition.g = (float) rand() / (float) RAND_MAX;
            definition.b = (float) rand() / (float) RAND_MAX;

            rendering_state->add_shape[insert_state->creating_shape_type](rendering_state, &definition, rendering_state->length);
        }
    }
}

#include "Rendering/Rendering.h"
#include "State/State.h"

void InsertMode_Update_FinalState(ControlState *control_state, RenderingState *rendering_state) {
    InsertModeState *insert_state = &control_state->insert_mode;

    const MouseState *current_mouse = &control_state->current_mouse;
    const MouseState *last_mouse = &control_state->last_mouse;

    const KeyboardState *current_keyboard = &control_state->current_keyboard;
    const KeyboardState *last_keyboard = &control_state->last_keyboard;

    if (current_keyboard->r_button_state == GLFW_PRESS)
        insert_state->creating_shape_type = RECT;
    if (current_keyboard->t_button_state == GLFW_PRESS)
        insert_state->creating_shape_type = TRIANGLE;

    const bool mouse_clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    const bool space_released = (last_keyboard->space_button_state == GLFW_PRESS) && (current_keyboard->space_button_state == GLFW_RELEASE);
    if (mouse_clicked && rendering_state->length < MAX_SHAPES) {
        insert_state->startX = current_mouse->x_pos;
        insert_state->startY = current_mouse->y_pos;

        ShapeDefinition definition;
        definition.x1 = current_mouse->x_pos;
        definition.y1 = current_mouse->y_pos;
        definition.x2 = current_mouse->x_pos;
        definition.y2 = current_mouse->y_pos;

        definition.r = (float) rand() / (float) RAND_MAX;
        definition.g = (float) rand() / (float) RAND_MAX;
        definition.b = (float) rand() / (float) RAND_MAX;

        rendering_state->add_shape[insert_state->creating_shape_type](rendering_state, &definition, rendering_state->length);
        control_state->current_mode_update_fptr = InsertMode_Update_ClickedState;
    } else if (space_released) {
        control_state->current_mode_update_fptr = EditMode_Update_FinalState;
    } else {
        ControlState_CheckUndoRedo(control_state, rendering_state);
    }
}

void InsertMode_Update_ClickedState(ControlState *control_state, RenderingState *rendering_state) {
    InsertModeState *insert_state = &control_state->insert_mode;
    const MouseState *current_mouse = &control_state->current_mouse;
    const MouseState *last_mouse = &control_state->last_mouse;
    const KeyboardState *current_keyboard = &control_state->current_keyboard;

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

    const bool clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    if (clicked) {
        Action *action = &control_state->history[control_state->history_current_pos];
        action->do_definition = rendering_state->shapes[rendering_state->length - 1].shape_definition;
        action->do_func = rendering_state->add_shape[insert_state->creating_shape_type];
        action->undo_func = RenderingState_RemoveShape;
        action->index = rendering_state->length - 1;

        control_state->history_current_pos++;
        control_state->history_length = control_state->history_current_pos;

        // switch the current mode
        control_state->current_mode_update_fptr = InsertMode_Update_FinalState;
    }
}

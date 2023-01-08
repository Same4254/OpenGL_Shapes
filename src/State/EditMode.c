#include "Rendering/Rendering.h"
#include "State/State.h"

void EditMode_Update_FinalState(ControlState *control_state, RenderingState *rendering_state) {
    EditModeState *edit_state = &control_state->edit_mode;

    MouseState *current_mouse = &control_state->current_mouse;
    MouseState *last_mouse = &control_state->last_mouse;

    KeyboardState *current_keyboard = &control_state->current_keyboard;
    KeyboardState *last_keyboard = &control_state->last_keyboard;

    bool mouse_clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    const bool space_released = (last_keyboard->space_button_state == GLFW_PRESS) && (current_keyboard->space_button_state == GLFW_RELEASE);
    if (mouse_clicked) {
        int shape_clicked_index = Rendering_ShapeContainingPoint(rendering_state, current_mouse->x_pos, current_mouse->y_pos);
        if (shape_clicked_index != -1) {
            edit_state->startX = current_mouse->x_pos;
            edit_state->startY = current_mouse->y_pos;

            edit_state->moving_shape_index = shape_clicked_index;
            edit_state->moving_shape_original_defintion = rendering_state->shapes[shape_clicked_index].shape_definition;

            control_state->current_mode_update_fptr = EditMode_Update_ClickedState;
        }
    } else if (space_released) {
        control_state->current_mode_update_fptr = InsertMode_Update_FinalState;
    } else {
		ControlState_CheckUndoRedo(control_state, rendering_state);
    }
}

void EditMode_Update_ClickedState(ControlState *control_state, RenderingState *rendering_state) {
	EditModeState *edit_state = &control_state->edit_mode;

    MouseState *current_mouse = &control_state->current_mouse;
    MouseState *last_mouse = &control_state->last_mouse;

    KeyboardState *current_keyboard = &control_state->current_keyboard;
    KeyboardState *last_keyboard = &control_state->last_keyboard;

    // move the shape to the mouse positiom
    ShapeDefinition new_definition;
    const ShapeDefinition *original_definition = &edit_state->moving_shape_original_defintion;

    new_definition.x1 = original_definition->x1 - (edit_state->startX - current_mouse->x_pos);
    new_definition.y1 = original_definition->y1 - (edit_state->startY - current_mouse->y_pos);
    new_definition.x2 = original_definition->x2 - (edit_state->startX - current_mouse->x_pos);
    new_definition.y2 = original_definition->y2 - (edit_state->startY - current_mouse->y_pos);

    new_definition.r = -1.0f;
    new_definition.g = -1.0f;
    new_definition.b = -1.0f;

    rendering_state->edit_shape[rendering_state->shapes[edit_state->moving_shape_index].type](rendering_state, &new_definition, edit_state->moving_shape_index);

    bool mouse_clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    bool delete_clicked = (last_keyboard->delete_button_state == GLFW_PRESS) && (current_keyboard->delete_button_state == GLFW_RELEASE);
    if (mouse_clicked) {
        // place the shape
        ShapeDefinition *moved_to = &rendering_state->shapes[edit_state->moving_shape_index].shape_definition;
        printf("End Moving. Placed at: %f %f %f %f\n", moved_to->x1, moved_to->y1, moved_to->x2, moved_to->y2);

        Action *action = &control_state->history[control_state->history_current_pos];
        action->do_definition = rendering_state->shapes[edit_state->moving_shape_index].shape_definition;
        action->do_definition.x1 = current_mouse->x_pos - edit_state->startX;
        action->do_definition.y1 = current_mouse->y_pos - edit_state->startY;

        action->do_definition.x2 = action->do_definition.x1;
        action->do_definition.y2 = action->do_definition.y1;

        action->undo_definition.x1 = -action->do_definition.x1;
        action->undo_definition.y1 = -action->do_definition.y1;
        action->undo_definition.x2 = -action->do_definition.x2;
        action->undo_definition.y2 = -action->do_definition.y2;

        action->do_definition.r = -1.0f;
        action->do_definition.g = -1.0f;
        action->do_definition.b = -1.0f;

        action->undo_definition.r = -1.0f;
        action->undo_definition.g = -1.0f;
        action->undo_definition.b = -1.0f;

        action->do_func = RenderingState_TranslateShape;
        action->undo_func = RenderingState_TranslateShape;
        action->index = edit_state->moving_shape_index;

        control_state->history_current_pos++;
        control_state->history_length = control_state->history_current_pos;

        control_state->current_mode_update_fptr = EditMode_Update_FinalState;
    } else if (delete_clicked) {
		Action *action = &control_state->history[control_state->history_current_pos];
        action->undo_definition = edit_state->moving_shape_original_defintion;
        action->undo_func = rendering_state->add_shape[rendering_state->shapes[edit_state->moving_shape_index].type];
        action->do_func = RenderingState_RemoveShape;
        action->index = edit_state->moving_shape_index;

        control_state->history_current_pos++;
        control_state->history_length = control_state->history_current_pos;

        RenderingState_RemoveShape(rendering_state, NULL, edit_state->moving_shape_index);
        control_state->current_mode_update_fptr = EditMode_Update_FinalState;
    }
}

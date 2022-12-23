#include "Rendering/Rendering.h"
#include "State/State.h"

void EditMode_Update(ControlState *control_state, RenderingState *rendering_state, GLFWwindow *window) {
    EditModeState *edit_state = &control_state->edit_mode;
    MouseState *current_mouse = &control_state->current_mouse;
    MouseState *last_mouse = &control_state->last_mouse;

	if (edit_state->moving_shape) {
		ShapeDefinition shape_definition;

        shape_definition.x1 = current_mouse->x_pos - last_mouse->x_pos;
        shape_definition.y1 = current_mouse->y_pos - last_mouse->y_pos;

        shape_definition.x2 = current_mouse->x_pos - last_mouse->x_pos;
        shape_definition.y2 = current_mouse->y_pos - last_mouse->y_pos;

        shape_definition.r = -1.0f;
        shape_definition.g = -1.0f;
        shape_definition.b = -1.0f;

		RenderingState_TranslateShape(rendering_state, &shape_definition, edit_state->moving_shape_index);
    }

    bool clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    if (clicked) {
		if (edit_state->moving_shape) {
			edit_state->moving_shape = false;

            Action *action = &control_state->history[control_state->history_current_pos];
            action->do_definition = rendering_state->shapes[edit_state->moving_shape_index].shape_definition;
			action->do_definition.x1 = current_mouse->x_pos - edit_state->startX;
			action->do_definition.y1 = current_mouse->y_pos - edit_state->startY;

			action->do_definition.x2 = action->do_definition.x1;
			action->do_definition.y2 = action->do_definition.y1;
            
            action->undo_definition.x1 = - action->do_definition.x1;
            action->undo_definition.y1 = - action->do_definition.y1;
            action->undo_definition.x2 = - action->do_definition.x2;
            action->undo_definition.y2 = - action->do_definition.y2;
            
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

        } else {
            edit_state->startX = current_mouse->x_pos;
            edit_state->startY = current_mouse->y_pos;
            edit_state->moving_shape = true;
            edit_state->moving_shape_index = 0;
        }
    }
}

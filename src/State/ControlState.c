#include "State/State.h"

void ControlState_Initialize(ControlState *control_state) {
    control_state->current_mode_update_fptr = InsertMode_Update_FinalState;
    control_state->history_length = 0;
    control_state->history_current_pos = 0;

    EditModeState *edit_mode = &control_state->edit_mode;
    InsertModeState *insert_mode = &control_state->insert_mode;

    insert_mode->creating_shape_type = TRIANGLE;

    control_state->current_mouse.x_pos = 0;
    control_state->current_mouse.y_pos = 0;

    control_state->last_mouse.x_pos = 0;
    control_state->last_mouse.y_pos = 0;

    control_state->current_mouse.left_button_state = GLFW_RELEASE;
    control_state->last_mouse.left_button_state = GLFW_RELEASE;

    control_state->last_keyboard.delete_button_state = GLFW_RELEASE;
    control_state->last_keyboard.space_button_state = GLFW_RELEASE;
    control_state->last_keyboard.y_button_state = GLFW_RELEASE;
    control_state->last_keyboard.z_button_state = GLFW_RELEASE;

    control_state->current_keyboard.delete_button_state = GLFW_RELEASE;
    control_state->current_keyboard.space_button_state = GLFW_RELEASE;
    control_state->current_keyboard.y_button_state = GLFW_RELEASE;
    control_state->current_keyboard.z_button_state = GLFW_RELEASE;
}

void ControlState_Update_Mode(ControlState *control_state, RenderingState *rendering_state) {
    control_state->current_mode_update_fptr(control_state, rendering_state);
}

void ControlState_CheckUndoRedo(ControlState *control_state, RenderingState *rendering_state) {
    if (control_state->last_keyboard.z_button_state == GLFW_PRESS && control_state->current_keyboard.z_button_state == GLFW_RELEASE && control_state->history_current_pos != 0) {
        Action *action = &control_state->history[control_state->history_current_pos - 1];
        action->undo_func(rendering_state, &action->undo_definition, action->index);
        control_state->history_current_pos--;
    }

    if (control_state->last_keyboard.y_button_state == GLFW_PRESS && control_state->current_keyboard.y_button_state == GLFW_RELEASE && control_state->history_current_pos != control_state->history_length) {
        Action *action = &control_state->history[control_state->history_current_pos];
        action->do_func(rendering_state, &action->do_definition, action->index);
        control_state->history_current_pos++;
    }
}

void ControlState_Update_Mouse(ControlState *control_state, GLFWwindow *window) {
    control_state->last_mouse = control_state->current_mouse;

    control_state->current_mouse.left_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    double x_pos_pixel, y_pos_pixel;
    glfwGetCursorPos(window, &x_pos_pixel, &y_pos_pixel);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    control_state->current_mouse.x_pos = (x_pos_pixel - (width / 2.0f)) / ((float) width / 2.0f);
    control_state->current_mouse.y_pos = -(y_pos_pixel - (height / 2.0f)) / ((float) height / 2.0f);
}

void ControlState_Update_Keyboard(ControlState *control_state, GLFWwindow *window) {
    control_state->last_keyboard = control_state->current_keyboard;

    KeyboardState *keyboard = &control_state->current_keyboard;
    keyboard->space_button_state = glfwGetKey(window, GLFW_KEY_SPACE);
    keyboard->z_button_state = glfwGetKey(window, GLFW_KEY_Z);
    keyboard->t_button_state = glfwGetKey(window, GLFW_KEY_T);
    keyboard->r_button_state = glfwGetKey(window, GLFW_KEY_R);
    keyboard->y_button_state = glfwGetKey(window, GLFW_KEY_Y);
    keyboard->delete_button_state = glfwGetKey(window, GLFW_KEY_DELETE);
}

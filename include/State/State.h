#pragma once

#include "Rendering/Rendering.h"

typedef struct {
	float x_pos;
    float y_pos;

    int left_button_state;
} MouseState;

typedef struct {
	int space_button_state;
    int delete_button_state;

    int z_button_state;
    int y_button_state;
} KeyboardState;

typedef enum {
	EDIT,
    INSERT
} CONTROL_MODE;

typedef struct {
	bool moving_shape;
    float startX;
    float startY;

    ShapeDefinition moving_shape_original_defintion;
    size_t moving_shape_index;
} EditModeState;

typedef struct {
	bool creating_shape;
    float startX;
    float startY;
    SHAPE_TYPE creating_shape_type;
} InsertModeState;

#define HISTORY_STACK_CAPACITY 1000

typedef struct {
	ShapeDefinition do_definition;
	void (*do_func)(RenderingState *rendering_state, const ShapeDefinition *shape_definition, const size_t index);

	ShapeDefinition undo_definition;
	void (*undo_func)(RenderingState *rendering_state, const ShapeDefinition *shape_definition, const size_t index);

    size_t index;
} Action;

typedef struct {
	CONTROL_MODE current_mode;

    EditModeState edit_mode;
    InsertModeState insert_mode;

    MouseState current_mouse;
    MouseState last_mouse;

    KeyboardState current_keyboard;
    KeyboardState last_keyboard;

    size_t history_length;
    size_t history_current_pos;
    Action history[HISTORY_STACK_CAPACITY];
} ControlState;

void ControlState_Initialize(ControlState *control_state);

/**
* @brief Updates the mouse information in the control state. Call this exactly once per frame.
* Checks the mouse location and buttons
*
* @param control_state
* @param window
*/
void ControlState_Update_Mouse(ControlState *control_state, GLFWwindow *window);

void ControlState_Update_Keyboard(ControlState *control_state, GLFWwindow *window);

/**
* @brief Toggles the current state from one state to the other, *safely*.
* In the effort of safety, it may not be a possible time to exit the current state.
* Thus, this function will truly only change the state if it feels like it.
*
* Example: The user is in insert mode and is creating a shape. They have not yet clicked down to place the shape.
* Then the user attempts to switch to edit mode. Doing so would leave insert mode in an unfinished state.
* Therefore, the state will only transition if the current mode is able to do so.
*
* @param control_state
* @param redering_state
*
* @return Whether or not the mode was switched
*/
bool ControlState_ToggleMode(ControlState *control_state, RenderingState *rendering_state);

/**
* @brief Updates the state of the game in whichever state it is currently in. Uses the control state to determine what logic to use. Uses the rendering state to draw
*
* @param control_state
* @param redering_state
*/
void ControlState_Update_Mode(ControlState *control_state, RenderingState *rendering_state, GLFWwindow *window);

void InsertMode_Update(ControlState *control_state, RenderingState *rendering_state, GLFWwindow *window);
void EditMode_Update(ControlState *control_state, RenderingState *rendering_state, GLFWwindow *window);

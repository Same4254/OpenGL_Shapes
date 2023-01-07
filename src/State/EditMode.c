#include "Rendering/Rendering.h"
#include "State/State.h"
#include <stdio.h>

float sign(const Vertex_2D *p1, const Vertex_2D *p2, const Vertex_2D *p3) {
	return (p1->x - p3->x) * (p2->y - p3->y) - (p2->x - p3->x) * (p1->y - p3->y);
}

int FindShape(const RenderingState *rendering_state, float mouse_x, float mouse_y) {
    // find what shape is becing clicked. Go in reverse order to match the visual z-indexing
    // https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle

    printf("Finding Shape at mouse coordinates: %f %f\n", mouse_x, mouse_y);

    for (int i = rendering_state->length - 1; i >= 0; i--) {
        const Shape *shape = &rendering_state->shapes[i];

        for (size_t triangle_index = 0; triangle_index < shape->vertex_count; triangle_index += 3) {
            // check if the point is inside the triangle
            const Vertex_2D *vertex1 = &shape->verticies[triangle_index + 0];
            const Vertex_2D *vertex2 = &shape->verticies[triangle_index + 1];
            const Vertex_2D *vertex3 = &shape->verticies[triangle_index + 2];

            Vertex_2D mouse;
            mouse.x = mouse_x;
            mouse.y = mouse_y;

            float d1, d2, d3;
            bool has_neg, has_pos;

            d1 = sign(&mouse, vertex1, vertex2);
            d2 = sign(&mouse, vertex2, vertex3);
            d3 = sign(&mouse, vertex3, vertex1);

            has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

            printf("%f %f %f, Verticies: { %f, %f }, { %f, %f }, { %f, %f }\n", d1, d2, d3, vertex1->x, vertex1->y, vertex2->x, vertex2->y, vertex3->x, vertex3->y);

            if (!(has_neg && has_pos))
                return i;
        }
    }

    return -1;
}

void EditMode_Update(ControlState *control_state, RenderingState *rendering_state, GLFWwindow *window) {
    EditModeState *edit_state = &control_state->edit_mode;
    MouseState *current_mouse = &control_state->current_mouse;
    MouseState *last_mouse = &control_state->last_mouse;

	if (edit_state->moving_shape) {
		ShapeDefinition new_definition;
        const ShapeDefinition *original_definition = &edit_state->moving_shape_original_defintion;

        //shape_definition.x1 = current_mouse->x_pos - last_mouse->x_pos;
        //shape_definition.y1 = current_mouse->y_pos - last_mouse->y_pos;

        //shape_definition.x2 = current_mouse->x_pos - last_mouse->x_pos;
        //shape_definition.y2 = current_mouse->y_pos - last_mouse->y_pos;

		new_definition.x1 = original_definition->x1 - (edit_state->startX - current_mouse->x_pos);
		new_definition.y1 = original_definition->y1 - (edit_state->startY - current_mouse->y_pos);
		new_definition.x2 = original_definition->x2 - (edit_state->startX - current_mouse->x_pos);
		new_definition.y2 = original_definition->y2 - (edit_state->startY - current_mouse->y_pos);

        new_definition.r = -1.0f;
        new_definition.g = -1.0f;
        new_definition.b = -1.0f;

        rendering_state->edit_shape[rendering_state->shapes[edit_state->moving_shape_index].type](rendering_state, &new_definition, edit_state->moving_shape_index);
    }

    bool clicked = (last_mouse->left_button_state == GLFW_PRESS) && (current_mouse->left_button_state == GLFW_RELEASE);
    if (clicked) {
		if (edit_state->moving_shape) {
			ShapeDefinition *moved_to = &rendering_state->shapes[edit_state->moving_shape_index].shape_definition;
			printf("End Moving. Placed at: %f %f %f %f\n", moved_to->x1, moved_to->y1, moved_to->x2, moved_to->y2);

			edit_state->moving_shape = false;

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

        } else {
            int shape_clicked_index = FindShape(rendering_state, current_mouse->x_pos, current_mouse->y_pos);
            if (shape_clicked_index != -1) {
                edit_state->startX = current_mouse->x_pos;
                edit_state->startY = current_mouse->y_pos;
                edit_state->moving_shape = true;
                edit_state->moving_shape_index = shape_clicked_index;
                edit_state->moving_shape_original_defintion = rendering_state->shapes[shape_clicked_index].shape_definition;
            }
        }
    }
}

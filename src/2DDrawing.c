#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Rendering/Rendering.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	printf("Size Changed! %d, %d\n", width, height);
	glViewport(0, 0, width, height);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
		printf("[OpenGL Error](%d):%s\n", type, message);
}

typedef struct {
    bool last_left_mouse_down;
    bool following;
    float startX;
    float startY;
    SHAPE editing_shape_type;
} Control_State;

void process_input(GLFWwindow *window, Rendering_State *rendering_state, Control_State *control_state) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !control_state->following)
        control_state->editing_shape_type = RECT;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !control_state->following)
        control_state->editing_shape_type = TRIANGLE;
    
    double x_pos_pixel, y_pos_pixel;
    glfwGetCursorPos(window, &x_pos_pixel, &y_pos_pixel);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float x_pos = (x_pos_pixel - (width / 2.0f)) / ((float) width / 2.0f);
    float y_pos = -(y_pos_pixel - (height / 2.0f)) / ((float) height / 2.0f);

    if (control_state->following) {
        printf("Editing!\n");
        // preserve previous color
        rendering_state->edit_shape[control_state->editing_shape_type](rendering_state, control_state->startX, control_state->startY, x_pos, y_pos, 
                                                                       rendering_state->verticies[rendering_state->length - 1][2],
                                                                       rendering_state->verticies[rendering_state->length - 1][3],
                                                                       rendering_state->verticies[rendering_state->length - 1][4]);
    }

    if (control_state->last_left_mouse_down && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        printf("Mouse Change!\n");
        if (control_state->following) {
            control_state->following = false;
        } else if (rendering_state->length < MAX_SHAPES) {
            control_state->startX = x_pos;
            control_state->startY = y_pos;
            control_state->following = true;

            rendering_state->add_shape[control_state->editing_shape_type](rendering_state, x_pos, y_pos, x_pos, y_pos, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX);
        }
    }

    control_state->last_left_mouse_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

int main() {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(800, 600, "The Betterest Game", NULL, NULL);
	if (window == NULL) {
		//std::cout << "Failed to create GLFW window" << std::endl;
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
		printf("Failed to initialize OpenGL context\n");
		return -1;
	}

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glDebugMessageCallback(MessageCallback, 0);
	glViewport(0, 0, 800, 600);

    float vertices[] = {
     0.00f, 0.00f, 0.0f, 1.0f, 0.0f, // Vertex 1 (X, Y) (R, G, B)
     0.25f, 0.15f, 0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y)
     0.50f, 0.00f, 0.0f, 1.0f, 0.0f // Vertex 3 (X, Y)
    };

    Shader shader;
    Rendering_Shader_Initialize(&shader, "Shaders/VertexShader.vert", "Shaders/FragmentShader.frag");

    Control_State control_state;
    control_state.editing_shape_type = TRIANGLE;

    Rendering_State rendering_state;
    Rendering_State_Initialize(&rendering_state);
    
    Rendering_State_Add_Triangle(&rendering_state, 0.25f, 0.25f, 0.50f, 0.00f, 1.0f, 0.0f, 0.0f);
    Rendering_State_Add_Triangle(&rendering_state, -0.25f, 0.25f, 0.00f, 0.00f, 0.0f, 0.0f, 1.0f);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

	float lastTime = (float) glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = (float) glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

        glUseProgram(shader.programID);

        for (size_t i = 0; i < rendering_state.length; i++) {
            glBindVertexArray(rendering_state.vaos[i]);
            glBindBuffer(GL_ARRAY_BUFFER, rendering_state.vbos[i]);
            switch (rendering_state.shape_type[i]) {
                case RECT:
                    glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT_RECT);
                    break;
                case TRIANGLE:
                    glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT_TRIANGLE);
                    break;
                default:
                    printf("Unrecognized Shape!\n");
            }
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        process_input(window, &rendering_state, &control_state);

        glfwPollEvents();
    }

    Rendering_State_Free(&rendering_state);
    Rendering_Shader_FreeContent(&shader);
    glfwTerminate();

    return 0;
}

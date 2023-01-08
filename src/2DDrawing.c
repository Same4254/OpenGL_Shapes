#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "State/State.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	printf("Size Changed! %d, %d\n", width, height);
	glViewport(0, 0, width, height);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
		printf("[OpenGL Error](%d):%s\n", type, message);
}

int main() {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Not MSPaint", NULL, NULL);
	if (window == NULL) {
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

    Shader shader;
    Rendering_Shader_Initialize(&shader, "Shaders/VertexShader.vert", "Shaders/FragmentShader.frag");

    // initialize control states
    ControlState control_state;
    ControlState_Initialize(&control_state);

    RenderingState rendering_state;
    RenderingState_Initialize(&rendering_state);

    // Test Shapes
    ShapeDefinition def1;
    def1.x1 = 0.25f;
    def1.y1 = 0.25f;
    def1.x2 = 0.50f;
    def1.y2 = 0.00f;
    def1.r = 1.00f;
    def1.g = 0.00f;
    def1.b = 0.00f;
    
    ShapeDefinition def2;
    def2.x1 = -0.25f;
    def2.y1 = 0.25f;
    def2.x2 = 0.00f;
    def2.y2 = 0.00f;
    def2.r = 0.00f;
    def2.g = 0.00f;
    def2.b = 1.00f;

    RenderingState_Add_Triangle(&rendering_state, &def1, 0);
    RenderingState_Add_Triangle(&rendering_state, &def2, 1);

	float lastTime = (float) glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        float currentTime = (float) glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

        // Draw the shapes
        glUseProgram(shader.programID);

        for (size_t i = 0; i < rendering_state.length; i++) {
            Shape *shape = &rendering_state.shapes[i];
            glBindVertexArray(shape->vao);
            glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
            glDrawArrays(GL_TRIANGLES, 0, shape->vertex_count);
        }

        glfwSwapBuffers(window);

        ControlState_Update_Mouse(&control_state, window);
        ControlState_Update_Keyboard(&control_state, window);
        ControlState_Update_Mode(&control_state, &rendering_state);

        glfwPollEvents();
    }

    RenderingState_Free(&rendering_state);
    Rendering_Shader_FreeContent(&shader);
    glfwTerminate();

    return 0;
}

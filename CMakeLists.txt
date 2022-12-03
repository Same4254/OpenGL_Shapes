cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

project ("OpenGLGame")

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG        "3.3.6"
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glad
  GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
  GIT_TAG        "v0.1.36"
)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG        "0.9.9.8"
)
FetchContent_MakeAvailable(glm)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(2DDrawing "src/2DDrawing.c" "src/glad.c" "src/Rendering/Rendering.c" "src/Rendering/Shader.c" "src/Utils/Matrix.c" "src/Utils/Utils.c" "src/Utils/FileIO.c")

target_link_libraries (2DDrawing ${OPENGL_LIBRARY} glfw glm)

add_custom_target(copy-shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Shaders/ ${PROJECT_BINARY_DIR}/Shaders
        DEPENDS 2DDrawing)

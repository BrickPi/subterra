cmake_minimum_required(VERSION 3.20)
project("subterra" VERSION 0.1 LANGUAGES C CXX)

add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

add_subdirectory(lib/cglm)
include_directories(lib/cglm/include)

add_executable(subterra src/main.c src/glad.c src/shader.c)
target_include_directories(subterra PUBLIC inc)
target_link_libraries(subterra glfw ${GLFW_LIBRARIES} cglm)
file(COPY res DESTINATION ./)
if (WIN32)
    target_link_options(subterra PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()
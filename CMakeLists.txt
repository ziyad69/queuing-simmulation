cmake_minimum_required(VERSION 3.15)
project(QueueSimulation CXX)

set(CMAKE_CXX_STANDARD 17)

# ── Find system dependencies ──────────────────────────────────────────────────
find_package(OpenGL REQUIRED)

# GLFW — try system package first, then manual path
find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
    message(STATUS "glfw3 not found via CMake; set GLFW_DIR or install libglfw3-dev")
endif()

# ── Dear ImGui sources ────────────────────────────────────────────────────────
# Point IMGUI_DIR at your cloned imgui/ folder
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/imgui"
    CACHE PATH "Path to Dear ImGui source directory")

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ── Your project sources ──────────────────────────────────────────────────────
set(PROJECT_SOURCES
    imgui_main.cpp          # <── the new file (replaces main.cpp)
    src/customerType.cpp
    src/serverType.cpp
    src/serverListType.cpp
    src/validation.cpp
    src/errorHandler.cpp
)

# ── Build target ──────────────────────────────────────────────────────────────
add_executable(QueueSim ${PROJECT_SOURCES} ${IMGUI_SOURCES})

target_include_directories(QueueSim PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Ensure project headers in src/ are findable
target_include_directories(QueueSim PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(QueueSim PRIVATE
    OpenGL::GL
    glfw
)

# On Linux you may also need:
# target_link_libraries(QueueSim PRIVATE dl pthread)
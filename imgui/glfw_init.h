#ifndef GLFW_INIT
#define GLFW_INIT

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "imgui_impl_glfw.h"

#include <glad/glad.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <cstdio>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
//// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
//// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
//#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
//#pragma comment(lib, "legacy_stdio_definitions")
//#endif

static void glfw_error_callback(int error, const char* description)
{
    auto res = fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    std::cerr << "glfw_error_callback: " << res << "\n";
}

inline void* glad_load_proc_wrapper(const char* name) {
    return reinterpret_cast<void*>(glfwGetProcAddress(name));
}

static GLFWwindow* initialize_window_glsl_450(const int window_width, const int window_height)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Dear ImGui GLFW+OpenGL example", nullptr, nullptr);
    if (window == nullptr)
    {
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable / Disable vsync
    if (!gladLoadGLLoader(glad_load_proc_wrapper))
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        return nullptr;
    }
    return window;
}

#endif // !GLFW_INIT

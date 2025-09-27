#include "OpenGLApp.hpp"

#include "glad/glad.h" 
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../../helpers/simulationObjects.hpp"
#include "../ObjectRenderingCashing.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>

using namespace OpenGL;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


static void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

void OpenGL::App::startup()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

    const char* glsl_version = "#version 450 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
		throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGL())
        throw std::runtime_error("Failed to initialize GLAD");
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.5f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void OpenGL::App::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool App::update() {
    if(glfwWindowShouldClose(window))
		return false;

    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return true;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    _lastCash = objectLists::objectCash.getsimulationTimeCash(timeObjects::realRunTime);
	deltaTime = io->DeltaTime;

	for (auto i = _windows.begin(); i != _windows.end();) {
		if (!i->get()->render()) [[unlikely]]
			i = _windows.erase(i);
		else [[likely]]
			++i;
	}

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
    glFinish();

	return true;
}

bool OpenGL::App::hasWindow(const std::string& name)
{
    for (auto& w : _windows) {
        if (w->name == name)
            return true;
    }
    return false;
}

ImguiWindow& OpenGL::App::getWindow(const std::string& name)
{
    for (auto& w : _windows) {
        if (w->name == name)
            return *w;
	}
	throw std::runtime_error("No window with name: " + name);
}

// Must be called after ImGui::NewFrame();
GLFWwindow* OpenGL::App::getGLFWwindow()
{
    ImGuiViewport* viewport = ImGui::GetWindowViewport();
    return static_cast<GLFWwindow*>(viewport->PlatformHandle);
}

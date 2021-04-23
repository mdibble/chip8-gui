#include "ui.h"

UI::UI() {
    UIState state;
}

void UI::init(CHIP8* system) {
    this->system = system;
    glfwInit();

    const char* glsl_version = "#version 130";

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    this->window = glfwCreateWindow(1440, 720, "Test", NULL, NULL);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);

    gladLoadGL();

    ImGui::CreateContext();
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void UI::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void UI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    glfwPollEvents();

    ImGui::NewFrame();
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load ROM")) { }
            if (ImGui::MenuItem("Reset")) { Actions::reset(this->system); }
            ImGui::Separator();
            if (ImGui::MenuItem("About")) { this->state.aboutWindow = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Debugger")) { this->state.debugWindow = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (this->state.debugWindow) {
        ImGuiIO &io = ImGui::GetIO(); (void)io;
        ImGui::Begin("Debugger", &this->state.debugWindow);
        ImGui::Text("Emulator Information");
        ImGui::Text("PC: 0x%.04x", this->system->pc);
        ImGui::Text("Registers:");
        for (int i = 0; i < 16; i += 1) {
            ImGui::Text("V%02d: 0x%02x", i, this->system->r[0]);
        }
        ImGui::Separator();
        ImGui::Text("Software Information");
        ImGui::Text("Frame %d (%.1f FPS)", ImGui::GetFrameCount(), io.Framerate);
        ImGui::End();
    }

    if (this->state.aboutWindow) {
        ImGui::Begin("About", &this->state.aboutWindow);
        ImGui::Text("CHIP-8 Emulator");
        ImGui::Text("By Matthew Dibble (github.com/mdibble)");
        ImGui::End();
    }

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(this->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);

    this->system->renderer.render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->window);
}

bool UI::isRunning() {
    return !glfwWindowShouldClose(this->window);
}

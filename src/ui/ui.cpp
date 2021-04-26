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

    this->window = glfwCreateWindow(1440, 720, "CHIP-8 GUI", NULL, NULL);
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
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load ROM")) { Actions::loadRom(this->system); }
            ImGui::Separator();
            if (ImGui::MenuItem("About")) { this->state.aboutWindow = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (this->system->running == true) {
                if (ImGui::MenuItem("Stop")) { Actions::pause(this->system); }
            }
            else {
                if (ImGui::MenuItem("Start")) { Actions::resume(this->system); }
            }
            if (ImGui::MenuItem("Reset")) { Actions::reset(this->system); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Debugger")) { this->state.debugWindow = true; }
            if (ImGui::MenuItem("Memory")) { this->state.memoryWindow = true; }
            if (ImGui::MenuItem("Display")) { this->state.displayWindow = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (this->state.debugWindow) {
        ImGuiIO &io = ImGui::GetIO(); (void)io;
        ImGui::Begin("Debugger", &this->state.debugWindow);
        ImGui::Text("Emulator Information");
        ImGui::Text("Running: %s", this->system->running == true ? "Yes" : "No");
        ImGui::Text("PC: 0x%04x", this->system->pc);
        ImGui::Text("Opcode: 0x%04x", (this->system->mem[this->system->pc] << 8) | (this->system->mem[this->system->pc + 1]));
        ImGui::Text("Stack Pointer: 0x%04x", this->system->sp);
        ImGui::Text("I: 0x%04x", this->system->i);
        ImGui::Text("Delay Timer: 0x%02x", this->system->dt);
        ImGui::Text("Sound Timer: 0x%02x", this->system->st);
        ImGui::Text("Registers:");
        for (int i = 0; i < 16; i += 1) {
            if (i != 0 && i % 4 != 0) {
                ImGui::SameLine();
            }
            ImGui::Text("V%01x: 0x%02x", i, this->system->r[i]);
            
        }
        ImGui::Separator();
        ImGui::Text("Emulator Controls");
        if (this->system->running == true) {
            if (ImGui::Button("Stop")) { Actions::pause(this->system); }
        }
        else {
            if (ImGui::Button("Start")) { Actions::resume(this->system); }
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) { Actions::reset(this->system); }
        ImGui::SameLine();
        if (ImGui::Button("Step")) { Actions::step(this->system); }
        ImGui::SameLine();
        if (ImGui::Button("Load ROM")) { Actions::loadRom(this->system); }
        ImGui::Separator();
        ImGui::Text("Software Information");
        ImGui::Text("Frame %d (%.1f FPS)", ImGui::GetFrameCount(), io.Framerate);
        ImGui::End();
    }

    if (this->state.memoryWindow) {
        const ImVec4 grey = { 0.5f, 0.5f, 0.5f, 1.0f };
        ImGui::Begin("Memory", &this->state.memoryWindow);
        ImGui::Text("    ");
        for (int i = 0; i < 16; i += 1) {
            ImGui::SameLine();
            ImGui::TextColored(grey, "%02x", i);
        }
        for (int i = 0; i < 256; i += 1) {
            ImGui::TextColored(grey, "%04x", i * 16);
            for (int j = 0; j < 16; j += 1) {
                ImGui::SameLine();
                ImGui::Text("%02x", system->mem[(i * 16) + j]);
            }
        }
        ImGui::End();
    }

    if (this->state.aboutWindow) {
        ImGui::Begin("About", &this->state.aboutWindow);
        ImGui::Text("CHIP-8 Emulator");
        ImGui::Text("By Matthew Dibble (github.com/mdibble)");
        ImGui::End();
    }

    if (this->state.displayWindow) {
        ImGui::Begin("Display", &this->state.displayWindow);
        for (int i = 0; i < 32; i += 1) {
            for (int j = 0; j < 64; j += 1) {
                if (system->renderer.display[i][j] == true) {
                    ImGui::Text("@");
                }
                else {
                    const ImVec4 black = { 0.0f, 0.0f, 0.0f, 1.0f };
                    ImGui::TextColored(black, "@");
                }
                if (j != 63) {
                    ImGui::SameLine();
                }
            }
        }
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

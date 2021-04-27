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

void UI::updateKeypad() {
    this->keypad[0] = glfwGetKey(this->window, GLFW_KEY_X);
    this->keypad[1] = glfwGetKey(this->window, GLFW_KEY_1);
    this->keypad[2] = glfwGetKey(this->window, GLFW_KEY_2);
    this->keypad[3] = glfwGetKey(this->window, GLFW_KEY_3);
    this->keypad[4] = glfwGetKey(this->window, GLFW_KEY_Q);
    this->keypad[5] = glfwGetKey(this->window, GLFW_KEY_W);
    this->keypad[6] = glfwGetKey(this->window, GLFW_KEY_E);
    this->keypad[7] = glfwGetKey(this->window, GLFW_KEY_A);
    this->keypad[8] = glfwGetKey(this->window, GLFW_KEY_S);
    this->keypad[9] = glfwGetKey(this->window, GLFW_KEY_D);
    this->keypad[10] = glfwGetKey(this->window, GLFW_KEY_Z);
    this->keypad[11] = glfwGetKey(this->window, GLFW_KEY_C);
    this->keypad[12] = glfwGetKey(this->window, GLFW_KEY_4);
    this->keypad[13] = glfwGetKey(this->window, GLFW_KEY_R);
    this->keypad[14] = glfwGetKey(this->window, GLFW_KEY_F);
    this->keypad[15] = glfwGetKey(this->window, GLFW_KEY_V);
}

void UI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    glfwPollEvents();
    updateKeypad();

    Actions::setKeypad(this->system, this->keypad);

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
            if (ImGui::MenuItem("Keypad")) { this->state.keypadWindow = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (this->state.debugWindow) {
        ImGuiIO &io = ImGui::GetIO(); (void)io;
        ImGui::Begin("Debugger", &this->state.debugWindow);
        ImGui::Text("Emulator Information");
        ImGui::Text("Running: %s", this->system->running == true ? "Yes" : "No");
        ImGui::Text("Cycle Count: %d", this->system->clockNum);
        ImGui::Text("PC: 0x%04x", this->system->pc);
        ImGui::Text("Opcode: 0x%04x", (this->system->mem[this->system->pc] << 8) | (this->system->mem[this->system->pc + 1]));
        ImGui::Text("Stack Pointer: 0x%01x", this->system->sp);
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
        ImGui::Text("System Speed: %dx", this->system->systemSpeed);
        ImGui::SameLine();
        if (ImGui::Button("-")) { Actions::decreaseSpeed(this->system); }
        ImGui::SameLine();
        if (ImGui::Button("+")) { Actions::increaseSpeed(this->system); }
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

    if (this->state.keypadWindow) {
        const ImVec4 grey = { 0.5f, 0.5f, 0.5f, 1.0f };
        ImGui::Begin("Keypad", &this->state.keypadWindow);
        if (this->system->keypad[0x01] == true) { ImGui::Text("1"); } else { ImGui::TextColored(grey, "1"); } ImGui::SameLine();
        if (this->system->keypad[0x02] == true) { ImGui::Text("2"); } else { ImGui::TextColored(grey, "2"); } ImGui::SameLine();
        if (this->system->keypad[0x03] == true) { ImGui::Text("3"); } else { ImGui::TextColored(grey, "3"); } ImGui::SameLine();
        if (this->system->keypad[0x0C] == true) { ImGui::Text("C"); } else { ImGui::TextColored(grey, "C"); }
        if (this->system->keypad[0x04] == true) { ImGui::Text("4"); } else { ImGui::TextColored(grey, "4"); } ImGui::SameLine();
        if (this->system->keypad[0x05] == true) { ImGui::Text("5"); } else { ImGui::TextColored(grey, "5"); } ImGui::SameLine();
        if (this->system->keypad[0x06] == true) { ImGui::Text("6"); } else { ImGui::TextColored(grey, "6"); } ImGui::SameLine();
        if (this->system->keypad[0x0D] == true) { ImGui::Text("D"); } else { ImGui::TextColored(grey, "D"); }
        if (this->system->keypad[0x07] == true) { ImGui::Text("7"); } else { ImGui::TextColored(grey, "7"); } ImGui::SameLine();
        if (this->system->keypad[0x08] == true) { ImGui::Text("8"); } else { ImGui::TextColored(grey, "8"); } ImGui::SameLine();
        if (this->system->keypad[0x09] == true) { ImGui::Text("9"); } else { ImGui::TextColored(grey, "9"); } ImGui::SameLine();
        if (this->system->keypad[0x0E] == true) { ImGui::Text("E"); } else { ImGui::TextColored(grey, "E"); }
        if (this->system->keypad[0x0A] == true) { ImGui::Text("A"); } else { ImGui::TextColored(grey, "A"); } ImGui::SameLine();
        if (this->system->keypad[0x00] == true) { ImGui::Text("0"); } else { ImGui::TextColored(grey, "0"); } ImGui::SameLine();
        if (this->system->keypad[0x0B] == true) { ImGui::Text("B"); } else { ImGui::TextColored(grey, "B"); } ImGui::SameLine();
        if (this->system->keypad[0x0F] == true) { ImGui::Text("F"); } else { ImGui::TextColored(grey, "F"); }
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

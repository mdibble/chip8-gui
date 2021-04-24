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
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load ROM")) {
#ifdef _WIN32
                OPENFILENAMEA ofile = { 0 };
                char fpath[_MAX_PATH] = { 0 };

                ofile.lStructSize = sizeof( ofile );
                ofile.hwndOwner = GetActiveWindow();
                ofile.lpstrFile = fpath;
                ofile.nMaxFile = sizeof( fpath );
                if ( GetOpenFileNameA( &ofile ) ) {
                    Actions::reset(this->system);
                    Actions::loadRom(this->system, fpath);
                }
#elif __gnu_linux__
                char fpath[1024];
                FILE* hFile = popen( "zenity --file-selection", "r" );
                fgets( fpath, sizeof( fpath ), hFile );
                if ( fpath[strlen( fpath ) - 1] == '\n' ) {
                    fpath[strlen( fpath ) - 1] = 0;
                }
                Actions::reset(this->system);
                Actions::loadRom(this->system, fpath);
#endif
            }
            if (ImGui::MenuItem("Reset")) { Actions::reset(this->system); }
            ImGui::Separator();
            if (ImGui::MenuItem("About")) { this->state.aboutWindow = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Debugger")) { this->state.debugWindow = true; }
            if (ImGui::MenuItem("Memory")) { this->state.memoryWindow = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (this->state.debugWindow) {
        ImGuiIO &io = ImGui::GetIO(); (void)io;
        ImGui::Begin("Debugger", &this->state.debugWindow);
        ImGui::Text("Emulator Information");
        ImGui::Text("PC: 0x%04x", this->system->pc);
        ImGui::Text("Opcode: 0x%04x", (this->system->mem[this->system->pc] << 8) | (this->system->mem[this->system->pc + 1]));
        ImGui::Text("Stack Pointer: 0x%04x", this->system->sp);
        ImGui::Text("Registers:");
        for (int i = 0; i < 16; i += 1) {
            if (i != 0 && i % 4 != 0) {
                ImGui::SameLine();
            }
            ImGui::Text("V%01x: 0x%02x", i, this->system->r[0]);
            
        }
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

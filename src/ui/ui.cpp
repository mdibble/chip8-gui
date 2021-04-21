#include "ui.h"

UI::UI() {
    UIState state;
}

void UI::init(CHIP8* system) {
    this->system = system;
    glfwInit();

    const char* glsl_version = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    this->window = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
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
                    reset(this->system);
                    loadRom(this->system, fpath);
                }
#elif __gnu_linux__
                char fpath[1024];
                FILE* hFile = popen( "zenity --file-selection", "r" );
                fgets( fpath, sizeof( fpath ), hFile );
                if ( fpath[strlen( fpath ) - 1] == '\n' ) {
                    fpath[strlen( fpath ) - 1] = 0;
                }
                reset(this->system);
                loadRom(this->system, fpath);
#endif
            }
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
    ImGui::Begin("Hello, world!");
    ImGui::End();

    if (this->state.debugWindow) {
        ImGui::Begin("Debugger", &this->state.debugWindow);
        ImGui::Text("Debug");
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

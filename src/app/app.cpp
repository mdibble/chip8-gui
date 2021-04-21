#include "app.h"

App::App() {
    UI ui;
    CHIP8 chip8;
};

void App::run() {
    ui.init(&chip8);
    while (ui.isRunning()) {
        ui.render();
    }
    ui.destroy();
}

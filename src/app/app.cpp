#include "app.h"

App::App() {
    UI ui;
    CHIP8 chip8;
};

void App::run() {
    this->ui.init(&(this->chip8));
    this->chip8.renderer.init();
    while (this->ui.isRunning()) {
        this->ui.render();
    }
    this->ui.destroy();
}

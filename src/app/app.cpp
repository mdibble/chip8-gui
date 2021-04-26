#include "app.h"

App::App() {
    UI ui;
    CHIP8 chip8;
};

void App::run() {
    this->chip8.reset();
    this->ui.init(&(this->chip8));
    this->chip8.renderer.init();
    while (this->ui.isRunning()) {
        this->chip8.cycle();
        if (this->chip8.frameComplete == true) {
            this->chip8.renderer.render();
            this->ui.render();
            this->chip8.frameComplete = false;
        }
    }
    this->ui.destroy();
}

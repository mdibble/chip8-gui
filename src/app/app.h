#pragma once

#include "../ui/ui.h"
#include "../chip8/chip8.h"

class App {
private:
public:
    App();
    void run();
    UI ui;
    CHIP8 chip8;
};

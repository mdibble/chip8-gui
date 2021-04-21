#pragma once

#include <stdint.h>

#include "renderer.h"

class CHIP8 {
private:
public:
    uint8_t r[0x0F];
    uint16_t pc;
    uint8_t mem[0x1000];
    Renderer renderer;
};

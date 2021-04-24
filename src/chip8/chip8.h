#pragma once

#include <stdint.h>

#include "renderer.h"

class CHIP8 {
private:
public:
    uint8_t r[0x0F];
    uint16_t pc;
    uint8_t mem[0x1000];
    uint16_t i;
    uint16_t stack[0x0F];
    uint8_t sp;
    uint8_t dt;
    uint8_t st;
    Renderer renderer;
    bool frameComplete;
    void reset();
    void cycle();
};

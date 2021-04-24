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
    uint16_t opcode;
    Renderer renderer;
    bool frameComplete;
    bool running;
    void reset();
    void cycle();
    void op_00e0();
    void op_3xnn();
    void op_6xnn();
};

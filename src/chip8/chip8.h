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
    void op_0nnn();
    void op_00e0();
    void op_00ee();
    void op_1nnn();
    void op_2nnn();
    void op_3xnn();
    void op_4xnn();
    void op_5xy0();
    void op_6xnn();
    void op_7xnn();
    void op_8xy0();
    void op_8xy1();
    void op_8xy2();
    void op_8xy3();
    void op_8xy4();
    void op_8xy5();
    void op_8xy6();
    void op_8xy7();
    void op_8xye();
    void op_9xy0();
    void op_annn();
    void op_bnnn();
    void op_cxnn();
    void op_dxyn();
    void op_ex9e();
    void op_exa1();
    void op_fx07();
    void op_fx0a();
    void op_fx15();
    void op_fx18();
    void op_fx1e();
    void op_fx29();
    void op_fx33();
    void op_fx55();
    void op_fx65();
};

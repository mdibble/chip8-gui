#include "chip8.h"

void CHIP8::reset() {
    for (int i = 0; i < 0x1000; i += 1) {
        this->mem[i] = 0x00;
    }
    this->pc = 0x0200;
    this->i  = 0x0000;
    this->sp = 0x00;
    this->dt = 0x00;
    this->st = 0x00;
    for (int i = 0; i < 0x0F; i += 1) {
        this->r[i] = 0x00;
        this->stack[i] = 0x0000;
    }
    this->renderer.clearCanvas();
};

void CHIP8::cycle() {
    uint16_t opcode = this->mem[this->pc] << 8 | this->mem[this->pc + 1];

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x0FFF) {
                case 0x00E0:
                    // 00E0
                    break;
                case 0x00EE:
                    // 00EE
                    break;
                default:
                    // 0NNN
                    break;
            }
            break;
        case 0x1000:
            // 1NNN
            break;
        case 0x2000:
            // 2NNN
            break;
        case 0x3000:
            // 3XNN
            break;
        case 0x4000:
            // 4XNN
            break;
        case 0x5000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    // 5XY0
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    //this->pc += 2;
    this->frameComplete = true;
}

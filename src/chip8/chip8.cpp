#include "chip8.h"

void CHIP8::reset() {
    this->running = false;

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
};

void CHIP8::cycle() {
    if (this->running == false) {
        this->frameComplete = true;
        return;
    }

    this->opcode = this->mem[this->pc] << 8 | this->mem[this->pc + 1];
    switch (this->opcode & 0xF000) {
        case 0x0000:
            switch (this->opcode & 0x0FFF) {
                case 0x00E0:
                    // 00E0
                    this->op_00e0();
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
            this->op_3xnn();
            break;
        case 0x4000:
            // 4XNN
            break;
        case 0x5000:
            switch (this->opcode & 0x000F) {
                case 0x0000:
                    // 5XY0
                    break;
                default:
                    break;
            }
            break;
        case 0x6000:
            this->op_6xnn();
            break;
        default:
            break;
    }
    this->pc += 2;
    this->frameComplete = true;
}

void CHIP8::op_00e0() {
    this->renderer.clearCanvas();
}

void CHIP8::op_3xnn() {
   if (this->r[(this->opcode & 0x0F00) >> 8] == (this->opcode & 0x00FF)) {
       this->pc += 2;
   }
    // Needs to be tested
}

void CHIP8::op_6xnn() {
    this->r[(this->opcode & 0x0F00) >> 8] = this->opcode & 0x00FF;
}

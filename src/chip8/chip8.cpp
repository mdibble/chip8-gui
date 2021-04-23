#include "chip8.h"

void CHIP8::reset() {
    for (int i = 0; i < 0x1000; i += 1) {
        this->mem[i] = 0x00;
    }
    this->pc = 0x0000;
    this->i = 0x0000;
    for (int i = 0; i < 0x0F; i += 1) {
        this->r[i] = 0x00;
    }
    this->renderer.clearCanvas();
};

void CHIP8::cycle() {
    this->pc += 1;
    if (this->pc % 144 == 0) {
        this->frameComplete = true;
    }
}

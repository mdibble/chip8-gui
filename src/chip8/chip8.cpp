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
    for (int i = 0; i <= 0x0F; i += 1) {
        this->r[i] = 0x00;
        this->stack[i] = 0x0000;
    }

    this->renderer.clearCanvas();
};

void CHIP8::cycle() {
    if (this->running == false) {
        this->frameComplete = true;
        return;
    }

    if (this->dt > 0) {
        this->dt -= 1;
    }
    if (this->st > 0) {
        this->st -= 1;
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
                    this->op_00ee();
                    break;
                default:
                    // 0NNN
                    this->op_0nnn();
                    break;
            }
            break;
        case 0x1000:
            // 1NNN
            this->op_1nnn();
            break;
        case 0x2000:
            // 2NNN
            this->op_2nnn();
            break;
        case 0x3000:
            this->op_3xnn();
            break;
        case 0x4000:
            // 4XNN
            this->op_4xnn();
            break;
        case 0x5000:
            // 5XY0
            this->op_5xy0();
            break;
        case 0x6000:
            // 6XNN
            this->op_6xnn();
            break;
        case 0x7000:
            // 7XNN
            this->op_7xnn();
            break;
        case 0x8000:
            switch (this->opcode & 0x000F) {
                case 0x0000:
                    // 8XY0
                    this->op_8xy0();
                    break;
                case 0x0001:
                    // 8XY1
                    this->op_8xy1();
                    break;
                case 0x0002:
                    // 8XY2
                    this->op_8xy2();
                    break;
                case 0x0003:
                    // 8XY3
                    this->op_8xy3();
                    break;
                case 0x0004:
                    // 8XY4
                    this->op_8xy4();
                    break;
                case 0x0005:
                    // 8XY5
                    this->op_8xy5();
                    break;
                case 0x0006:
                    // 8XY6
                    this->op_8xy6();
                    break;
                case 0x0007:
                    // 8XY7
                    this->op_8xy7();
                    break;
                case 0x000E:
                    // 8XYE
                    this->op_8xye();
                    break;
                default:
                    break;
            }
            break;
        case 0x9000:
            // 9XY0
            this->op_9xy0();
            break;
        case 0xA000:
            // ANNN
            this->op_annn();
            break;
        case 0xB000:
            // BNNN
            this->op_bnnn();
            break;
        case 0xC000:
            // CXNN
            this->op_cxnn();
            break;
        case 0xD000:
            // DXYN
            this->op_dxyn();
            break;
        case 0xE000:
            switch (this->opcode & 0x00FF) {
                case 0x009E:
                    // EX9E
                    this->op_ex9e();
                    break;
                case 0x00A1:
                    // EXA1
                    this->op_exa1();
                    break;
                default:
                    break;
            }
            break;
        case 0xF000:
            switch (this->opcode & 0x00FF) {
                case 0x0007:
                    // FX07
                    this->op_fx07();
                    break;
                case 0x000A:
                    // FX0A
                    this->op_fx0a();
                    break;
                case 0x0015:
                    // FX15
                    this->op_fx15();
                    break;
                case 0x0018:
                    // FX18
                    this->op_fx18();
                    break;
                case 0x001E:
                    // FX1E
                    this->op_fx1e();
                    break;
                case 0x0029:
                    // FX29
                    this->op_fx29();
                    break;
                case 0x0033:
                    // FX33
                    this->op_fx33();
                    break;
                case 0x0055:
                    // FX55
                    this->op_fx55();
                    break;
                case 0x0065:
                    // FX65
                    this->op_fx65();
                    break;
            }
            break;
        default:
            break;
    }
    this->pc += 2;
    this->frameComplete = true; // Slows down the emulator but good for debugging
}

void CHIP8::op_0nnn() {
    throw(1);
}

void CHIP8::op_00e0() {
    this->renderer.clearCanvas();
}

void CHIP8::op_00ee() {
    this->sp -= 1;

    this->pc = this->stack[this->sp];
    this->pc -= 2;
}

void CHIP8::op_1nnn() {
    uint16_t nnn = (this->opcode & 0x0FFF);

    this->pc = nnn;
    this->pc -= 2;
}

void CHIP8::op_2nnn() {
    uint16_t nnn = (this->opcode & 0x0FFF);

    this->stack[this->sp] = (this->pc + 2);
    this->sp += 1;
    this->pc = nnn;
    this->pc -= 2;
}

void CHIP8::op_3xnn() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint16_t nn = (this->opcode & 0x00FF);

    if (this->r[x] == nn) {
       this->pc += 2;
    }
}

void CHIP8::op_4xnn() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint16_t nn = (this->opcode & 0x00FF);

    if (this->r[x] != nn) {
       this->pc += 2;
    }
}

void CHIP8::op_5xy0() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;

    if (this->r[x] == this->r[y]) {
        this->pc += 2;
    }
}

void CHIP8::op_6xnn() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint16_t nn = (this->opcode & 0x00FF);

    this->r[x] = nn;
}

void CHIP8::op_7xnn() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint16_t nn = (this->opcode & 0x00FF);

    this->r[x] += nn;
}

void CHIP8::op_8xy0() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;
    
    this->r[x] = this->r[y];
}

void CHIP8::op_8xy1() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;

    this->r[x] = this->r[x] | this->r[y];
}

void CHIP8::op_8xy2() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;

    this->r[x] = this->r[x] & this->r[y];
}

void CHIP8::op_8xy3() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;

    this->r[x] = this->r[x] ^ this->r[y];
}

void CHIP8::op_8xy4() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;
    bool carry = false;

    if ((r[x] + r[y]) < r[x]) { carry = true; }
    this->r[x] += this->r[y];
    this->r[0x0F] = carry == true ? 1 : 0;
}

void CHIP8::op_8xy5() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;
    bool borrow = false;

    if (r[y] > r[x]) { borrow = true; }
    this->r[x] -= (this->r[y]);
    this->r[0x0F] = borrow == true ? 0 : 1;
}

void CHIP8::op_8xy6() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->r[0x0F] = (this->r[x] & 0x0001);
    this->r[x] = (this->r[x] >> 1);
}

void CHIP8::op_8xy7() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;
    bool borrow = false;

    if (r[x] > r[y]) { borrow = true; }
    this->r[x] = this->r[y] - this->r[x];
    this->r[0x0F] = borrow == true ? 0 : 1;
}

void CHIP8::op_8xye() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->r[0x0F] = (this->r[x] >> 7);
    this->r[x] = (this->r[x] << 1);
}

void CHIP8::op_9xy0() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;
    
    if (this->r[x] != this->r[y]) {
        this->pc += 2;
    }
}

void CHIP8::op_annn() {
    uint16_t nnn = (this->opcode & 0x0FFF);

    this->i = nnn;
}

void CHIP8::op_bnnn() {
    throw(1);
}

void CHIP8::op_cxnn() {
    throw(1);
}

void CHIP8::op_dxyn() {
    uint8_t n = (this->opcode & 0x000F);
    uint8_t x = (this->opcode & 0x0F00) >> 8;
    uint8_t y = (this->opcode & 0x00F0) >> 4;
    bool unset = false;

    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            if (((this->mem[this->i + i]) >> (7 - j)) & 0x0001 == 0x0001) {
                if (this->renderer.display[this->r[y] + i][this->r[x] + j] == true) {
                    this->renderer.display[this->r[y] + i][this->r[x] + j] = false;
                    unset = true;
                }
                else {
                    this->renderer.display[this->r[y] + i][this->r[x] + j] = true;
                }
            }
        }
    }

    if (unset == true) {
        this->r[0x0F] = 1;
    }
    else {
        this->r[0x0F] = 0;
    }

    this->frameComplete = true;
}

void CHIP8::op_ex9e() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    if (this->keypad[x] == true) {
        this->pc += 2;
    }
}

void CHIP8::op_exa1() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    if (this->keypad[x] == false) {
        this->pc += 2;
    }
}

void CHIP8::op_fx07() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->r[x] = this->dt;
}

void CHIP8::op_fx0a() {
    throw(1);
}

void CHIP8::op_fx15() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->dt = this->r[x];
}

void CHIP8::op_fx18() {
    throw(1);
}

void CHIP8::op_fx1e() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->i += this->r[x];
}

void CHIP8::op_fx29() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->i = 5 * this->r[x];
}

void CHIP8::op_fx33() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    this->mem[this->i + 0] = (this->r[x] % 1000) / 100;
    this->mem[this->i + 1] = (this->r[x] %  100) /  10;
    this->mem[this->i + 2] = (this->r[x] %   10) /   1;
}

void CHIP8::op_fx55() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; i += 1) {
        this->mem[this->i + i] = this->r[i];
    }
}

void CHIP8::op_fx65() {
    uint8_t x = (this->opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; i += 1) {
        this->r[i] = this->mem[this->i + i];
    }
}

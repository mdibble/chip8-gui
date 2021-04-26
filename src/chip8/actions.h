#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif __gnu_linux__
#include <byteswap.h>
#endif

#include <fstream>

#include "chip8.h"

namespace Actions {
    void loadRom(CHIP8* system);
    void loadFont(CHIP8* system);
    void reset(CHIP8* system);
    void pause(CHIP8* system);
    void resume(CHIP8* system);
    void step(CHIP8* system);
    void setKeypad(CHIP8* system, bool keypad[]);
}

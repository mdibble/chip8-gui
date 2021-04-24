#pragma once

#include <fstream>

#include "chip8.h"

namespace Actions {
    void loadRom(CHIP8* system, char* filepath);
    void reset(CHIP8* system);
}

#include "actions.h"

namespace Actions {
    void loadRom(CHIP8* system, char* filepath) {
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        std::string fileData;

        try {
            file.open(filepath);

            std::stringstream fileStream;

            fileStream << file.rdbuf();
            file.close();

            fileData = fileStream.str();
        }
        catch (std::ifstream::failure &err) {
        std::cout << "Error: file couldn't be read" << std::endl;
        }
        for (int i = 0; i < fileData.length(); i += 1) {
            system->mem[0x200 + i] = fileData.at(i);
        }
    }

    void reset(CHIP8* system) {
        system->reset();
    }
}

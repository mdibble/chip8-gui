#include "actions.h"

namespace Actions {
    void loadRom(CHIP8* system) {
#ifdef _WIN32
        OPENFILENAMEA ofile = { 0 };
        char fpath[_MAX_PATH] = { 0 };

        ofile.lStructSize = sizeof( ofile );
        ofile.hwndOwner = GetActiveWindow();
        ofile.lpstrFile = fpath;
        ofile.nMaxFile = sizeof( fpath );
        if ( GetOpenFileNameA( &ofile ) ) {
            reset(system);
        }
#elif __gnu_linux__
        char fpath[1024];
        FILE* hFile = popen( "zenity --file-selection", "r" );
        fgets( fpath, sizeof( fpath ), hFile );
        if ( fpath[strlen( fpath ) - 1] == '\n' ) {
            fpath[strlen( fpath ) - 1] = 0;
        }
        reset(system);
#endif
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        std::string fileData;

        try {
            file.open(fpath);

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

    void pause(CHIP8* system) {
        system->running = false;
    }

    void resume(CHIP8* system) {
        system->running = true;
    }

    void step(CHIP8* system) {
        system->running = true;
        system->cycle();
        system->running = false;
    }
}

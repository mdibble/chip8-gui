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
        FILE* file = fopen(fpath, "rb");
        fread(&system->mem[0x200], 1, 0x800, file);

        loadFont(system);
    }

    void loadFont(CHIP8* system) {
        uint8_t fontset[] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,
            0x20, 0x60, 0x20, 0x20, 0x70,
            0xF0, 0x10, 0xF0, 0x80, 0xF0,
            0xF0, 0x10, 0xF0, 0x10, 0xF0,
            0x90, 0x90, 0xF0, 0x10, 0x10,
            0xF0, 0x80, 0xF0, 0x10, 0xF0,
            0xF0, 0x80, 0xF0, 0x90, 0xF0,
            0xF0, 0x10, 0x20, 0x40, 0x40,
            0xF0, 0x90, 0xF0, 0x90, 0xF0,
            0xF0, 0x90, 0xF0, 0x10, 0xF0,
            0xF0, 0x90, 0xF0, 0x90, 0x90,
            0xE0, 0x90, 0xE0, 0x90, 0xE0,
            0xF0, 0x80, 0x80, 0x80, 0xF0,
            0xE0, 0x90, 0x90, 0x90, 0xE0,
            0xF0, 0x80, 0xF0, 0x80, 0xF0,
            0xF0, 0x80, 0xF0, 0x80, 0x80,
        };
        for (int i = 0; i < 0x50; i += 1) {
            system->mem[i] = fontset[i];
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

    void setKeypad(CHIP8* system, bool keypad[]) {
        for (int i = 0; i <= 16; i += 1) {
            system->keypad[i] = keypad[i];
        }
    }

    void decreaseSpeed(CHIP8* system) {
        if (system->systemSpeed > 1) {
            system->systemSpeed -= 1;
        }
    }

    void increaseSpeed(CHIP8* system) {
        if (system->systemSpeed < 8) {
            system->systemSpeed += 1;
        }
    }
}

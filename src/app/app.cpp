#include "app.h"

App::App() {
    UI ui;
};

void App::run() {
    ui.init();
    while (ui.isRunning()) {
        ui.render();
    }
    ui.destroy();
}

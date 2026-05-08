#include "HospitalSystem.h"
#include "SFMLUI.h"
#include <iostream>

namespace sf {}
using namespace std;
using namespace sf;


static bool setupSystem(HospitalSystem& system) {
    if (!system.initialize()) {
        std::cout << "Failed to initialize data files.\n";
        return false;
    }
    if (!system.loadData()) {
        std::cout << "Failed to load data files.\n";
        return false;
    }
    return true;
}

static void launchApplication(HospitalSystem& system) {
#ifdef HMS_ENABLE_SFML
    SFMLUI ui;
    ui.run(system);
#else
    system.run();
#endif
}

int main() {
    HospitalSystem system;
    if (!setupSystem(system)) {
        return 1;
    }
    launchApplication(system);
    return 0;
}

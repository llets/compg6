#include "mainhandler.h"
#include "glut_backend.h"
#include <Magick++.h>

int main(int argc, char** argv)
{
    srand(time(0));
    Magick::InitializeMagick(*argv);
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 41")) {
        return 1;
    }

    mainhandler* app = new mainhandler();

    if (!app->Init()) {
        return 1;
    }
    app->Run();
    delete app;
    return 0;
}
#include "mainhandler.h"
#include "ogldev_glut_backend.h"
#include <Magick++.h>

int main(int argc, char** argv)
{
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
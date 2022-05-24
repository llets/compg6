#include "glut_backend.h"
#include "mainhandler.h"
#include <Magick++.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

int main(int argc, char** argv)
{
    Magick::InitializeMagick(*argv);
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 43")) {
        return 1;
    }

    mainhandler app;
    if (!app.Init()) {
        return 1;
    }
    app.Run();
    return 0;
}
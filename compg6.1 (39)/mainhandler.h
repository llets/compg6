#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "silhouette_technique.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"

using namespace std;

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 800

class mainhandler: public ICallbacks
{
public:

    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    virtual void IdleCB();
    virtual void SpecialKeyboardCB(int Key, int x, int y);
    virtual void KeyboardCB(unsigned char Key, int x, int y);
    virtual void PassiveMouseCB(int x, int y);
    virtual void MouseCB(int Button, int State, int x, int y);

private:

    void RenderScene();
    void CalcFPS();
    void RenderFPS();
    LightingTechnique m_LightingTech;
    SilhouetteTechnique m_silhouetteTech;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
    glm::vec3 m_boxPos;
    Mesh m_mesh;
    ProjInfo m_persProjInfo;
    int m_glutTime;
    long long m_startTime;
    int m_frameCount;
    float m_fps;
};
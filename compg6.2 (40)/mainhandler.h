#pragma once

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "shadow_volume_technique.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "null_technique.h"

using namespace std;

#define WINDOW_WIDTH  1000  
#define WINDOW_HEIGHT 800

class mainhandler : public ICallbacks
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

    void RenderSceneIntoDepth();
    void RenderShadowVolIntoStencil();
    void RenderShadowedScene();
    void RenderAmbientLight();
    void CalcFPS();
    void RenderFPS();

    ShadowVolumeTechnique m_ShadowVolTech;
    LightingTechnique m_LightingTech;
    NullTechnique m_nullTech;
    Camera* m_pGameCamera;
    float m_scale;
    PointLight m_pointLight;
    glm::vec3 m_boxPos;
    Mesh m_box;
    Mesh m_quad;
    Texture* m_pGroundTex;
    ProjInfo m_persProjInfo;
    int m_glutTime;
    long long m_startTime;
    int m_frameCount;
    float m_fps;
};
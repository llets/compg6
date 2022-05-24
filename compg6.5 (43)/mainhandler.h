#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "shadow_map_technique.h"
#include "shadow_map_fbo.h"
#include "structs.h"

using namespace std;

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

class mainhandler : public ICallbacks
{
public:

    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    void ShadowMapPass();
    void RenderPass();
    virtual void IdleCB();
    virtual void SpecialKeyboardCB(int Key, int x, int y);
    virtual void KeyboardCB(unsigned char Key, int x, int y);
    virtual void PassiveMouseCB(int x, int y);
    virtual void MouseCB(int Button, int State, int x, int y);

private:

    void CalcFPS();
    void RenderFPS();

    LightingTechnique m_lightingEffect;
    ShadowMapTechnique m_shadowMapEffect;
    Camera* m_pGameCamera;
    float m_scale;
    PointLight m_pointLight;
    Mesh m_mesh;
    Mesh m_quad;
    Orientation m_quad1Orientation;
    Orientation m_quad2Orientation;
    Orientation m_mesh1Orientation;
    Orientation m_mesh2Orientation;
    PersProjInfo m_persProjInfo;
    Texture* m_pGroundTex;
    ShadowMapFBO m_shadowMapFBO;
    int m_time;
    int m_frameCount;
    float m_fps;
};
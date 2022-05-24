#pragma once

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include "ogldev_engine_common.h"
#include "ogldev_app.h"
#include "ogldev_util.h"
#include "pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_texture.h"
#include "ogldev_glfw_backend.h"
#include "lighting_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_basic_mesh.h"
#include "shadow_map_technique.h"
#include "shadow_map_fbo.h"

using namespace std;

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

class mainhandler : public ICallbacks, public OgldevApp
{
public:

    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    void ShadowMapPass();
    void RenderPass();
    void KeyboardCB(OGLDEV_KEY OgldevKey);
    virtual void PassiveMouseCB(int x, int y);

private:

    LightingTechnique* m_pLightingEffect;
    ShadowMapTechnique* m_pShadowMapEffect;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    BasicMesh m_mesh;
    BasicMesh m_quad;
    PersProjInfo m_persProjInfo;
    Texture* m_pGroundTex;
    ShadowMapFBO m_shadowMapFBO;
};

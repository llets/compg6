#pragma once

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include "ogldev_engine_common.h"
#include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "camera.h"
#include "skinning_technique.h"
#include "motion_blur_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_skinned_mesh.h"
#include "intermediate_buffer.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class mainhandler : public ICallbacks, public OgldevApp
{
public:

    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    void RenderPass();
    void MotionBlurPass();
    virtual void KeyboardCB(OGLDEV_KEY OgldevKey);
    virtual void PassiveMouseCB(int x, int y);


private:

    SkinningTechnique* m_pSkinningTech;
    MotionBlurTechnique* m_pMotionBlurTech;
    Camera* m_pGameCamera;
    DirectionalLight m_directionalLight;
    SkinnedMesh m_mesh;
    SkinnedMesh m_quad;
    Vector3f m_position;
    PersProjInfo m_persProjInfo;
    IntermediateBuffer m_intermediateBuffer;
    Pipeline m_pipeline;
    vector<Matrix4f> m_prevTransforms;
};
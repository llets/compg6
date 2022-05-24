#include "mainhandler.h"

struct CameraDirection
{
    GLenum CubemapFace;
    glm::vec3 Target;
    glm::vec3 Up;
};

CameraDirection gCameraDirections[NUM_OF_LAYERS] =
{
    { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

    mainhandler::mainhandler()
    {
        m_pGameCamera = NULL;
        m_scale = 0.0f;
        m_pointLight.AmbientIntensity = 0.1f;
        m_pointLight.DiffuseIntensity = 0.9f;
        m_pointLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
        m_pointLight.Attenuation.Linear = 0.0f;
        m_pointLight.Position = glm::vec3(0.0, 5.0, 0.0f);

        m_persProjInfo.FOV = 90.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;

        m_frameCount = 0;
        m_fps = 0.0f;
    }

    mainhandler::~mainhandler()
    {
        SAFE_DELETE(m_pGameCamera);
    }

    bool mainhandler::Init()
    {
        glm::vec3 Pos(0.0f, 8.0f, -10.0f);
        glm::vec3 Target(0.0f, -0.5f, 1.0f);
        glm::vec3 Up(0.0, 1.0f, 0.0f);

        if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            printf("Error initializing the shadow map FBO\n");
            return false;
        }

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        if (!m_lightingEffect.Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_lightingEffect.Enable();

        m_lightingEffect.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
        m_lightingEffect.SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);
        m_lightingEffect.SetPointLight(m_pointLight);

        if (!m_shadowMapEffect.Init()) {
            printf("Error initializing the shadow map technique\n");
            return false;
        }

        m_shadowMapEffect.Enable();
        m_shadowMapEffect.SetLightWorldPos(m_pointLight.Position);

        if (!m_quad.LoadMesh("C:/src/test/quad.obj")) {
            return false;
        }

        // the orientation of the two quads
        m_quad1Orientation.m_scale = glm::vec3(10.0f, 10.0f, 10.0f);
        m_quad1Orientation.m_rotation = glm::vec3(90.0f, 0.0f, 0.0f);

        m_quad2Orientation.m_scale = glm::vec3(10.0f, 10.0f, 10.0f);
        m_quad2Orientation.m_pos = glm::vec3(0.0f, 0.0f, 7.0f);

        m_pGroundTex = new Texture(GL_TEXTURE_2D, "C:/src/test/test.png");

        if (!m_pGroundTex->Load()) {
            return false;
        }

        if (!m_mesh.LoadMesh("C:/src/test/sphere.obj")) {
            return false;
        }

        m_mesh1Orientation.m_pos = glm::vec3(0.0f, 3.0f, 0.0f);
        m_mesh2Orientation.m_pos = glm::vec3(0.0f, 5.0f, 3.0f);

        m_time = glutGet(GLUT_ELAPSED_TIME);

        glEnable(GL_TEXTURE_CUBE_MAP);

        return true;
    }

    void mainhandler::Run()
    {
        GLUTBackendRun(this);
    }

    void mainhandler::RenderSceneCB()
    {
        CalcFPS();
        m_scale += 0.05f;
        m_pGameCamera->OnRender();
        ShadowMapPass();
        RenderPass();
        RenderFPS();
        glutSwapBuffers();
    }

    void mainhandler::ShadowMapPass()
    {
        glCullFace(GL_FRONT);

        m_shadowMapEffect.Enable();

        PersProjInfo ProjInfo;
        ProjInfo.FOV = 90.0f;
        ProjInfo.Height = WINDOW_HEIGHT;
        ProjInfo.Width = WINDOW_WIDTH;
        ProjInfo.zNear = 1.0f;
        ProjInfo.zFar = 100.0f;

        Pipeline p;
        p.SetPerspectiveProj(m_persProjInfo);

        glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

        for (uint i = 0; i < NUM_OF_LAYERS; i++) {
            m_shadowMapFBO.BindForWriting(gCameraDirections[i].CubemapFace);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            p.SetCamera(m_pointLight.Position, gCameraDirections[i].Target, gCameraDirections[i].Up);

            p.Orient(m_mesh1Orientation);
            m_shadowMapEffect.SetWorld(p.GetWorldTrans());
            m_shadowMapEffect.SetWVP(p.GetWVPTrans());
            m_mesh.Render();

            p.Orient(m_mesh2Orientation);
            m_shadowMapEffect.SetWorld(p.GetWorldTrans());
            m_shadowMapEffect.SetWVP(p.GetWVPTrans());
            m_mesh.Render();
        }
    }

    void mainhandler::RenderPass()
    {
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_lightingEffect.Enable();
        m_shadowMapFBO.BindForReading(SHADOW_TEXTURE_UNIT);
        m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());

        Pipeline p;
        p.SetPerspectiveProj(m_persProjInfo);
        p.SetCamera(*m_pGameCamera);

        // Render the quads
        m_pGroundTex->Bind(COLOR_TEXTURE_UNIT);
        p.Orient(m_quad1Orientation);
        m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
        m_lightingEffect.SetWVP(p.GetWVPTrans());
        m_quad.Render();

        p.Orient(m_quad2Orientation);
        m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
        m_lightingEffect.SetWVP(p.GetWVPTrans());
        m_quad.Render();

        // Render the meshes
        p.Orient(m_mesh1Orientation);
        m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
        m_lightingEffect.SetWVP(p.GetWVPTrans());
        m_mesh.Render();

        p.Orient(m_mesh2Orientation);
        m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
        m_lightingEffect.SetWVP(p.GetWVPTrans());
        m_mesh.Render();
    }

    void mainhandler::IdleCB()
    {
        RenderSceneCB();
    }

    void mainhandler::SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }

    void mainhandler::KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;
        }
    }

    void mainhandler::PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

    void mainhandler::MouseCB(int Button, int State, int x, int y)
    {
    }

    void mainhandler::CalcFPS()
    {
        m_frameCount++;

        int time = glutGet(GLUT_ELAPSED_TIME);

        if (time - m_time > 1000) {
            m_fps = (float)m_frameCount * 1000.0f / (time - m_time);
            m_time = time;
            m_frameCount = 0;
        }
    }

    void mainhandler::RenderFPS()
    {
        char text[32];
        ZERO_MEM(text);
        SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
    }
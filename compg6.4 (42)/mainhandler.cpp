#include "mainhandler.h"

enum OGLDEV_BACKEND_TYPE {
	OGLDEV_BACKEND_TYPE_GLUT,
	OGLDEV_BACKEND_TYPE_GLFW
};

static OGLDEV_BACKEND_TYPE sBackendType = OGLDEV_BACKEND_TYPE_GLUT;

void OgldevBackendLeaveMainLoop()
{
	switch (sBackendType) {
	case OGLDEV_BACKEND_TYPE_GLUT:
		GLUTBackendLeaveMainLoop();
		break;
	case OGLDEV_BACKEND_TYPE_GLFW:
		GLFWBackendLeaveMainLoop();
		break;
	default:
		assert(0);
	}
}

mainhandler::mainhandler()
{
	m_pGameCamera = NULL;
	m_pLightingEffect = NULL;
	m_pShadowMapEffect = NULL;
	m_scale = 0.0f;
	m_spotLight.AmbientIntensity = 0.1f;
	m_spotLight.DiffuseIntensity = 0.9f;
	m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight.Attenuation.Linear = 0.01f;
	m_spotLight.Position = glm::vec3(-20.0, 20.0, 1.0f);
	m_spotLight.Direction = glm::vec3(1.0f, -1.0f, 0.0f);
	m_spotLight.Cutoff = 20.0f;

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;
}

mainhandler:: ~mainhandler()
{
	SAFE_DELETE(m_pLightingEffect);
	SAFE_DELETE(m_pShadowMapEffect);
	SAFE_DELETE(m_pGameCamera);
}

bool mainhandler::Init()
{
	glm::vec3 Pos(3.0f, 8.0f, -10.0f);
	glm::vec3 Target(0.0f, -0.5f, 1.0f);
	glm::vec3 Up(0.0, 1.0f, 0.0f);

	if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		return false;
	}

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	m_pLightingEffect = new LightingTechnique();

	if (!m_pLightingEffect->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pLightingEffect->Enable();

	m_pLightingEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_pLightingEffect->SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);
	m_pLightingEffect->SetSpotLights(1, &m_spotLight);
	m_pLightingEffect->SetShadowMapSize((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);

	m_pShadowMapEffect = new ShadowMapTechnique();

	if (!m_pShadowMapEffect->Init()) {
		printf("Error initializing the shadow map technique\n");
		return false;
	}

	if (!m_quad.LoadMesh("C:/src/test/quad.obj")) {
		return false;
	}

	m_pGroundTex = new Texture(GL_TEXTURE_2D, "C:/src/test/test.png");

	if (!m_pGroundTex->Load()) {
		return false;
	}

	if (!m_mesh.LoadMesh("C:/src/test/phoenix_ugv.md2")) {
		return false;
	}

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

	m_shadowMapFBO.BindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);

	m_pShadowMapEffect->Enable();

	Pipeline p;
	p.Scale(0.1f, 0.1f, 0.1f);
	p.Rotate(0.0f, m_scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 3.0f);
	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	p.SetPerspectiveProj(m_persProjInfo);
	m_pShadowMapEffect->SetWVP(p.GetWVPTrans());
	m_mesh.Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mainhandler::RenderPass()
{
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pLightingEffect->Enable();

	m_shadowMapFBO.BindForReading(GL_TEXTURE1);

	Pipeline p;
	p.SetPerspectiveProj(m_persProjInfo);
	p.Scale(10.0f, 10.0f, 10.0f);
	p.WorldPos(0.0f, 0.0f, 1.0f);
	p.Rotate(90.0f, 0.0f, 0.0f);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

	m_pLightingEffect->SetWVP(p.GetWVPTrans());
	m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	m_pLightingEffect->SetLightWVP(p.GetWVPTrans());
	m_pLightingEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
	m_pGroundTex->Bind(GL_TEXTURE0);
	m_quad.Render();

	p.Scale(0.1f, 0.1f, 0.1f);
	p.Rotate(0.0f, m_scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 3.0f);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	m_pLightingEffect->SetWVP(p.GetWVPTrans());
	m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	m_pLightingEffect->SetLightWVP(p.GetWVPTrans());

	m_mesh.Render();
}

void mainhandler::KeyboardCB(OGLDEV_KEY OgldevKey)
{
	switch (OgldevKey) {
	case OGLDEV_KEY_ESCAPE:
	case OGLDEV_KEY_q:
		OgldevBackendLeaveMainLoop();
		break;
	default:
		m_pGameCamera->OnKeyboard(OgldevKey);
	}
}

void mainhandler::PassiveMouseCB(int x, int y)
{
	m_pGameCamera->OnMouse(x, y);
}
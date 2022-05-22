#include "mainhandler.h"

#include "windows.h"
#include <winsock.h>
#include <ctime>

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);
	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;
	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;
	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}

static long long GetCurrentTimeMillis()
{
	timeval t;
	gettimeofday(&t, NULL);
	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
}


mainhandler::mainhandler()
{
	m_pGameCamera = nullptr;
	m_scale = 0.0f;
	m_pointLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_pointLight.Position = glm::vec3(0.0f, 15.0f, 0.0f);

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_frameCount = 0;
	m_fps = 0.0f;

	m_boxPos = glm::vec3(0.0f, 2.0f, 0.0);
}

mainhandler::~mainhandler()
{
	delete (m_pGameCamera);
}

bool mainhandler::Init()
{
	glm::vec3 Pos(0.0f, 2.0f, -7.0f);
	glm::vec3 Target(0.0f, 0.0f, 1.0f);
	glm::vec3 Up(0.0, 1.0f, 0.0f);

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	if (!m_nullTech.Init()) {
		printf("Error initializing the null technique\n");
		return false;
	}

	if (!m_ShadowVolTech.Init()) {
		printf("Error initializing the shadow volume technique\n");
		return false;
	}

	if (!m_LightingTech.Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_LightingTech.Enable();

	m_LightingTech.SetColorTextureUnit(0);
	m_LightingTech.SetPointLights(1, &m_pointLight);
	m_LightingTech.SetMatSpecularIntensity(0.0f);
	m_LightingTech.SetMatSpecularPower(0);

	if (!m_box.LoadMesh("C:/src/test/box.obj", true)) {
		printf("Mesh load failed\n");
		return false;
	}

	m_glutTime = glutGet(GLUT_ELAPSED_TIME);
	m_startTime = GetCurrentTimeMillis();

	if (!m_quad.LoadMesh("C:/src/test/quad.obj", false)) {
		return false;
	}

	m_pGroundTex = new Texture(GL_TEXTURE_2D, "C:/src/test/test.png");

	if (!m_pGroundTex->Load()) {
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
	m_scale += 0.1f;
	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	RenderSceneIntoDepth();
	glEnable(GL_STENCIL_TEST);
	RenderShadowVolIntoStencil();
	RenderShadowedScene();
	glDisable(GL_STENCIL_TEST);
	RenderAmbientLight();
	RenderFPS();
	glutSwapBuffers();
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
void mainhandler::RenderSceneIntoDepth()
{
	glDrawBuffer(GL_NONE);
	glDepthMask(GL_TRUE);

	m_nullTech.Enable();

	Pipeline p;

	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	p.WorldPos(m_boxPos);
	p.Rotate(0, m_scale, 0);
	m_nullTech.SetWVP(p.GetWVPTrans());
	m_box.Render();

	p.Scale(10.0f, 10.0f, 10.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.Rotate(90.0f, 0.0f, 0.0f);
	m_nullTech.SetWVP(p.GetWVPTrans());
	m_quad.Render();
}

void mainhandler::RenderShadowVolIntoStencil()
{
	glDrawBuffer(GL_NONE);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0xff);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	m_ShadowVolTech.Enable();

	m_ShadowVolTech.SetLightPos(m_pointLight.Position);

	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.WorldPos(m_boxPos);
	p.Rotate(0, m_scale, 0);
	m_ShadowVolTech.SetWorldMatrix(p.GetWorldTrans());
	m_ShadowVolTech.SetVP(p.GetVPTrans());

	m_box.Render();

	glEnable(GL_CULL_FACE);
}

void mainhandler::RenderShadowedScene()
{
	glDrawBuffer(GL_BACK);

	// prevent update to the stencil buffer
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 0x0, 0xFF);

	m_LightingTech.Enable();

	m_pointLight.AmbientIntensity = 0.0f;
	m_pointLight.DiffuseIntensity = 0.8f;

	m_LightingTech.SetPointLights(1, &m_pointLight);

	Pipeline p;
	p.SetPerspectiveProj(m_persProjInfo);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

	p.WorldPos(m_boxPos);
	p.Rotate(0, m_scale, 0);
	m_LightingTech.SetWVP(p.GetWVPTrans());
	m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
	m_box.Render();

	p.Scale(10.0f, 10.0f, 10.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.Rotate(90.0f, 0.0f, 0.0f);
	m_LightingTech.SetWVP(p.GetWVPTrans());
	m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
	m_pGroundTex->Bind(GL_TEXTURE0);
	m_quad.Render();
}

void mainhandler::RenderAmbientLight()
{
	glDrawBuffer(GL_BACK);
	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	m_LightingTech.Enable();

	m_pointLight.AmbientIntensity = 0.2f;
	m_pointLight.DiffuseIntensity = 0.0f;

	m_LightingTech.SetPointLights(1, &m_pointLight);

	m_pGroundTex->Bind(GL_TEXTURE0);

	Pipeline p;
	p.SetPerspectiveProj(m_persProjInfo);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

	p.WorldPos(m_boxPos);
	p.Rotate(0, m_scale, 0);
	m_LightingTech.SetWVP(p.GetWVPTrans());
	m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
	m_box.Render();

	p.Scale(10.0f, 10.0f, 10.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.Rotate(90.0f, 0.0f, 0.0f);
	m_LightingTech.SetWVP(p.GetWVPTrans());
	m_LightingTech.SetWorldMatrix(p.GetWorldTrans());

	m_quad.Render();

	glDisable(GL_BLEND);
}

void mainhandler::CalcFPS()
{
	m_frameCount++;

	int time = glutGet(GLUT_ELAPSED_TIME);

	if (time - m_glutTime > 1000) {
		m_fps = (float)m_frameCount * 1000.0f / (time - m_glutTime);
		m_glutTime = time;
		m_frameCount = 0;
	}
}

void mainhandler::RenderFPS()
{
	char text[32];
	memset(text, 0, sizeof(text));
	snprintf(text, sizeof(text), "FPS: %.2f", m_fps);
}
#include "mainhandler.h"
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
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.55f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);

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

	if (!m_silhouetteTech.Init()) {
		printf("Error initializing the silhouette technique\n");
		return false;
	}

	if (!m_LightingTech.Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_LightingTech.Enable();

	m_LightingTech.SetColorTextureUnit(0);
	m_LightingTech.SetDirectionalLight(m_directionalLight);
	m_LightingTech.SetMatSpecularIntensity(0.0f);
	m_LightingTech.SetMatSpecularPower(0);

	if (!m_mesh.LoadMesh("C:/src/test/box.obj", true)) {
		printf("Mesh load failed\n");
		return false;
	}
	m_glutTime = glutGet(GLUT_ELAPSED_TIME);
	m_startTime = GetCurrentTimeMillis();

	return true;
}

void mainhandler::Run()
{
	GLUTBackendRun(this);
}


void mainhandler::RenderSceneCB()
{
	CalcFPS();

	m_scale += 0.01f;

	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderScene();

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

void mainhandler::RenderScene()
{
	// Render the object as-is
	m_LightingTech.Enable();

	Pipeline p;
	p.SetPerspectiveProj(m_persProjInfo);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.WorldPos(m_boxPos);
	m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
	m_LightingTech.SetWVP(p.GetWVPTrans());
	m_mesh.Render();

	// Render the object's silhouette
	m_silhouetteTech.Enable();

	m_silhouetteTech.SetWorldMatrix(p.GetWorldTrans());
	m_silhouetteTech.SetWVP(p.GetWVPTrans());
	m_silhouetteTech.SetLightPos(glm::vec3(0.0f, 10.0f, 0.0f));

	glLineWidth(5.0f);

	m_mesh.Render();
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
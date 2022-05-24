#include "pipeline.h"

glm::mat4 multip(const glm::mat4& m1, const glm::mat4& m2) {
	glm::mat4 Ret;

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			Ret[i][j] = m1[i][0] * m2[0][j] +
				m1[i][1] * m2[1][j] +
				m1[i][2] * m2[2][j] +
				m1[i][3] * m2[3][j];
		}
	}
	return Ret;
}

glm::mat4 InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	glm::mat4 m;
	m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
	m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
	m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
	return m;
}

glm::mat4 InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	glm::mat4 rx, ry, rz;

	const float x = glm::radians(RotateX);
	const float y = glm::radians(RotateY);
	const float z = glm::radians(RotateZ);

	rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
	rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
	rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
	rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

	ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
	ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
	ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
	ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

	rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
	rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
	rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
	rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

	return rx * ry * rz;
	//return multip(multip(rz, ry), rx);
}

glm::mat4 InitTranslationTransform(float x, float y, float z)
{
	glm::mat4 m;
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return m;
}

glm::mat4 InitCameraTransform(const glm::vec3& Target, const glm::vec3& Up)
{
	glm::vec3 N = Target;
	glm::normalize(N);
	glm::vec3 U = Up;
	glm::normalize(U);
	U = glm::cross(U, N);
	glm::vec3 V = glm::cross(N, U);
	glm::mat4 m;
	m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
	m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
	m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
	return m;
}

glm::mat4 InitPersProjTransform(const PersProjInfo& p)
{
	const float ar = p.Width / p.Height;
	const float zRange = p.zNear - p.zFar;
	const float tanHalfFOV = tanf(glm::radians(p.FOV / 2.0f));
	glm::mat4 m;
	m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;            m[0][3] = 0.0;
	m[1][0] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;            m[1][3] = 0.0;
	m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-p.zNear - p.zFar) / zRange; m[2][3] = 2.0f * p.zFar * p.zNear / zRange;
	m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;            m[3][3] = 0.0;
	return m;
}

glm::mat4 InitOrthoProjTransform(const OrthoProjInfo& p)
{
	float l = p.l;
	float r = p.r;
	float b = p.b;
	float t = p.t;
	float n = p.n;
	float f = p.f;
	glm::mat4 m;
	m[0][0] = 2.0f / (r - l); m[0][1] = 0.0f;         m[0][2] = 0.0f;         m[0][3] = -(r + l) / (r - l);
	m[1][0] = 0.0f;         m[1][1] = 2.0f / (t - b); m[1][2] = 0.0f;         m[1][3] = -(t + b) / (t - b);
	m[2][0] = 0.0f;         m[2][1] = 0.0f;         m[2][2] = 2.0f / (f - n); m[2][3] = -(f + n) / (f - n);
	m[3][0] = 0.0f;         m[3][1] = 0.0f;         m[3][2] = 0.0f;         m[3][3] = 1.0;
	return m;
}

const glm::mat4& Pipeline::GetVPTrans()
{
	glm::mat4 CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

	CameraTranslationTrans = InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	CameraRotateTrans = InitCameraTransform(m_camera.Target, m_camera.Up);
	PersProjTrans = InitPersProjTransform(m_persProjInfo);

	m_VPTtransformation = CameraTranslationTrans * CameraRotateTrans * PersProjTrans;
	return m_VPTtransformation;
}

const glm::mat4& Pipeline::GetWorldTrans()
{
	glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;

	ScaleTrans = InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	RotateTrans = InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	TranslationTrans = InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

	m_WorldTransformation = ScaleTrans * RotateTrans * TranslationTrans;
	return m_WorldTransformation;
}

const glm::mat4& Pipeline::GetWVPTrans()
{
	GetWorldTrans();
	GetVPTrans();

	m_WVPtransformation = m_WorldTransformation * m_VPTtransformation;
	return m_WVPtransformation;
}

const glm::mat4& Pipeline::GetWVTrans()
{
	GetWorldTrans();

	glm::mat4 CameraTranslationTrans, CameraRotateTrans;

	CameraTranslationTrans = InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	CameraRotateTrans = InitCameraTransform(m_camera.Target, m_camera.Up);

	m_WVtransformation = m_WorldTransformation * CameraTranslationTrans * CameraRotateTrans;
	return m_WVtransformation;
}
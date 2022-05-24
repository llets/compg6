#include "shadow_map_technique.h"
#include "pipeline.h"

ShadowMapTechnique::ShadowMapTechnique()
{
}

bool ShadowMapTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "C:/src/test/shaders/shadow_map43.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "C:/src/test/shaders/shadow_map43.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_lightWorldPosLoc = GetUniformLocation("gLightWorldPos");

    if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
	m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
        m_lightWorldPosLoc == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

void ShadowMapTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)&WVP);
}

void ShadowMapTechnique::SetWorld(const glm::mat4& World)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)&World);
}

void ShadowMapTechnique::SetLightWorldPos(const glm::vec3& Pos)
{
    glUniform3f(m_lightWorldPosLoc, Pos.x, Pos.y, Pos.z);    
}
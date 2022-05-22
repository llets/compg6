#include <string>

#include "shadow_volume_technique.h"
#include "glfx.h"

using namespace std;

static const char* pEffectFile = "C:/src/test/shaders/shadow_volume40.glsl";

ShadowVolumeTechnique::ShadowVolumeTechnique() : Technique(pEffectFile)
{   
}


bool ShadowVolumeTechnique::Init()
{
    if (!CompileProgram("ShadowVolume")) {
        return false;
    }
    
    m_VPLocation = GetUniformLocation("gVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_lightPosLocation = GetUniformLocation("gLightPos");

    if (m_VPLocation == INVALID_UNIFORM_LOCATION ||
        m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
        m_lightPosLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }
            
    return true;
}

void ShadowVolumeTechnique::SetVP(const glm::mat4& VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)&VP);    
}

void ShadowVolumeTechnique::SetWorldMatrix(const glm::mat4& WorldInverse)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)&WorldInverse);
}

void ShadowVolumeTechnique::SetLightPos(const glm::vec3& Pos)
{
    glUniform3f(m_lightPosLocation, Pos.x, Pos.y, Pos.z);
}
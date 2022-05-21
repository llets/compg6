#include "silhouette_technique.h"
#include "glfx.h"

static const char* pEffectFile = "C:/src/test/shaders/silhouette39.glsl";

SilhouetteTechnique::SilhouetteTechnique() : Technique(pEffectFile)
{   
}

bool SilhouetteTechnique::Init()
{
    if (!CompileProgram("Silhouette")) {
        return false;
    }
    
    m_WVPLocation = GetUniformLocation("gWVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_lightPosLocation = GetUniformLocation("gLightPos");

    return true;
}

void SilhouetteTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)&WVP);    
}

void SilhouetteTechnique::SetWorldMatrix(const glm::mat4& WorldInverse)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)&WorldInverse);
}

void SilhouetteTechnique::SetLightPos(const glm::vec3& Pos)
{
    glUniform3f(m_lightPosLocation, Pos.x, Pos.y, Pos.z);
}
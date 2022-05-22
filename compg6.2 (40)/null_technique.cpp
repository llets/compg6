#include <string.h>

#include "null_technique.h"

static const char* pEffectFile = "C:/src/test/shaders/null_technique40.glsl";

NullTechnique::NullTechnique() : Technique(pEffectFile)
{   
}

bool NullTechnique::Init()
{
    if (!CompileProgram("NullTechnique")) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void NullTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)&WVP);    
}
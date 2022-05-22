#ifndef SHADOW_VOLUME_TECHNIQUE_H
#define	SHADOW_VOLUME_TECHNIQUE_H

#include "technique.h"
#include <glm/glm.hpp>

class ShadowVolumeTechnique : public Technique {
public:

    ShadowVolumeTechnique();

    virtual bool Init();

    void SetVP(const glm::mat4& VP);
    void SetWorldMatrix(const glm::mat4& World);
    void SetLightPos(const glm::vec3& Pos);
    
private:
    
    GLuint m_VPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_lightPosLocation;
};
#endif
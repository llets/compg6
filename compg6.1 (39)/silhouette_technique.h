#ifndef SILHOUETTE_TECHNIQUE_H
#define	SILHOUETTE_TECHNIQUE_H

#include "technique.h"
#include <glm/glm.hpp>

class SilhouetteTechnique : public Technique {
public:

    SilhouetteTechnique();
    virtual bool Init();
    void SetWVP(const glm::mat4& WVP);
    void SetWorldMatrix(const glm::mat4& WVP);
    void SetLightPos(const glm::vec3& Pos);

private:
    
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_lightPosLocation;
};
#endif
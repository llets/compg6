#ifndef SHADOW_MAP_TECHNIQUE_H
#define	SHADOW_MAP_TECHNIQUE_H

#include <glm/glm.hpp>
#include "technique.h"
#include "mesh.h"
#include "camera.h"

#define NUM_OF_LAYERS 6

class ShadowMapTechnique : public Technique {

public:

    ShadowMapTechnique();
    virtual bool Init();
    void SetWVP(const glm::mat4& WVP);	
    void SetWorld(const glm::mat4& World);
    void SetLightWorldPos(const glm::vec3& Pos);
    
private:

    GLint m_WVPLocation;
    GLint m_WorldMatrixLocation;
    GLint m_lightWorldPosLoc;
};
#endif
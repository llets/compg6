#ifndef NULL_TECHNIQUE_H
#define	NULL_TECHNIQUE_H

#include "technique.h"
#include <glm/glm.hpp>

class NullTechnique : public Technique {
public:

    NullTechnique();
    virtual bool Init();
    void SetWVP(const glm::mat4& WVP);

private:

    GLuint m_WVPLocation;
};
#endif
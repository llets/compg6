#ifndef SHADOW_MAP_TECHNIQUE_H
#define	SHADOW_MAP_TECHNIQUE_H

#include "technique.h"
#include <glm/glm.hpp>

class ShadowMapTechnique : public Technique {

public:

	ShadowMapTechnique();
	virtual bool Init();
	void SetWVP(const glm::mat4& WVP);
	void SetTextureUnit(unsigned int TextureUnit);
private:
#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF
	GLuint m_WVPLocation;
	GLuint m_textureLocation;
};
#endif
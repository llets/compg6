#ifndef CAMERA_H
#define	CAMERA_H

#include <glm/glm.hpp>
#include "ogldev_keys.h"
//#include "ogldev_atb.h"

class Camera
{
public:

    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up);
    bool OnKeyboard(OGLDEV_KEY Key);
    void OnMouse(int x, int y);
    void OnRender();

    const glm::vec3& GetPos() const
    {
        return m_pos;
    }

    const glm::vec3& GetTarget() const
    {
        return m_target;
    }

    const glm::vec3& GetUp() const
    {
        return m_up;
    }

private:

    void Init();
    void Update();

    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::vec3 m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;
    float m_AngleV;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    glm::vec2 m_mousePos;
};

#endif
#pragma once
#include "../define.h"

struct Camera{
    public: 
    Camera(int shaderID, uint32 width, uint32 height, float fov, glm::vec3& position);
    Camera(const Camera& copyconst) = delete;
    ~Camera();
    void update(float32 delta, glm::vec3 pos, glm::vec3 up);
    glm::vec3 onMouseMoved(float xRel, float yRel, float mouseSensitivity);
    glm::mat4 getview();
    glm::mat4 getproj();
    glm::vec3 lookat();
    private:
    int modelViewProjmatrixloc, modelViewmatrixloc, invmodelViewmatrixloc;
    glm::vec3 position;
    glm::mat4 model ;
    glm::mat4 projektion ;
    glm::mat4 modelViewProj, invModelView;
    float yaw;
    float pitch;
    glm::mat4 view;
    glm::vec3 lookAt;
    
};
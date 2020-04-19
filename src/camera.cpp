#include "camera.h" 


Camera::Camera(int shaderID, uint32 width, uint32 height, float fov, glm::vec3& position){
    modelViewProjmatrixloc = GLCALL(glGetUniformLocation(shaderID, "u_modelViewProj"));
    modelViewmatrixloc = GLCALL(glGetUniformLocation(shaderID, "u_modelView"));
    invmodelViewmatrixloc = GLCALL(glGetUniformLocation(shaderID, "u_invModelView"));
    model = glm::mat4(1.0f);model = glm::scale(model, glm::vec3(0.5f));
    position = glm::vec3(0.0f, 0.0f, 5.0f);
    projektion = glm::perspective(glm::radians(fov / 2), (float)width / (float)height, 0.1f, 1000.0f);
    view = glm::translate(glm::mat4(1.0f), position);
    yaw = -90.0f;
    pitch = 0.0f;
    lookAt = onMouseMoved(0.0f, 0.0f, 0.0f);
}
Camera::~Camera(){
}
void Camera::update(float32 delta, glm::vec3 pos, glm::vec3 up){
    position = glm::vec3(pos);
    view = glm::lookAt(position, position + lookAt, up);
    modelViewProj = projektion * view * model;
    invModelView = glm::transpose(glm::transpose(view));
    GLCALL(glUniformMatrix4fv(modelViewProjmatrixloc, 1, GL_FALSE, &modelViewProj[0][0]));
    GLCALL(glUniformMatrix4fv(modelViewmatrixloc, 1, GL_FALSE, &view[0][0]));
    GLCALL(glUniformMatrix4fv(invmodelViewmatrixloc, 1, GL_FALSE, &invModelView[0][0]));
}
glm::vec3 Camera::onMouseMoved(float xRel, float yRel, float mouseSensitivity) {
        yaw += xRel * mouseSensitivity;
        pitch -= yRel * mouseSensitivity;
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        lookAt = glm::normalize(front);
        return glm::normalize(front);
    }
glm::mat4 Camera::getview(){
    return view;
}
glm::mat4 Camera::getproj(){
    return projektion;
}
glm::vec3 Camera::lookat(){
    return lookAt;
}
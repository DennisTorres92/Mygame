#include "player.h" 


Player::Player(){
}
Player::~Player(){
    saveplayer(); 
}
void Player::loadplayer(){
}
void Player::saveplayer(){ 
}
void Player::camera(int shaderID, uint32 width, uint32 height, float fov){
    modelViewProjmatrixloc = GLCALL(glGetUniformLocation(shaderID, "u_modelViewProj"));
    modelViewmatrixloc = GLCALL(glGetUniformLocation(shaderID, "u_ModelView"));
    invmodelViewmatrixloc = GLCALL(glGetUniformLocation(shaderID, "u_invModelView"));
    model = glm::mat4(1.0f);
    position = glm::vec3(0.0f, 0.0f,3.0f);
    projektion = glm::perspective(glm::radians(fov / 2), (float)width/(float)height, 0.1f, 1000.0f);
    view = glm::translate(glm::mat4(1.0f), position);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    onMouseMoved(0.0f, 0.0f);
}
void Player::cameraupdate(float32 delta){
    view = glm::lookAt(position, position + lookAt, up);
    modelViewProj = projektion * view * model;
    invModelView = glm::transpose(glm::transpose(view));
    GLCALL(glUniformMatrix4fv(modelViewProjmatrixloc, 1, GL_FALSE, &modelViewProj[0][0]));
    GLCALL(glUniformMatrix4fv(modelViewmatrixloc, 1, GL_FALSE, &view[0][0]));
    GLCALL(glUniformMatrix4fv(invmodelViewmatrixloc, 1, GL_FALSE, &invModelView[0][0]));
}
void Player::onMouseMoved(float xRel, float yRel) {
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
    }
void Player::createplayer(const char* name, Player::pclas pclas){
    Player::name = name;
    playerclass = pclas;
}
std::string Player::getclass(){
    if(playerclass == WARRIOR){
        return "Warrior";
    }else if(playerclass == MAGE){
        return "Mage";
    }else{
        return "ERROR";
    }
}
void Player::move(SDL_Event ev){
    if(ev.type == SDL_KEYDOWN ){
        switch(ev.key.keysym.sym){
            case SDLK_w:
                mvforward = true;
                break;
            case SDLK_s:
                mvbackward = true;
                break;
            case SDLK_e:
                mvleft = true;
                break;
            case SDLK_q:
                mvright = true;
                break;
            case SDLK_a:
                rotal = 1;
                break;
            case SDLK_d:
                rotar = 1;
                break;
            case SDLK_r:
                fly = true;
                break;
            case SDLK_t:
                fly = false;
                break;
            case SDLK_SPACE:
                if(position.y <= 0.0f && !bjump){
                    jump = 30;
                    bjump = true;
                }
                break;
            default:
                break;
        }
    }else if(ev.type == SDL_KEYUP ){
        switch(ev.key.keysym.sym){
            case SDLK_w:
                mvforward = false;
                break;
            case SDLK_s:
                mvbackward = false;
                break;
            case SDLK_e:
                mvleft = false;
                break;
            case SDLK_q:
                mvright = false;
                break;
            case SDLK_a:
                rotal = 0;
                break;
            case SDLK_d:
                rotar = 0;
                break;
            case SDLK_SPACE:
                    bjump = false;
                break;
            default:
                break;
        }
    }else if(ev.type == SDL_MOUSEBUTTONDOWN ) {
        if( ev.button.button == SDL_BUTTON_RIGHT)
            mouseright = true;
            SDL_SetRelativeMouseMode(SDL_TRUE);
    }else if(ev.type == SDL_MOUSEBUTTONUP ) {
        if( ev.button.button == SDL_BUTTON_RIGHT)
            mouseright = false;
            SDL_SetRelativeMouseMode(SDL_FALSE);
    }else if(ev.type == SDL_MOUSEMOTION && mouseright) {
            onMouseMoved(ev.motion.xrel, ev.motion.yrel);
    }
}
void Player::ismove(float32 delta){
    if(mvforward){
        if(fly){
            position += lookAt * (2.0f * delta);
        }else{
            position += glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)*lookAt) * (2.0f * delta);
        }
    }if(mvbackward){
        if(fly){
            position -= lookAt * (2.0f * delta);
        }else{
            position -= glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)*lookAt) * (2.0f * delta);
        }     
    }if(mvleft){         
        position += glm::normalize(glm::cross(lookAt, up)) * (2.0f * delta);
    }if(mvright){
        position -= glm::normalize(glm::cross(lookAt, up)) * (2.0f * delta);
    }if(rotal == 1){
        onMouseMoved(-2.0f/mouseSensitivity, 0);
    }if(rotar == 1){
        onMouseMoved(2.0f/mouseSensitivity, 0);
    }if(!fly && position.y > 0.0f && jump <= 0){
        position -= glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * (4.0f * delta);
    }if(position.y < 0.0f){
        position.y = 0.0f;
    }if(jump > 0){
        position += glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * (4.0f * delta);
        jump--;
    }
}
glm::mat4 Player::getview(){
    return view;
}
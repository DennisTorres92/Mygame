#include "player.h" 


Player::Player(int shaderid, uint32 width, uint32 height, float fov){
    loadplayer();
    camera = new Camera(shaderid, width, height, fov);
}
Player::~Player(){
    saveplayer(); 
    delete camera;
}
void Player::loadplayer(){
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}
void Player::saveplayer(){ 
}

void Player::update(float32 delta){
    camera->update(delta, position, up);
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
                if(position.y <= 0.0f && !bjump && !fly){
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
            camera->onMouseMoved(ev.motion.xrel, ev.motion.yrel, mouseSensitivity);
    }
}
void Player::ismove(float32 delta){
    if(mvforward){
        if(fly){
            position += camera->lookat() * (2.0f * delta);
        }else{
            position += glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)*camera->lookat()) * (2.0f * delta);
        }
    }if(mvbackward){
        if(fly){
            position -= camera->lookat() * (2.0f * delta);
        }else{
            position -= glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)*camera->lookat()) * (2.0f * delta);
        }     
    }if(mvleft){         
        position += glm::normalize(glm::cross(camera->lookat(), up)) * (2.0f * delta);
    }if(mvright){
        position -= glm::normalize(glm::cross(camera->lookat(), up)) * (2.0f * delta);
    }if(rotal == 1){
        camera->onMouseMoved(-2.0f, 0, mouseSensitivity);
    }if(rotar == 1){
        camera->onMouseMoved(2.0f, 0, mouseSensitivity);
    }if(!fly && position.y > 0.0f && jump <= 0){
        position -= glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * (4.0f * delta);
    }if(position.y < 0.0f){
        position.y = 0.0f;
    }if(jump > 0){
        position += glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * (4.0f * delta);
        jump--;
    }
}

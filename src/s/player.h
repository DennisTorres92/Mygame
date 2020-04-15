#pragma once
#include "../define.h"

struct Player{
    public: 
    enum pclas{WARRIOR, MAGE};
    Player();
    Player(const Player& copyconst) = delete;
    ~Player();
    void loadplayer();
    void saveplayer();
    void createplayer(const char* name, Player::pclas pclas); 
    void camera(int shaderID, uint32 width, uint32 height, float degrees);
    void cameraupdate(float32 delta);
    void move(SDL_Event ev);
    void ismove(float32 delta);
    void onMouseMoved(float xRel, float yRel);
    void getstats(int& lvl, int& live, int& dev, int& str, int& mag, std::string& name, int& pclass);
    std::string getclass();
    glm::mat4 getview();
    private:
    bool mvforward = false, mvbackward = false, mvleft = false, mvright = false, mouseright = false, fly = false, bjump = false;
    int modelViewProjmatrixloc, modelViewmatrixloc, invmodelViewmatrixloc;
    glm::vec3 position;
    glm::mat4 model ;
    glm::mat4 projektion ;
    glm::mat4 view ;
    glm::mat4 modelViewProj, invModelView;
    std::string name = "";
    int level = 1, live = 110, dev = 20, str = 14, mag = 12, rotal = 0, rotar = 0, jump = 0;
    Player::pclas playerclass;
    float yaw;
    float pitch;
    glm::vec3 lookAt;
    const float mouseSensitivity = 0.3f;
    glm::vec3 up;
};
#pragma once
#include "../define.h"
#include "camera.h"

struct Player{
    public: 
    enum pclas{WARRIOR, MAGE};
    Player(int shaderid, uint32 width, uint32 height, float fov);
    Player(const Player& copyconst) = delete;
    ~Player();
    void loadplayer();
    void saveplayer();
    void createplayer(const char* name, Player::pclas pclas); 
    void update(float32 delta);
    void move(SDL_Event ev);
    void ismove(float32 delta);
    void getstats(int& lvl, int& live, int& dev, int& str, int& mag, std::string& name, int& pclass);
    std::string getclass();
    Camera* camera;
    private:
    const float mouseSensitivity = 0.3f;
    bool mvforward = false, mvbackward = false, mvleft = false, mvright = false, mouseright = false, fly = false, bjump = false;
    glm::vec3 position;
    std::string name = "";
    int level = 1, live = 110, dev = 20, str = 14, mag = 12, rotal = 0, rotar = 0, jump = 0;
    Player::pclas playerclass;
    
    glm::vec3 up;
};
#include "src/menu.h"
#include "src/player.h"
#include "src/models.h"
#include "shaders/shader.h"

int main(int argc, char** argv){
    uint32 width, height, flags;
    ldcfg(width, height, flags);
    Window win("My Game", width, height, flags);
    Shader shader("shaders/basic.vs", "shaders/basic.fs");
    shader.bind();


    Model monkey;
    monkey.init("models/world.bmf", &shader);

    uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
    uint64 lastCounter = SDL_GetPerformanceCounter();
    float32 delta = 0.0f;

    Player player(shader.getShaderid(), width, height, 90.0f);
    
    //wireframe
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glEnable(GL_DEPTH_TEST));
    bool close = false;
    do{
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                close = true;
            }else if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        close = true;
                    break;
                }
            }
            player.move(event);
        }
        player.ismove(delta, shader.getShaderid());
        player.update(delta);
        monkey.render();
        //monkey.rotate(player.camera->getview(), player.camera->getproj(), delta, &shader);
        pointlight(&shader, player.camera->getview(), delta);
        sun(player.camera->getview(), glm::vec3(1.0f), &shader);
        
        win.swap();

        uint64 endCounter = SDL_GetPerformanceCounter();
        uint64 counterElapsed = endCounter - lastCounter;
        delta = (float32)counterElapsed / (float32)perfCounterFrequency;
        uint32 FPS = (uint32)((float32)perfCounterFrequency / (float32)counterElapsed);
        lastCounter = endCounter;
    }while(!close);
    return 0;
}



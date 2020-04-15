#pragma once
#include "../define.h"

struct Window{
    Window(const char*  name, uint32 width, uint32 height, uint32 flags);
    virtual ~Window();
    void swap();
private:
    SDL_Window* window;
    GLenum err;
    SDL_GLContext glcontext;
};

int mainmenu(int width, int height);
int ldcfg(uint32& WIDTH, uint32& HEIGHT, uint32& flags);
int chcfg(uint32& WIDTH, uint32& HEIGHT, uint32& flags);
#include "menu.h"
#include <fstream>
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum serverety, GLsizei lenght, const GLchar* message, const void* userparam){
    std::cout<<"[OPENGL ERROR]: "<<message<<"\n";
}
#ifndef NDEBUG
    void GLGetError(const char* file, int line, const char* call){
        while(GLenum error = glGetError()){
            std::cout<<"[OPENGL ERROR]: "<<glewGetErrorString(error)<<" in "<<file<<":"<<line<<" Call: "<<call<<"\n";
        }  
    }
    #define GLCALL(call) call; GLGetError(__FILE__, __LINE__, #call)
#else
    #define GLCALL(call) call
#endif  /*  NDEBUG   */

Window::Window(const char*  name, uint32 width, uint32 height, uint32 flags){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    #ifndef NDEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,height, SDL_WINDOW_OPENGL);
    #else  
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    #endif /*  NDEBUG  */
    GLCALL(glcontext = SDL_GL_CreateContext(window));
    err = glewInit();
    if (err != GLEW_OK){
        std::cout<<"ERROR: "<<glewGetErrorString(err)<<"\n";
        getchar();
    }
    #ifndef NDEBUG 
        std::cout<<"GLVersion: "<<glGetString(GL_VERSION)<<"\n";
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openGLDebugCallback, 0);
    #endif /*NDEBUG */
}
void Window::swap(){
    SDL_GL_SwapWindow(window);
}
Window::~Window(){
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int ldcfg(uint32& WIDTH, uint32& HEIGHT, uint32& flags){
    std::ifstream input = std::ifstream("config", std::ios::in | std::ios::binary);
    if(!input.is_open()){
        std::ofstream output("config", std::ios::out | std::ios::binary);
        if(!output.is_open()){
            std::cout<<"Cant create config file\n";
            return 1;
        }
        WIDTH = 900;
        HEIGHT = 600;
        flags = SDL_WINDOW_OPENGL;
        output.write((char*)&WIDTH,sizeof(uint32));
        output.write((char*)&HEIGHT,sizeof(uint32));
        output.write((char*)&flags,sizeof(uint32));
        output.close();
    }else{
        input.read((char*)&WIDTH,sizeof(uint32));
        input.read((char*)&HEIGHT,sizeof(uint32));
        input.read((char*)&flags,sizeof(uint32));
        input.close();
    }
    return 0;
}
int chcfg(uint32& WIDTH, uint32& HEIGHT, uint32& flags){
    std::ofstream output("config", std::ios::out | std::ios::binary);
    if(!output.is_open()){
        std::cout<<"Cant change config file\n";
        return 1;
    }
    output.write((char*)&WIDTH,sizeof(uint32));
    output.write((char*)&HEIGHT,sizeof(uint32));
    output.write((char*)&flags,sizeof(uint32));
    output.close();
    return 0;
}

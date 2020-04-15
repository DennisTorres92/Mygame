#pragma once
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#ifdef _WIN32
    #include <SDL.h>
    #pragma comment(lib, "SDL2.lib")
    #pragma comment(lib, "glew32s.lib")
    #pragma comment(lib, "opengl32.lib")
#else
    #include <SDL2/SDL.h>
#endif /* _WIN32 */

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;


//Debuglog define in menu.cpp
void ogldebugerror(GLenum source, GLenum type, GLuint id,GLenum serverety, GLsizei lenght,const GLchar* msg, const void* userparam);
#ifdef _DEBUG
    void GLGetError(const char* file, int line, const char* call);
    #define GLCALL(call) call; GLGetError(__FILE__, __LINE__, #call)
#else
    #define GLCALL(call) call
#endif  /*  _DEBUG   */


#ifndef SHADER_H
#define SHADER_H

#include "../define.h"

struct Shader{
    Shader(const char* vertexShaderFilename,const char* fragmentShaderFilename);
    virtual ~Shader();
    void bind();
    void unbind();
    int getShaderid();
private:
    GLuint compile(std::string shadersource, GLenum type);
    std::string parse(const char* filename);
    GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    GLuint shaderid;
};
#endif /*  SHADER_H  */
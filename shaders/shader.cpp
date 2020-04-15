#include "shader.h"


Shader::Shader(const char* vertexShaderFilename,const char* fragmentShaderFilename){
    shaderid = createShader(vertexShaderFilename, fragmentShaderFilename);
}
Shader::~Shader(){
    glDeleteProgram(shaderid);
}
void Shader::bind(){ 
    glUseProgram(shaderid);
}
void Shader::unbind(){
    glUseProgram(0);
}
GLuint Shader::compile(std::string shadersource, GLenum type){
    GLuint id = glCreateShader(type);
    const char* src = shadersource.c_str();
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE){
        int lenght = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* msg = new char[lenght];
        glGetShaderInfoLog(id, lenght, &lenght, msg);
        std::cout<<"Shader Compile Error "<<msg<<"\n";
        delete[] msg;
        return 0;
    }
    return id;
}
std::string Shader::parse(const char* filename){
    FILE* file;
    file = fopen(filename, "rb");
    if(file == nullptr){
        std::cout<<"ERROR while File "<<filename<<" not found \n";
        return 0;
    }
    std::string contents;
    fseek(file, 0, SEEK_END);
    std::size_t fileLength = ftell(file);
    rewind(file);
    contents.resize(fileLength);
    fread(&contents[0], 1, fileLength, file);
    fclose(file);
    return contents;
}
GLuint Shader::createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename){
    std::string vertexShaderSource = parse(vertexShaderFilename);
    std::string fragmentShaderSource = parse(fragmentShaderFilename);
    GLuint program = glCreateProgram();
    GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    #ifdef _RELEASE
        glDetachShader(program, vs);
        glDetachShader(program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
    #endif  /*_RELEASE  */
    return program;
} 
int Shader::getShaderid(){
    return shaderid;
}
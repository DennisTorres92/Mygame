#pragma once
#include "../define.h"
#include "../libs/stb_image.h"

struct Vertex{
    glm::vec3 positions;
    glm::vec3 normals;
    glm::vec2 texture;
};
struct Material{
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
    GLuint diffuseMap;
    GLuint normalMap;
};

struct IndexBuffer{
    IndexBuffer(void* data, uint32 numIndexies, uint32 size);
    virtual ~IndexBuffer();
    void bind();
    void unbind();
private:
    uint32 numvertices;
    GLuint bufferid;
};

struct VertexBuffer{
    VertexBuffer(void* data, uint32 numverticies);
    virtual ~VertexBuffer();
    void bind();
    void unbind();
private:
    uint32 numvertices;
    GLuint bufferid;
    GLuint vio;
};

struct TextureBuffer{
public:
    TextureBuffer(const char* filediffuse, GLuint& diffuseMap, const char* filenormal, GLuint& normalMap);
    ~TextureBuffer();
    void bind();
    void unbind();
private:
    int32 textureWidth = 0;
    int32 textureHeight = 0;
    int32 bitsPerPixel = 0;
	GLuint idDiffuse, idNormal;
};  


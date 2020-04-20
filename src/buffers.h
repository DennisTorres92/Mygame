#ifndef BUFFERS_H
#define BUFFERS_H

#include "../define.h"
#include "../libs/stb_image.h"

struct Vertex{
    glm::vec3 positions;
    glm::vec3 normals;
    glm::vec3 tangent;
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
    void binddif();
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
    void binddif();
    void bindnorm();
    void unbind();
private:
    int32 textureWidth = 0;
    int32 textureHeight = 0;
    int32 bitsPerPixel = 0;
	GLuint idDiffuse, idNormal;
};  

#endif /*  BUFFERS_H  */
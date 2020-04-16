#pragma once
#include <vector>
#include <fstream>
#include "../shaders/shader.h"

#include "buffers.h"

struct Mesh{
    public:
        Mesh(std::vector<Vertex>& vertices, uint64 numVertices, std::vector<uint32>& indices, Material material, uint64 numindexies, Shader* shader);
        ~Mesh();
    void render();
    private:
        VertexBuffer* vertexbuffer;
        IndexBuffer* indexbuffer;
        TextureBuffer* texturebuffer;
        Shader* shader;
        Material material;
        uint64 numindexies;
        int diffuselocation;
        int specularlocation;
        int emissivelocation;
        int shininesslocation;
};
class Model{
    public:
        ~Model();
        void init(const char* filename, Shader* shader);
        void render();
        void rotate(glm::mat4 view,glm::mat4 projection, float delta, Shader* shader);
    private:
        glm::mat4 model;
        std::vector<Mesh*> meshes;
};

void sun(glm::mat4 view, glm::vec3 sunColor, Shader* shader);
void pointlight(Shader* shader, glm::mat4 view);
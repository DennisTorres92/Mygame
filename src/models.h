#pragma once
#include <vector>
#include <fstream>
#include "../shaders/shader.h"

#include "buffers.h"

class Mesh{
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
    private:
        std::vector<Mesh*> meshes;
};

void sun(glm::mat4 view, glm::vec3 sunColor, Shader* shader);
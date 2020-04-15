#include "models.h"


Mesh::Mesh(std::vector<Vertex>& vertices, uint64 numVertices, std::vector<uint32>& indices, Material material, uint64 numindexies, Shader* shader){
    this->material = material;
    this->shader = shader;
    this->numindexies = numindexies;
    vertexbuffer = new VertexBuffer(vertices.data(), numVertices);
    indexbuffer = new IndexBuffer(indices.data(), numindexies, sizeof(indices[0]));
    diffuselocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.diffuse"));
    specularlocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.specular"));
    emissivelocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.emissive"));
    shininesslocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.shininess"));
}
Mesh::~Mesh(){
    delete indexbuffer;
    delete vertexbuffer;
    //delete texturebuffer;
}
void Mesh::render(){
    vertexbuffer->bind();
    indexbuffer->bind();
    GLCALL(glUniform3fv(diffuselocation, 1, (float*)&material.diffuse));
    GLCALL(glUniform3fv(specularlocation, 1, (float*)&material.specular));
    GLCALL(glUniform3fv(emissivelocation, 1, (float*)&material.emissive));
    GLCALL(glUniform1f(shininesslocation, material.shininess));
    GLCALL(glDrawElements(GL_TRIANGLES, numindexies, GL_UNSIGNED_INT, 0));
}

void Model::init(const char* filename, Shader* shader){
    std::ifstream input = std::ifstream(filename, std::ios::in | std::ios::binary);
    if(!input.is_open()){
        std::cout<<"[ERROR] file: "<<filename<<" not found.\n";
        return;
    }
    uint64 nummeshes;
    input.read((char*)&nummeshes, sizeof(uint64));
    for(uint64 i = 0; i < nummeshes; i++){
        std::vector<Vertex> vertices;
        uint64 numVertices = 0;
        std::vector<uint32> indices;
        uint64 numindexies = 0;
        Material material;
        input.read((char*)&material.diffuse, sizeof(glm::vec3));
        input.read((char*)&material.emissive, sizeof(glm::vec3));
        input.read((char*)&material.specular, sizeof(glm::vec3));
        input.read((char*)&material.shininess, sizeof(float));
        input.read((char*)&numVertices, sizeof(uint64));
        input.read((char*)&numindexies, sizeof(uint64));
        for(uint64 i = 0; i < numVertices; i++) {
            Vertex vertex;
            input.read((char*)&vertex.positions.x, sizeof(float));
            input.read((char*)&vertex.positions.y, sizeof(float));
            input.read((char*)&vertex.positions.z, sizeof(float));
            input.read((char*)&vertex.normals.x, sizeof(float));
            input.read((char*)&vertex.normals.y, sizeof(float));
            input.read((char*)&vertex.normals.z, sizeof(float));
            vertices.push_back(vertex);
        }
        for(uint64 i = 0; i < numindexies; i++) {
            uint32 index;
            input.read((char*)&index, sizeof(uint32));
            indices.push_back(index);
        }
        Mesh* mesh = new Mesh(vertices, numVertices, indices, material, numindexies, shader);
        meshes.push_back(mesh);
    }
}
Model::~Model(){
    for(Mesh* mesh : meshes){
        delete mesh;
    }
    
}
void Model::render(){
    for(Mesh* mesh : meshes){
        mesh->render();
    }
}

void sun(glm::mat4 view, glm::vec3 sunColor, Shader* shader){
    glm::vec3 sunPos = glm::vec3(-1.0f);
    glm::vec4 transSunPosition = glm::transpose(glm::inverse(view)) * glm::vec4(sunPos, 1.0f);
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.direction"), 1, (float*)&transSunPosition));
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.diffuse"), 1, (float*)&sunColor));
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.specular"), 1, (float*)&sunColor));
    sunColor *= 0.3f;
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.ambient"), 1, (float*)&sunColor));
}
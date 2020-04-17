#include "models.h"


Mesh::Mesh(std::vector<Vertex>& vertices, uint64 numVertices, std::vector<uint32>& indices, Material material, 
uint64 numindexies, Shader* shader, const char* filediffuse, const char* filenormal){
    this->material = material;
    this->shader = shader;
    this->numindexies = numindexies;
    vertexbuffer = new VertexBuffer(vertices.data(), numVertices);
    indexbuffer = new IndexBuffer(indices.data(), numindexies, sizeof(indices[0]));
    texturebuffer = new TextureBuffer(filediffuse, material.diffuseMap, filenormal, material.normalMap);
    diffuselocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.diffuse"));
    specularlocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.specular"));
    emissivelocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.emissive"));
    shininesslocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_material.shininess"));
    diffuseMapLocation = GLCALL(glGetUniformLocation(shader->getShaderid(), "u_diffusemap"));
}
Mesh::~Mesh(){
    delete indexbuffer;
    delete vertexbuffer;
    delete texturebuffer;
}
void Mesh::render(){
    vertexbuffer->bind();
    indexbuffer->bind();
    GLCALL(glUniform3fv(diffuselocation, 1, (float*)&material.diffuse));
    GLCALL(glUniform3fv(specularlocation, 1, (float*)&material.specular));
    GLCALL(glUniform3fv(emissivelocation, 1, (float*)&material.emissive));
    GLCALL(glUniform1f(shininesslocation, material.shininess));
    texturebuffer->bind();
    GLCALL(glUniform1i(diffuseMapLocation, 0));
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
        uint64 diffuseMapNameLength = 0;
        input.read((char*)&diffuseMapNameLength, sizeof(uint64));
        std::string diffuseMapName(diffuseMapNameLength, '\0');
        input.read((char*)&diffuseMapName[0], diffuseMapNameLength);
        uint64 normalMapNameLength = 0;
        input.read((char*)&normalMapNameLength, sizeof(uint64));
        std::string normalMapName(normalMapNameLength, '\0');
        input.read((char*)&normalMapName[0], normalMapNameLength);
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
            input.read((char*)&vertex.texture.x, sizeof(float));
            input.read((char*)&vertex.texture.y, sizeof(float));
            vertices.push_back(vertex);
        }
        for(uint64 i = 0; i < numindexies; i++) {
            uint32 index;
            input.read((char*)&index, sizeof(uint32));
            indices.push_back(index);
        }
        Mesh* mesh = new Mesh(vertices, numVertices, indices, material, numindexies, shader, diffuseMapName.c_str(), normalMapName.c_str());
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
void Model::rotate(glm::mat4 view,glm::mat4 projection, float delta, Shader* shader){
    model = glm::rotate(model, 1.0f*delta, glm::vec3(0, 1, 0));
    glm::mat4 modelViewProj = projection * view * model;
    GLCALL(glUniformMatrix4fv(glGetUniformLocation(shader->getShaderid(), "u_modelViewProj"), 1, GL_FALSE, &modelViewProj[0][0]));
}

void sun(glm::mat4 view, glm::vec3 sunColor, Shader* shader){
    glm::vec3 sunPos = glm::vec3(-1.0f);
    glm::vec4 transSunPosition = glm::transpose(glm::inverse(view)) * glm::vec4(sunPos, 1.0f);
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.direction"), 1, (float*)&transSunPosition));
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.diffuse"), 1, (float*)&sunColor));
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.specular"), 1, (float*)&sunColor));
    sunColor *= 0.2f;
    GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_directional.ambient"), 1, (float*)&sunColor));
}
void pointlight(Shader* shader, glm::mat4 view, float delta){
    glm::vec3 pointLightColor = glm::vec3(1.0f, 0.01f, 0.01f);
	GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_pointlight.diffuse"), 1, (float*)&pointLightColor));
	GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_pointlight.specular"), 1, (float*)&pointLightColor));
	pointLightColor *= 0.1f;
	GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_pointlight.ambient"), 1, (float*)&pointLightColor));
	GLCALL(glUniform1f(glGetUniformLocation(shader->getShaderid(), "u_pointlight.linear"), 0.027f));
	GLCALL(glUniform1f(glGetUniformLocation(shader->getShaderid(), "u_pointlight.quadratic"), 0.0028f));
	glm::vec4 pointLightPosition = glm::vec4(-1.01f, 1.01f, 50.0f, 1.0f);
    glm::mat4 pointLightMatrix = glm::rotate(glm::mat4(1.0f), -delta, {0.0f, 1.0f, 0.0f});
	pointLightPosition = pointLightMatrix * pointLightPosition;
	glm::vec3 transformedPointLightPosition = (glm::vec3) (view * pointLightPosition);
	GLCALL(glUniform3fv(glGetUniformLocation(shader->getShaderid(), "u_pointlight.position"), 1, (float*)&transformedPointLightPosition));
}
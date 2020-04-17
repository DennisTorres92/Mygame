#define STB_IMAGE_IMPLEMENTATION
#include "buffers.h"    

// INDEXBUFFER
IndexBuffer::IndexBuffer(void* data, uint32 numIndexies, uint32 size){
    GLCALL(glGenBuffers(1, &bufferid));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferid));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexies * size, data, GL_STATIC_DRAW));
} 
IndexBuffer::~IndexBuffer(){
    GLCALL(glDeleteBuffers(1, &bufferid));
}
void IndexBuffer::bind(){
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferid));
}
void IndexBuffer::unbind(){
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//VERTEXBUFFER
VertexBuffer::VertexBuffer(void* data, uint32 numvertices){
    GLCALL(glGenVertexArrays(1, &vio));
    GLCALL(glBindVertexArray(vio));
    GLCALL(glGenBuffers(1, &bufferid));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, bufferid));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, numvertices * sizeof(Vertex), data, GL_DYNAMIC_DRAW));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4*3)));
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4*6)));
    this->numvertices = numvertices;
    GLCALL(glBindVertexArray(0));
}
VertexBuffer::~VertexBuffer(){
    GLCALL(glDeleteBuffers(1, &bufferid));
}
void VertexBuffer::bind(){
    GLCALL(glBindVertexArray(vio));
}
void VertexBuffer::unbind(){
    GLCALL(glBindVertexArray(0));
}


//TEXTURBUFFER
TextureBuffer::TextureBuffer(const char* filediffuse, int diffuseMap, const char* filenormal, int normalMap){
    idDiffuse = diffuseMap;
    GLCALL(glGenTextures(2, &idDiffuse));
    stbi_set_flip_vertically_on_load(true);
    {
        auto textureBuffer = stbi_load(filediffuse, &textureWidth, &textureHeight, &bitsPerPixel, 4);
        assert(textureBuffer);
        assert(idDiffuse);
        GLCALL(glBindTexture(GL_TEXTURE_2D, idDiffuse));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
    if(textureBuffer) {
        stbi_image_free(textureBuffer);
    }
    }
    
    idNormal = normalMap;
    {   
        auto textureBuffer = stbi_load(filenormal, &textureWidth, &textureHeight, &bitsPerPixel, 4);
        assert(textureBuffer);
        assert(idNormal);
        GLCALL(glBindTexture(GL_TEXTURE_2D, idNormal));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
        if(textureBuffer) {
            stbi_image_free(textureBuffer);
        }
    }
}
TextureBuffer::~TextureBuffer(){
    GLCALL(glDeleteTextures(1, &idNormal));
    GLCALL(glDeleteTextures(1, &idDiffuse));

}
void TextureBuffer::bind(){
    //GLCALL(glActiveTexture(GL_TEXTURE0));
	//GLCALL(glBindTexture(GL_TEXTURE_2D, idNormal));
    GLCALL(glBindTexture(GL_TEXTURE_2D, idDiffuse));
}
void TextureBuffer::unbind(){
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}


#include "Mesh.h"




Mesh::Mesh()
{
    vao.Create();
    vao.Bind();
}

void Mesh::AddBuffer(const void* bufferdata, unsigned int buffersize, const BufferLayout& buffer_layout)
{
    // u create a vbo 
    std::unique_ptr<VertexBuffers> vbo = std::make_unique<VertexBuffers>(bufferdata, buffersize);

    buffers.emplace_back(std::move(vbo));
    // u add the buffer

    vao.Bind();

    vao.addBuffer(*(buffers[BufferIndex]),buffer_layout);

    BufferIndex++;
}

void Mesh::AddIndices(const unsigned int* bufferdata, unsigned int buffersize)
{
    ibo.Create(bufferdata, buffersize);
    ibo.Bind();

}

const VertexArray& Mesh::getVertexArray() const
{
    return vao;
}

const IndexBuffer& Mesh::getIndexBuffer() const
{
    return ibo;
}

Mesh::~Mesh()
{
    
}

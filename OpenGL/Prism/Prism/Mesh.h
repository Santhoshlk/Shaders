#pragma once
#include <vector>
#include <memory>
#include "VertexArray.h"
#include "VertexBuffers.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"

class Mesh
{
    VertexArray vao;
    std::vector<std::unique_ptr<VertexBuffers>> buffers;
    IndexBuffer ibo;
    int BufferIndex = 0;

public:
    Mesh();

    void AddBuffer(const void* bufferdata, unsigned int buffersize , const BufferLayout& buffer_layout);
    
    void AddIndices(const unsigned int* bufferdata, unsigned int buffersize);


    // No copies allowed
    Mesh(const Mesh& mesh) = delete;
    void operator=(const Mesh& mesh) = delete;

    const VertexArray& getVertexArray() const;

    const IndexBuffer& getIndexBuffer() const;


    ~Mesh();
};

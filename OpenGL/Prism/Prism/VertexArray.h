#pragma once
#include "Renderer.h"

class VertexBuffers;
class BufferLayout;


class VertexArray
{
private:
    unsigned int m_RenderId;
public:
    // generate
    //Delete
    //bind 
    // unbind 
    // do the buffer layout for each buffer

    VertexArray();
    
    VertexArray(const VertexArray& va) = delete;
    VertexArray& operator=(const VertexArray& va) = delete;

    void Bind() const;
    void UnBind() const;
    void addBuffer(const VertexBuffers& buffer, const BufferLayout& bufferlayout);


    ~VertexArray();
};


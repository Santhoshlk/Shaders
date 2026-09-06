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

    void Bind();
    void UnBind();
    void addBuffer(const VertexBuffers& buffer, const BufferLayout& bufferlayout);


    ~VertexArray();
};


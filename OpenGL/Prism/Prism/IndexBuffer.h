#pragma once

#include "Renderer.h"

class IndexBuffer
{
private:
    // u need id and count 
    unsigned int m_RenderId;
    //unique indices ibo has to work with 
    unsigned int m_count;
public:
    IndexBuffer(const unsigned int* bufferdata, unsigned int buffersize);

    // unbind and bind matters in IndexBuffer
    void Bind();

    void UnBind();

    // do not have copy constuctor and assignment
    IndexBuffer(const IndexBuffer& ind) = delete;
    IndexBuffer& operator=(const IndexBuffer& ind) = delete;


    ~IndexBuffer();


};


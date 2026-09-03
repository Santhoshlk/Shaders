#pragma once

#include "Renderer.h"

class IndexBuffer
{
private:
    // u need id and count 
    unsigned int m_RenderId;
    unsigned int count;
public:
    IndexBuffer(const unsigned int* bufferdata, unsigned int buffersize);

    // unbind and bind matters in IndexBuffer
    void Bind();

    void UnBind();


    ~IndexBuffer();


};


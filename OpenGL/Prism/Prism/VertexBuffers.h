#pragma once

//dynamic linking
#include "Renderer.h"

class VertexBuffers
{
private:
    // fancy way for the index that open gl gives
    unsigned int m_RenderId;
public:
    VertexBuffers(const void* bufferdata, unsigned int buffersize);

    // functions are bind and unbind
   void Bind();
   void UnBind();


    ~VertexBuffers();


};


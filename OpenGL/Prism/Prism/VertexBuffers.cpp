#include "VertexBuffers.h"

VertexBuffers::VertexBuffers(const void* bufferdata, unsigned int buffersize)
{
    glGenBuffers(1, &m_RenderId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
    glBufferData(GL_ARRAY_BUFFER, buffersize, bufferdata, GL_STATIC_DRAW);
}

void VertexBuffers::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
}

void VertexBuffers::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffers::~VertexBuffers()
{
    // at deletion 
    glDeleteBuffers(1, &m_RenderId);
    m_RenderId = 0;
}

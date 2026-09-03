#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* bufferdata, unsigned int buffersize) : count(buffersize/sizeof(unsigned int))
{
    glGenBuffers(1, &m_RenderId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffersize, bufferdata, GL_STATIC_DRAW);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
}

void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RenderId);
    m_RenderId = 0;
    count = 0;

}

#include "VertexArray.h"

#include "VertexBuffers.h"
#include "BufferLayout.h"


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RenderId);
    glBindVertexArray(m_RenderId);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_RenderId);
}

void VertexArray::UnBind()
{
    glBindVertexArray(0);
}
    
void VertexArray::addBuffer(const VertexBuffers& buffer, const BufferLayout& bufferlayout)
{
    buffer.Bind();

    const auto& BufferElements = bufferlayout.getElements();

    for (const auto& element : BufferElements)
    {
        glEnableVertexAttribArray(element.index);
        glVertexAttribPointer(element.index, element.count, element.type, element.normalized, bufferlayout.getStride(), reinterpret_cast<const void*>(element.offset));
    }
    // so the next one would bind
    buffer.UnBind();

}

VertexArray::~VertexArray()
{
   
    glDeleteVertexArrays(1, &m_RenderId);
}

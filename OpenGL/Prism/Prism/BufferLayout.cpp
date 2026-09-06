#include "BufferLayout.h"

const std::vector<BufferElements>& BufferLayout::getElements() const
{
    return m_elements;
}

size_t BufferLayout::getStride() const
{
    return m_stride;
}

void BufferLayout::PushBuffers(unsigned int index, unsigned int count, GLenum type, bool normalized, unsigned int offset)
{

    int val = 0;
    switch (type)
    {
    case GL_FLOAT:
        val = 4;
        break;
    case GL_UNSIGNED_INT:
        val = 4;
        break;
    case GL_INT:
        val = 4;
        break;
    case GL_UNSIGNED_BYTE:
        val = 1;
        break;
    case GL_BYTE:
        val = 1;
        break;
    case GL_UNSIGNED_SHORT:
        val = 2;
        break;
    case GL_SHORT:
        val = 2;
        break;
    case GL_DOUBLE:
        val = 8;
        break;
    default:
        break;
    }

    // calculate the stride
    m_stride += count * (val);
    m_elements.push_back({index,count,type,normalized,offset});
}





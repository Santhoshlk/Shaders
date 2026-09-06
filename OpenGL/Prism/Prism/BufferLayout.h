#pragma once

#include "Renderer.h"
#include <vector>
// struct for the elements

struct BufferElements
{
    // index , count, type , normalized 
    unsigned int index;
    unsigned int count;
    GLenum type;
    bool normalized;
    unsigned int offset;
};


class BufferLayout
{
private:
  // u can have a vector that has all the layouts for one buffer
    std::vector<BufferElements> m_elements;
    // stride is common
    size_t m_stride;
public:
  // getter for the array
    const std::vector<BufferElements>& getElements() const;

    size_t getStride() const;

    // actual insertion
    void PushBuffers(unsigned int index, unsigned int count, GLenum type, bool normalized,unsigned int offset);
};


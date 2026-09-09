#pragma once

#include "Renderer.h"
#include <string>
class Texture
{
private:
    unsigned int m_RendererId;
    unsigned char* m_Buffer;
    std::string m_filepath;
   
    // u can store things for sampling
    int m_Width, m_Height, m_BPP;

public:

    Texture(std::string& filepath);

    void Bind(unsigned int slot = 0) const;

    void UnBind() const;
    ~Texture();
};


#pragma once

#include <string>
#include <GL/GL.h>

class Texture
{
private:
    unsigned int m_TextureId;
    int m_Width;
    unsigned char* m_Buffer;
    std::string m_filepath;
  
    // u can store things for sampling
    int m_Height, m_Depth;

public:

    Texture( const std::string& filepath ,  int InterpolationFilter = GL_LINEAR , int WrapFilter = GL_REPEAT);

    void Bind(unsigned int slot = 0) const;

    Texture(const Texture& s) = delete;
    Texture& operator = (const Texture &s) = delete;

    void UnBind() const;

    ~Texture();
};


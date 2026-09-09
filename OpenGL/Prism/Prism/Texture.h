#pragma once

#include <string>
class Texture
{
private:
    unsigned int m_RendererId;
    int m_Width;
    unsigned char* m_Buffer;
    std::string m_filepath;
  
    // u can store things for sampling
    int m_Height, m_BPP;

public:

    Texture( const std::string& filepath);

    void Bind(unsigned int slot = 0) const;

    Texture(const Texture& s) = delete;
    Texture& operator = (const Texture &s) = delete;

    void UnBind() const;

    ~Texture();
};


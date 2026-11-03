#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

Texture::Texture(const std::string& filepath,  int InterpolationFilter,  int WrapFilter) : m_filepath(filepath)
{

    stbi_set_flip_vertically_on_load(1);
    m_Buffer = stbi_load(filepath.c_str(),&m_Width,&m_Height,&m_Depth,4); 

    if (!m_Buffer)
    {
        std::cout << "The File did not load properly" << stbi_failure_reason() << std::endl;
    }

    glGenTextures(1,&m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    // Set the filters
    if (InterpolationFilter == GL_LINEAR_MIPMAP_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, InterpolationFilter);
    }
    else if (InterpolationFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, InterpolationFilter);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, InterpolationFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, InterpolationFilter);
    }
  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,WrapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapFilter);

    // u need to send it to the gpu

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,m_Width,m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_Buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(m_Buffer);

}

void Texture::Bind(unsigned int slot) const
{
    assert("The slot used is invalid" && slot < 32);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D,m_TextureId);
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    m_Buffer = nullptr;
    m_TextureId = 0;
    m_Height = -1;
    m_Width = -1;
    m_Depth = -1;
    m_filepath = "";
}

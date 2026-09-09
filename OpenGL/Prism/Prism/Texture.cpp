#include "Texture.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <iostream>
#include <cassert>

Texture::Texture(const std::string& filepath) : m_filepath(filepath)
{
    // load the image
    stbi_set_flip_vertically_on_load(1);

    // u chose unsigned buffer because its unsigned buffer as a return type
   m_Buffer = stbi_load(filepath.c_str(),&m_Width,&m_Height,&m_BPP,4);
 
    if (!m_Buffer)
    {
        std::cout << "The file is not loaded" << std::endl;
    }
   // generate the texture
   glGenTextures(1,&m_RendererId);
   glBindTexture(GL_TEXTURE_2D, m_RendererId);

   // before giving data set the parameters
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

   // now give the data to gpu and free the local copy for now 
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,m_Width,m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_Buffer);

   if (m_Buffer)
   {
       // then free the data
       stbi_image_free(m_Buffer);
   }

}

void Texture::Bind(unsigned int slot) const
{
   assert("slot are only allowed upto 31" && slot < 32);
   glActiveTexture(GL_TEXTURE0 + slot);
   glBindTexture(GL_TEXTURE_2D, m_RendererId);
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererId);
    m_Width = -1;
    m_Height = -1;
    m_BPP = -1;
}

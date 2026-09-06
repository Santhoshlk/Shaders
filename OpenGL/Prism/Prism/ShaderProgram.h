#pragma once

#include "Renderer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// create a struct for storing the string of vertex and fragment shaders
struct Shaders
{
    std::string vs;
    std::string fs;
};

enum class ShaderType : uint8_t
{
    none,
    vertex,
    fragment
};


Shaders ParseFile(const std::string& infile);



class ShaderProgram
{
private:
    unsigned int m_ProgramId;

    unsigned int CompileShader(unsigned int ShaderType, const std::string& Shader);

    unsigned int  CreateShaderProgram(const std::string& VertexShader, const std::string& FragmentShader);
public:
    
    ShaderProgram(const std::string& filetoParse);
  
    void Bind();
    void UnBind();

    const unsigned int& getProgramId() const;

    ~ShaderProgram();


};


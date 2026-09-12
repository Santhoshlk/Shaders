#pragma once

#include "Renderer.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

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
 // the shader program should contain a storage for all th uniform location it has
    std::unordered_map<std::string, int> uniformcache;

    unsigned int m_ProgramId;

    unsigned int CompileShader(unsigned int ShaderType, const std::string& Shader);

    unsigned int  CreateShaderProgram(const std::string& VertexShader, const std::string& FragmentShader);
public:
    
    void SetUniform4f(const std::string& uniform,float f1,float f2,float f3,float f4);

    void SetUnifom1f(const std::string& uniform, float f1);

    void SetUniform1i(const std::string& uniform, int i);

    void SetUniformMat4(const std::string& unifrom, const glm::mat4& mat);

    int Uniformlocation(const std::string& uniform);

    ShaderProgram(const std::string& filetoParse);
  
    void Bind() const;
    void UnBind() const;

   inline const unsigned int& getProgramId() const;

    ~ShaderProgram();


};

